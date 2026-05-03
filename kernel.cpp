#include <iostream>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include "resource.h"
#include "process.h"
#include "kernel.h"
#include "scheduler.h"
using namespace std;

// ─── Global mutex ────────────────────────────────────────────────────────────
// Protects the PCB linked list and resource counters from race conditions
// between the main thread (launching tasks) and the reaper thread (cleaning up)
pthread_mutex_t gMutex = PTHREAD_MUTEX_INITIALIZER;

struct PCBNode {
    PCB*     pcb;
    PCBNode* next;
    PCBNode(PCB* p) : pcb(p), next(nullptr) {}
};

static PCBNode* head = nullptr;

void addProcess(PCB* pcb) {
    PCBNode* node = new PCBNode(pcb);
    if (!head) { head = node; return; }
    PCBNode* temp = head;
    while (temp->next) temp = temp->next;
    temp->next = node;
}

void removeProcess(PCB* pcb) {
    if (!head) return;
    PCBNode* temp = head;
    PCBNode* prev = nullptr;
    while (temp) {
        if (temp->pcb->pid == pcb->pid) {
            if (!prev) head = temp->next;
            else       prev->next = temp->next;
            delete temp->pcb;  
            delete temp;      
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

void listProcess() {
    if (!head) { cout << "No processes running" << endl; return; }
    const char* stateNames[] = { "Ready", "Running", "Blocked", "Terminated" };
    const char* levelNames[] = { "Foreground(RR)", "Background(FCFS)", "System(Priority)" };
    PCBNode* temp = head;
    while (temp) {
        cout << "PID:       " << temp->pcb->pid                          << endl;
        cout << "PPID:      " << temp->pcb->ppid                         << endl;
        cout << "Name:      " << temp->pcb->name                         << endl;
        cout << "State:     " << stateNames[temp->pcb->state]            << endl;
        cout << "Level:     " << levelNames[temp->pcb->taskLevel]        << endl;
        cout << "Priority:  " << temp->pcb->priority                     << endl;
        cout << "RAM:       " << temp->pcb->requiredRAM   << " MB"       << endl;
        cout << "HDD:       " << temp->pcb->requiredHardDisk << " MB"   << endl;
        cout << "------------------------" << endl;
        temp = temp->next;
    }
}


void killProcess(int pid) {
    PCBNode* temp = head;
    while (temp) {
        if (temp->pcb->pid == pid) {
            kill(pid, SIGKILL);
            temp->pcb->state = Terminated;
            cout << "Process " << pid << " killed." << endl;
            return;
        }
        temp = temp->next;
    }
    cout << "PID " << pid << " not found." << endl;
}

void stopProcess(int pid) {
    PCBNode* temp = head;
    while (temp) {
        if (temp->pcb->pid == pid) {
            kill(pid, SIGSTOP);
            temp->pcb->state = Blocked;
            cout << "Process " << pid << " paused (minimized)." << endl;
            return;
        }
        temp = temp->next;
    }
    cout << "PID " << pid << " not found." << endl;
}

void resumeProcess(int pid) {
    PCBNode* temp = head;
    while (temp) {
        if (temp->pcb->pid == pid) {
            if (temp->pcb->state == Blocked) {
                temp->pcb->state = Ready;
                cout << "Process " << pid << " resumed (Moved to Ready Queue)." << endl;
                scheduleProcess(temp->pcb); // Let the scheduler handle the SIGCONT when it's picked up
            } else {
                cout << "Process " << pid << " is not blocked." << endl;
            }
            return;
        }
        temp = temp->next;
    }
    cout << "PID " << pid << " not found." << endl;
}

void killAllProcesses() {
    PCBNode* temp = head;
    while (temp) {
        kill(temp->pcb->pid, SIGKILL);
        freeRam(temp->pcb->requiredRAM);
        PCBNode* next = temp->next;
        delete temp->pcb;
        delete temp;
        temp = next;
    }
    head = nullptr;
    availableCores = totalCores;
    cout << "All processes terminated. Resources freed." << endl;
}

// ─── Process reaper ───────────────────────────────────────────────────────────
//
// KEY FIX — why the old approach was broken:
//
//   Old code iterated the PCB list and called waitpid(specific_pid, WNOHANG).
//   This means:
//     1. DENIED children (never added to PCB) were never reaped → zombies.
//     2. The function only ran at the top of the menu loop, so resources
//        were not freed until the user typed the NEXT menu choice.
//
//   New approach — flip the loop:
//     Call waitpid(-1, WNOHANG) to ask "did ANY child die?"
//     If yes, look that pid up in the PCB list.
//       - Found  → free its RAM + core, remove from list.
//       - Not found → it was a DENIED child; just reaped, nothing to free.
//     Repeat until no more dead children this cycle.
//
//   This single change fixes BOTH problems at once.
//
void checkAndCleanProcesses() {
    int    status;
    pid_t  dead;

    // Loop until no more zombie children exist right now
    while ((dead = waitpid(-1, &status, WNOHANG)) > 0) {

        // Search PCB list for this pid
        PCBNode* temp = head;
        while (temp) {
            if (temp->pcb->pid == dead) {
                // This was a running (granted) process — free its resources
                freeRam(temp->pcb->requiredRAM);
                removeProcess(temp->pcb);
                reprintStatusInPlace(); // update the header in-place instantly
                break;
            }
            temp = temp->next;
        }
        // If temp == nullptr here, pid wasn't in PCB (= denied child).
        // waitpid already reaped it; nothing else to do.
    }
}