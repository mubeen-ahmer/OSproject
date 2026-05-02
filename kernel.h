#ifndef KERNEL_H
#define KERNEL_H

#include "process.h"
void checkAndCleanProcesses();
void addProcess(PCB* pcb);
void removeProcess(PCB* pcb);
void listProcess();

#endif
