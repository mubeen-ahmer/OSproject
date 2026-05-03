#include <iostream>
#include <climits>
#include "ready_queue.h"
using namespace std;

// ─── Three level queues ───────────────────────────────────────────────────────
static ReadyQueue queues[3]; // index = TASK_LEVEL (0,1,2)

void initQueues() {
    for (int i = 0; i < 3; i++) {
        queues[i].front = nullptr;
        queues[i].rear  = nullptr;
        queues[i].count = 0;
    }
}

// ─── ReadyQueue methods ───────────────────────────────────────────────────────
void ReadyQueue::enqueue(PCB* pcb) {
    QueueNode* node = new QueueNode(pcb);
    if (!rear) { front = rear = node; }
    else       { rear->next = node; rear = node; }
    count++;
}

PCB* ReadyQueue::dequeue() {
    if (!front) return nullptr;
    QueueNode* node = front;
    PCB* pcb        = node->pcb;
    front           = front->next;
    if (!front) rear = nullptr;
    delete node;
    count--;
    return pcb;
}

PCB* ReadyQueue::peekFront() {
    return front ? front->pcb : nullptr;
}

bool ReadyQueue::isEmpty() const {
    return front == nullptr;
}

// ─── Public API ───────────────────────────────────────────────────────────────

void addToQueue(PCB* pcb) {
    int level = (int)pcb->taskLevel;        // 0, 1, or 2
    pcb->state = Ready;
    queues[level].enqueue(pcb);
}

// Returns the next process to give CPU time to, following multilevel policy:
//   1. Level 0 (Round Robin)  — checked first
//   2. Level 1 (FCFS)         — only if Level 0 is empty
//   3. Level 2 (Priority)     — only if Levels 0+1 are empty
//                               within Level 2, pick lowest priority number
PCB* getNextProcess() {
    // Level 0 — just dequeue (caller re-enqueues for RR after quantum)
    if (!queues[0].isEmpty())
        return queues[0].dequeue();

    // Level 1 — FCFS: plain dequeue
    if (!queues[1].isEmpty())
        return queues[1].dequeue();

    // Level 2 — Priority: scan for lowest priority number
    if (!queues[2].isEmpty()) {
        // Find node with lowest priority value
        QueueNode* cur  = queues[2].front;
        QueueNode* best = cur;
        while (cur) {
            if (cur->pcb->priority < best->pcb->priority)
                best = cur;
            cur = cur->next;
        }
        // Remove best from queue manually
        PCB* chosen = best->pcb;
        QueueNode* prev = nullptr;
        QueueNode* tmp  = queues[2].front;
        while (tmp && tmp != best) { prev = tmp; tmp = tmp->next; }
        if (!prev) queues[2].front = best->next;
        else       prev->next      = best->next;
        if (best == queues[2].rear) queues[2].rear = prev;
        delete best;
        queues[2].count--;
        return chosen;
    }

    return nullptr; // all queues empty
}

int queueSize(int level) {
    if (level < 0 || level > 2) return 0;
    return queues[level].count;
}
