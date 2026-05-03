#ifndef READY_QUEUE_H
#define READY_QUEUE_H

#include "process.h"

// ─── Ready Queue ──────────────────────────────────────────────────────────────
// Three separate FIFO queues, one per scheduling level:
//   Level 0 (FOREGROUND)  → Round Robin      (interactive tasks)
//   Level 1 (BACKGROUND)  → FCFS             (auto-finish tasks)
//   Level 2 (SYSTEM)      → Priority         (continuous utilities)
//
// addToQueue() inserts into the correct level automatically.
// The scheduler drains Level 0 first, then 1, then 2.

struct QueueNode {
    PCB*       pcb;
    QueueNode* next;
    explicit QueueNode(PCB* p) : pcb(p), next(nullptr) {}
};

struct ReadyQueue {
    QueueNode* front;
    QueueNode* rear;
    int        count;

    ReadyQueue() : front(nullptr), rear(nullptr), count(0) {}

    void enqueue(PCB* pcb);
    PCB* dequeue();        // remove from front (FCFS / RR order)
    PCB* peekFront();      // look without removing
    bool isEmpty() const;
};

// ─── Public API ───────────────────────────────────────────────────────────────
void initQueues();
void addToQueue(PCB* pcb);         // routes to correct level queue
PCB* getNextProcess();             // picks next by multilevel policy
int  queueSize(int level);         // number of PCBs waiting at a level

#endif
