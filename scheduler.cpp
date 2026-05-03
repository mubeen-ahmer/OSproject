#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include "scheduler.h"
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

        // ── Context switch IN: resume the selected process ────────────────────
        pthread_mutex_lock(&gMutex);
        pcb->state = Running;
        pthread_mutex_unlock(&gMutex);

        kill(pcb->pid, SIGCONT); // resume if it was stopped

        if (pcb->taskLevel == FOREGROUND) {
            // ── Round Robin: run for one quantum then preempt ─────────────────
            sleep(RR_QUANTUM_SEC);

            // Check if the process is still alive before stopping it
            if (kill(pcb->pid, 0) == 0) {
                // Context switch OUT: pause the process
                kill(pcb->pid, SIGSTOP);

                pthread_mutex_lock(&gMutex);
                pcb->state = Ready;
                pthread_mutex_unlock(&gMutex);

                // Re-enqueue for next round (RR rotation)
                pthread_mutex_lock(&gSchedMutex);
                addToQueue(pcb);
                pthread_mutex_unlock(&gSchedMutex);

                sem_post(&gTaskSemaphore); // keep the semaphore count accurate
            }
            // If process died during quantum, reaper thread handles cleanup

        } else {
            // ── FCFS (Level 1) and Priority (Level 2): run to completion ─────
            // These levels are non-preemptive — just mark Running, let reaper
            // handle cleanup when they naturally finish.
            // No re-enqueue needed.
        }
    }
    return nullptr;
}
