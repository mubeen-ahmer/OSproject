#ifndef RESOURCE_H
#define RESOURCE_H

int initialize(int ram, int hard, int core);
int allocateRam(int ram);
void freeRam(int ram);
int allocateHardDisk(int hdd);
void freeHardDisk(int hdd);
int allocateCore();
void freeCore();
void printStatus();

#endif
