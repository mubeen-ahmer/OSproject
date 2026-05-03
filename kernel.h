#ifndef KERNEL_H
#define KERNEL_H

#include <pthread.h>
#include "process.h"

// Shared mutex — lock this whenever touching the PCB list or resource counters
extern pthread_mutex_t gMutex;

void checkAndCleanProcesses();
void addProcess(PCB* pcb);
void removeProcess(PCB* pcb);
void listProcess();

// Kernel-mode process control
void killProcess(int pid);    // SIGKILL → terminate
void stopProcess(int pid);    // SIGSTOP → minimize/pause
void resumeProcess(int pid);  // SIGCONT → resume
void killAllProcesses();      // reset: kill every running process + free resources

#endif