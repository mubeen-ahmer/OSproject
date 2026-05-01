#include<iostream>
using namespace std;
int totalRam;
int availableRam;
int totalHardDisk;
int availableHardDisk;
int totalCores;
int availableCores;



int initialize(int ram,int hard,int core){
    if ( ram < 0 || hard < 0 || core < 0 ){
        return 1;
    }
    totalRam = ram;
    totalHardDisk = hard;
    totalCores=core;
    availableRam = ram;
    availableHardDisk = hard;
    availableCores = core;
    return 0;
}

int allocateRam(int ram){
    if(ram>availableRam){
        cout<<"Not sufficient RAM"<<endl;
        return 1;
    }
    availableRam-=ram;
    return 0;
}

void freeRam(int ram){
    availableRam+=ram;
    if(availableRam>totalRam) availableRam=totalRam;
}

int allocateHardDisk(int hdd){
    if(hdd > availableHardDisk){
        cout<<"Not sufficient HDD"<<endl;
        return 1;
    }
    availableHardDisk -= hdd;
    return 0;
}

void freeHardDisk(int hdd){
    availableHardDisk += hdd;
    if(availableHardDisk > totalHardDisk) availableHardDisk = totalHardDisk;
}

int allocateCore(){
    if(availableCores <= 0){
        cout<<"No cores available"<<endl;
        return 1;
    }
    availableCores--;
    return 0;
}

void freeCore(){
    availableCores++;
    if(availableCores > totalCores) availableCores = totalCores;
}

void printStatus(){
    cout<<"Total Ram : "<<totalRam<<endl;
    cout<<"Available Ram : "<<availableRam<<endl;
    cout<<"Total HDD : "<<totalHardDisk<<endl;
    cout<<"Availabe HDD : "<<availableHardDisk<<endl;
    cout<<"Total Cores : "<<totalCores<<endl;
    cout<<"Available Cores : "<<availableCores<<endl;
}