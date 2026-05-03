#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include "scheduler.h"
#include "resource.h"
#include "ready_queue.h"
#include "kernel.h"
#include "process.h"
using namespace std;

// ─── Synchronisation primitives (definitions) ─────────────────────────────────
pthread_mutex_t gSchedMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  gQueueCond  = PTHREAD_COND_INITIALIZER;
sem_t           gTaskSemaphore;

// ─── Init ─────────────────────────────────────────────────────────────────────
void initScheduler(int cores) {
    initQueues();
    // Semaphore starts at 0; scheduleProcess() posts to it each time a task
    // is granted, signalling the scheduler that something is runnable.
    // We initialise to 0 (no tasks yet). Max = cores count in practice.
    sem_init(&gTaskSemaphore, 0, 0);
}

// ─── Called from OS main thread after GRANT ───────────────────────────────────
void scheduleProcess(PCB* pcb) {
    pthread_mutex_lock(&gSchedMutex);
    addToQueue(pcb);                  // put in appropriate level queue
    pthread_cond_signal(&gQueueCond); // wake scheduler if sleeping
    pthread_mutex_unlock(&gSchedMutex);
    sem_post(&gTaskSemaphore);        // increment semaphore (one more runnable)
}

// ─── Scheduler thread ─────────────────────────────────────────────────────────
// This thread implements the multilevel queue scheduling logic:
//
//   It does NOT actually execute processes (they run inside xterm via fork/exec).
//   What it DOES do:
//     1. Maintains process state transitions (Ready → Running → back to Ready for RR)
//     2. Sends SIGSTOP/SIGCONT to enforce time-slice context switching for RR
//     3. Uses condition variable to sleep when no work, semaphore to count tasks
//
// Context switching for Round Robin:
//   - Run the Level-0 process for RR_QUANTUM_SEC seconds (SIGCONT)
//   - After quantum: send SIGSTOP (pause), re-enqueue, pick next
//
void* schedulerThread(void* arg) {
    (void)arg;
    while (true) {
        // ── Wait until at least one process is in a queue ─────────────────────
        sem_wait(&gTaskSemaphore); // blocks until scheduleProcess() posts

        pthread_mutex_lock(&gSchedMutex);

        // In case of spurious wakeup or queue already drained, use cond_wait
        while (queueSize(0) == 0 && queueSize(1) == 0 && queueSize(2) == 0) {
            pthread_cond_wait(&gQueueCond, &gSchedMutex);
        }

        PCB* pcb = getNextProcess(); // picks by multilevel policy
        pthread_mutex_unlock(&gSchedMutex);

        if (!pcb) continue;

        // ── Check if it was manually Blocked while in the queue ───────────────
        pthread_mutex_lock(&gMutex);
        if (pcb->state == Blocked) {
            pthread_mutex_unlock(&gMutex);
            continue; // Drop it, don't run it
        }

        // ── Context switch IN: resume the selected process ────────────────────
        allocateCore(); // This core is now BUSY
        pcb->state = Running;
        reprintStatusInPlace();
        pthread_mutex_unlock(&gMutex);

        kill(pcb->pid, SIGCONT); // resume if it was stopped

        if (pcb->taskLevel == FOREGROUND) {
            // ── Round Robin: run for one quantum then preempt ─────────────────
            bool died = false;
            for (int i = 0; i < (RR_QUANTUM_SEC * 10); i++) {
                if (kill(pcb->pid, 0) != 0) { 
                    died = true; 
                    break; 
                }
                if (pcb->state == Blocked) {
                    died = true; // treat as died so it doesn't re-enqueue
                    break;
                }
                usleep(100000); // 0.1s
            }

            if (!died) {
                // Context switch OUT: pause the process
                pthread_mutex_lock(&gMutex);
                if (pcb->state != Blocked) {
                    kill(pcb->pid, SIGSTOP);
                    pcb->state = Ready;
                    pthread_mutex_unlock(&gMutex);

                    // Re-enqueue for next round (RR rotation)
                    pthread_mutex_lock(&gSchedMutex);
                    addToQueue(pcb);
                    pthread_mutex_unlock(&gSchedMutex);

                    sem_post(&gTaskSemaphore); // keep the semaphore count accurate
                } else {
                    pthread_mutex_unlock(&gMutex);
                }
            }

        } else {
            // ── FCFS (Level 1) and Priority (Level 2): run to completion ─────
            while (kill(pcb->pid, 0) == 0) {
                if (pcb->state == Blocked) break;
                usleep(500000); // Check every 0.5s if it finished
            }
        }

        // ── Free Core: Task is either paused (RR) or dead ─────────────────────
        pthread_mutex_lock(&gMutex);
        freeCore(); // Core is available for the next iteration
        reprintStatusInPlace();
        pthread_mutex_unlock(&gMutex);
    }
    return nullptr;
}
