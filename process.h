#ifndef PROCESS_H
#define PROCESS_H
#include<cstring>

enum STATE{ Ready , Running , Blocked , Terminated };

struct PCB {
    int pid;
    int ppid;
    char name[50];
    STATE state;
    int requiredRAM;
    int requiredHardDisk;
    PCB(int pid, int ppid, const char* name, STATE state, int requiredRAM, int requiredHardDisk = 0) {
    this->pid = pid;
    this->ppid = ppid;
    strncpy(this->name, name, 49);
    this->name[49] = '\0';
    this->state = state;
    this->requiredRAM = requiredRAM;
    this->requiredHardDisk = requiredHardDisk;
}

};

#endif