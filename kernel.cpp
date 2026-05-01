#include <iostream>
using namespace std;
#include "process.h"
struct PCBNode{
    PCB*pcb;
    PCBNode*next;
    PCBNode(PCB*p):pcb(p),next(nullptr){};
};
PCBNode*head=nullptr;

void addProcess(PCB*pcb){
    PCBNode*node=new PCBNode(pcb);
    if (!head){
        head=node; 
        return;
    }
    PCBNode*temp=head;
    while(temp->next){
        temp=temp->next;
    }
    temp->next=node;
}
void removeProcess(PCB*pcb){
    if(!head) return;
    PCBNode* temp = head;
    PCBNode* prev = nullptr;
    while(temp){
        if(temp->pcb->pid == pcb->pid){
            if(!prev)
                head = temp->next;
            else
                prev->next = temp->next;
            delete temp;
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

void listProcess(){
    if(!head){
        cout << "No processes running" << endl;
        return;
    }
    
    string stateNames[] = {"Ready", "Running", "Blocked", "Terminated"};
    
    PCBNode* temp = head;
    while(temp){
        cout << "PID: "       << temp->pcb->pid                    << endl;
        cout << "PPID: "      << temp->pcb->ppid                   << endl;
        cout << "Name: "      << temp->pcb->name                   << endl;
        cout << "State: "     << stateNames[temp->pcb->state]      << endl;
        cout << "RAM: "       << temp->pcb->requiredRAM             << endl;
        cout << "HardDisk: "  << temp->pcb->requiredHardDisk        << endl;
        cout << "------------------------" << endl;
        temp = temp->next;
    }
}