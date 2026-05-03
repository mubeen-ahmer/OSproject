#ifndef PROCESS_H
#define PROCESS_H
#include <cstring>

enum STATE { Ready, Running, Blocked, Terminated };

// Task classification for multilevel queue:
//   Level 0 — Foreground/Interactive  → Round Robin
//   Level 1 — Background/Auto-finish  → FCFS
//   Level 2 — System/Utility          → Priority (lowest preemption)
enum TASK_LEVEL { FOREGROUND = 0, BACKGROUND = 1, SYSTEM = 2 };

struct PCB {
    int        pid;
    int        ppid;
    char       name[50];
    STATE      state;
    int        requiredRAM;
    int        requiredHardDisk;
    TASK_LEVEL taskLevel;   // which scheduler queue this belongs to
    int        priority;    // lower number = higher priority (used by Level 2)

    PCB(int pid, int ppid, const char* name, STATE state,
        int requiredRAM, int requiredHardDisk = 0,
        TASK_LEVEL taskLevel = FOREGROUND, int priority = 0)
    {
        this->pid             = pid;
        this->ppid            = ppid;
        strncpy(this->name, name, 49);
        this->name[49]        = '\0';
        this->state           = state;
        this->requiredRAM     = requiredRAM;
        this->requiredHardDisk = requiredHardDisk;
        this->taskLevel       = taskLevel;
        this->priority        = priority;
    }
};

#endif