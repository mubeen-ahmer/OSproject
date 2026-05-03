#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <semaphore.h>
#include <pthread.h>
#include "process.h"

// ─── Scheduler synchronisation primitives ────────────────────────────────────
//
//  gSchedMutex    — protects the ready queues from concurrent access
//  gQueueCond     — condition variable: scheduler sleeps when all queues empty,
//                   wakes when addToQueue() signals a new process arrived
//  gTaskSemaphore — counting semaphore: limits concurrent running tasks to the
//                   number of available CPU cores (enforces core allocation)
//
extern pthread_mutex_t gSchedMutex;
extern pthread_cond_t  gQueueCond;
extern sem_t           gTaskSemaphore;

#define RR_QUANTUM_SEC 0.2   // Round-Robin time slice in seconds

void  initScheduler(int cores);   // call once at boot with core count
void  scheduleProcess(PCB* pcb);  // hand a newly granted PCB to the scheduler
void* schedulerThread(void* arg); // pthread entry point

#endif
