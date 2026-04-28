enum state{ Ready , Running , Blocked , Terminated };
struct PCB {
    int pid;
    int ppid;
    char name[50];
    int state;
    int requiredRAM;
    int requiredHardDisk;
};