#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/wait.h>
#include "config.h"
#include <cstring>
#include "kernel.h"
#include "resource.h"
#include "process.h"
using namespace std;
void gotoxy(int ,int);
void welcomeScreen();
void goodbye();

int main(int argc,char* argv[]){
    if (argc < 4){
        cout<<"wrong command"<<endl;
        cout<<"use : ./OS RAM HDD Cores"<<endl;
        return 1;
    }
    int ram=atoi(argv[1]);
    int hdd=atoi(argv[2]);
    int core=atoi(argv[3]);
    if(initialize(ram,hdd,core)==1){
        cout<<"invalid arguments,none of these can't be negative"<<endl;
        return 1;
    }
    welcomeScreen();
    int choice;
    do{
        system("clear");
        printStatus();
        cout << "\n===== windows =====" << endl;
        cout << "/1.  Calculator      [RAM: " << CALC_RAM        << " MB]" << endl;
        cout << "/2.  Notepad         [RAM: " << NOTEPAD_RAM     << " MB]" << endl;
        cout << "/3.  Clock           [RAM: " << CLOCK_RAM       << " MB]" << endl;
        cout << "/4.  Calendar        [RAM: " << CALENDAR_RAM    << " MB]" << endl;
        cout << "/5.  File Copy       [RAM: " << FILE_COPY_RAM   << " MB]" << endl;
        cout << "/6.  File Move       [RAM: " << FILE_MOVE_RAM   << " MB]" << endl;
        cout << "/7.  File Delete     [RAM: " << FILE_DELETE_RAM << " MB]" << endl;
        cout << "/8.  File Create     [RAM: " << FILE_CREATE_RAM << " MB]" << endl;
        cout << "/9.  File Info       [RAM: " << FILE_INFO_RAM   << " MB]" << endl;
        cout << "10. File Editor     [RAM: " << FILE_EDITOR_RAM << " MB]" << endl;
        cout << "11. Text Search     [RAM: " << TEXT_SEARCH_RAM << " MB]" << endl;
        cout << "12. Minesweeper     [RAM: " << MINESWEEPER_RAM << " MB]" << endl;
        cout << "/13. Music Sim       [RAM: " << MUSIC_SIM_RAM   << " MB]" << endl;
        cout << "/14. Print Sim       [RAM: " << PRINT_SIM_RAM   << " MB]" << endl;
        cout << "/15. RAM Viewer      [RAM: " << RAM_VIEWER_RAM  << " MB]" << endl;
        cout << "16. Process Viewer  [RAM: " << PROC_VIEWER_RAM << " MB]" << endl;
        cout << "/17. Log Generator   [RAM: " << LOG_GEN_RAM     << " MB]" << endl;
        cout << "/18. Random Number   [RAM: " << RAND_GEN_RAM    << " MB]" << endl;
        cout << "/19. Timer/Alarm     [RAM: " << TIMER_RAM       << " MB]" << endl;
        cout << "20. Auto Backup     [RAM: " << AUTO_BACKUP_RAM << " MB]" << endl;
        cout << "0.  Shutdown" << endl;
        cout << "\nSelect: ";
        cin >> choice;

        switch(choice){
            case 1:{
                int p1fd[2];
                int p2fd[2];
                pipe(p1fd); //pipe1 : parent writes, child reads
                pipe(p2fd); //pipe2 : child writes, parent reads
                pid_t pid=fork();
                if(pid==0){  //child
                    int ram=CALC_RAM;
                    close(p1fd[1]);
                    close(p2fd[0]);

                    string message="REQUEST "+to_string(ram);
                    const char *msg=message.c_str();
                    write(p2fd[1],msg,strlen(msg));

                    char buffer[100];
                    int n=read(p1fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    if(strcmp(buffer,"GRANT")==0){
                        execlp("xterm", "xterm", "-e", "/home/mubeen-ahmer/OS/tasks/calculator", NULL);    
                        exit(0);
                    }
                    else if(strcmp(buffer,"DENY")==0){
                        exit(1);
                    }
                }
                else{   //parent
                    close(p1fd[0]);
                    close(p2fd[1]);

                    char buffer[100];
                    int n=read(p2fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    string received(buffer);
                    
                    const char*permission;
                    string msg;
                    
                    PCB* pcb = nullptr;
                    int requestedRam=stoi(received.substr(8));
                    if(allocateRam(requestedRam) != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        // no RAM was taken, nothing to free
                    }
                    else if(allocateCore() != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        freeRam(requestedRam);  // give RAM back since core failed
                    }
                    else {
                        msg="GRANT";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));
                        pcb = new PCB(pid, getpid(), "calculator", Running, CALC_RAM);
                        addProcess(pcb);
                    }
                    int status;
                    wait(&status);
                    if(pcb != nullptr){
                        removeProcess(pcb);
                        freeRam(requestedRam);
                        freeCore();
                        delete pcb;
                    }
                }
            }
            break;
            case 2:{
                int p1fd[2];
                int p2fd[2];
                pipe(p1fd); //pipe1 : parent writes, child reads
                pipe(p2fd); //pipe2 : child writes, parent reads
                pid_t pid=fork();
                if(pid==0){  //child
                    int ram=NOTEPAD_RAM;
                    close(p1fd[1]);
                    close(p2fd[0]);

                    string message="REQUEST "+to_string(ram);
                    const char *msg=message.c_str();
                    write(p2fd[1],msg,strlen(msg));

                    char buffer[100];
                    int n=read(p1fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    if(strcmp(buffer,"GRANT")==0){
                        execlp("xterm", "xterm", "-e", "/home/mubeen-ahmer/OS/tasks/notepad", NULL);    
                        exit(0);
                    }
                    else if(strcmp(buffer,"DENY")==0){
                        exit(1);
                    }
                }
                else{   //parent
                    close(p1fd[0]);
                    close(p2fd[1]);

                    char buffer[100];
                    int n=read(p2fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    string received(buffer);
                    
                    const char*permission;
                    string msg;
                    
                    PCB* pcb = nullptr;
                    int requestedRam=stoi(received.substr(8));
                    if(allocateRam(requestedRam) != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        // no RAM was taken, nothing to free
                    }
                    else if(allocateCore() != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        freeRam(requestedRam);  // give RAM back since core failed
                    }
                    else {
                        msg="GRANT";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));
                        pcb = new PCB(pid, getpid(), "notepad", Running, NOTEPAD_RAM);
                        addProcess(pcb);
                    }
                    int status;
                    wait(&status);
                    if(pcb != nullptr){
                        removeProcess(pcb);
                        freeRam(requestedRam);
                        freeCore();
                        delete pcb;
                    }
                }
            }            
            break;
            case 3:{
                int p1fd[2];
                int p2fd[2];
                pipe(p1fd); //pipe1 : parent writes, child reads
                pipe(p2fd); //pipe2 : child writes, parent reads
                pid_t pid=fork();
                if(pid==0){  //child
                    int ram=CLOCK_RAM;
                    close(p1fd[1]);
                    close(p2fd[0]);

                    string message="REQUEST "+to_string(ram);
                    const char *msg=message.c_str();
                    write(p2fd[1],msg,strlen(msg));

                    char buffer[100];
                    int n=read(p1fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    if(strcmp(buffer,"GRANT")==0){
                        execlp("xterm", "xterm", "-e", "/home/mubeen-ahmer/OS/tasks/clock", NULL);    
                        exit(0);
                    }
                    else if(strcmp(buffer,"DENY")==0){
                        exit(1);
                    }
                }
                else{   //parent
                    close(p1fd[0]);
                    close(p2fd[1]);

                    char buffer[100];
                    int n=read(p2fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    string received(buffer);
                    
                    const char*permission;
                    string msg;
                    
                    PCB* pcb = nullptr;
                    int requestedRam=stoi(received.substr(8));
                    if(allocateRam(requestedRam) != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        // no RAM was taken, nothing to free
                    }
                    else if(allocateCore() != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        freeRam(requestedRam);  // give RAM back since core failed
                    }
                    else {
                        msg="GRANT";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));
                        pcb = new PCB(pid, getpid(), "clock", Running, CLOCK_RAM);
                        addProcess(pcb);
                    }
                    int status;
                    wait(&status);
                    if(pcb != nullptr){
                        removeProcess(pcb);
                        freeRam(requestedRam);
                        freeCore();
                        delete pcb;
                    }
                }
            }
            break;
            case 4:{
                int p1fd[2];
                int p2fd[2];
                pipe(p1fd); //pipe1 : parent writes, child reads
                pipe(p2fd); //pipe2 : child writes, parent reads
                pid_t pid=fork();
                if(pid==0){  //child
                    int ram=CALENDAR_RAM;
                    close(p1fd[1]);
                    close(p2fd[0]);

                    string message="REQUEST "+to_string(ram);
                    const char *msg=message.c_str();
                    write(p2fd[1],msg,strlen(msg));

                    char buffer[100];
                    int n=read(p1fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    if(strcmp(buffer,"GRANT")==0){
                        execlp("xterm", "xterm", "-e", "/home/mubeen-ahmer/OS/tasks/calender", NULL);    
                        exit(0);
                    }
                    else if(strcmp(buffer,"DENY")==0){
                        exit(1);
                    }
                }
                else{   //parent
                    close(p1fd[0]);
                    close(p2fd[1]);

                    char buffer[100];
                    int n=read(p2fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    string received(buffer);
                    
                    const char*permission;
                    string msg;
                    
                    PCB* pcb = nullptr;
                    int requestedRam=stoi(received.substr(8));
                    if(allocateRam(requestedRam) != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        // no RAM was taken, nothing to free
                    }
                    else if(allocateCore() != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        freeRam(requestedRam);  // give RAM back since core failed
                    }
                    else {
                        msg="GRANT";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));
                        pcb = new PCB(pid, getpid(), "calender", Running, CALENDAR_RAM);
                        addProcess(pcb);
                    }
                    sleep(4);
                    int status;
                    wait(&status);
                    if(pcb != nullptr){
                        removeProcess(pcb);
                        freeRam(requestedRam);
                        freeCore();
                        delete pcb;
                    }
                }
            }
            break;
            case 5:{
                int p1fd[2];
                int p2fd[2];
                pipe(p1fd); //pipe1 : parent writes, child reads
                pipe(p2fd); //pipe2 : child writes, parent reads
                pid_t pid=fork();
                if(pid==0){  //child
                    int ram=FILE_COPY_RAM;
                    close(p1fd[1]);
                    close(p2fd[0]);

                    string message="REQUEST "+to_string(ram);
                    const char *msg=message.c_str();
                    write(p2fd[1],msg,strlen(msg));

                    char buffer[100];
                    int n=read(p1fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    if(strcmp(buffer,"GRANT")==0){
                        execlp("xterm", "xterm", "-e", "/home/mubeen-ahmer/OS/tasks/fileCopy", NULL);    
                        exit(0);
                    }
                    else if(strcmp(buffer,"DENY")==0){
                        exit(1);
                    }
                }
                else{   //parent
                    close(p1fd[0]);
                    close(p2fd[1]);

                    char buffer[100];
                    int n=read(p2fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    string received(buffer);
                    
                    const char*permission;
                    string msg;
                    
                    PCB* pcb = nullptr;
                    int requestedRam=stoi(received.substr(8));
                    if(allocateRam(requestedRam) != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        // no RAM was taken, nothing to free
                    }
                    else if(allocateCore() != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        freeRam(requestedRam);  // give RAM back since core failed
                    }
                    else {
                        msg="GRANT";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));
                        pcb = new PCB(pid, getpid(), "fileCopy", Running, FILE_COPY_RAM);
                        addProcess(pcb);
                    }
                    sleep(4);
                    int status;
                    wait(&status);
                    if(pcb != nullptr){
                        removeProcess(pcb);
                        freeRam(requestedRam);
                        freeCore();
                        delete pcb;
                    }
                }
            }
            break;
            case 6:{
                int p1fd[2];
                int p2fd[2];
                pipe(p1fd); //pipe1 : parent writes, child reads
                pipe(p2fd); //pipe2 : child writes, parent reads
                pid_t pid=fork();
                if(pid==0){  //child
                    int ram=FILE_MOVE_RAM;
                    close(p1fd[1]);
                    close(p2fd[0]);

                    string message="REQUEST "+to_string(ram);
                    const char *msg=message.c_str();
                    write(p2fd[1],msg,strlen(msg));

                    char buffer[100];
                    int n=read(p1fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    if(strcmp(buffer,"GRANT")==0){
                        execlp("xterm", "xterm", "-e", "/home/mubeen-ahmer/OS/tasks/fileMove", NULL);    
                        exit(0);
                    }
                    else if(strcmp(buffer,"DENY")==0){
                        exit(1);
                    }
                }
                else{   //parent
                    close(p1fd[0]);
                    close(p2fd[1]);

                    char buffer[100];
                    int n=read(p2fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    string received(buffer);
                    
                    const char*permission;
                    string msg;
                    
                    PCB* pcb = nullptr;
                    int requestedRam=stoi(received.substr(8));
                    if(allocateRam(requestedRam) != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        // no RAM was taken, nothing to free
                    }
                    else if(allocateCore() != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        freeRam(requestedRam);  // give RAM back since core failed
                    }
                    else {
                        msg="GRANT";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));
                        pcb = new PCB(pid, getpid(), "fileMove", Running, FILE_MOVE_RAM);
                        addProcess(pcb);
                    }
                    sleep(4);
                    int status;
                    wait(&status);
                    if(pcb != nullptr){
                        removeProcess(pcb);
                        freeRam(requestedRam);
                        freeCore();
                        delete pcb;
                    }
                }
            }  
            break;
            case 7:{
                int p1fd[2];
                int p2fd[2];
                pipe(p1fd); //pipe1 : parent writes, child reads
                pipe(p2fd); //pipe2 : child writes, parent reads
                pid_t pid=fork();
                if(pid==0){  //child
                    int ram=FILE_DELETE_RAM;
                    close(p1fd[1]);
                    close(p2fd[0]);

                    string message="REQUEST "+to_string(ram);
                    const char *msg=message.c_str();
                    write(p2fd[1],msg,strlen(msg));

                    char buffer[100];
                    int n=read(p1fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    if(strcmp(buffer,"GRANT")==0){
                        execlp("xterm", "xterm", "-e", "/home/mubeen-ahmer/OS/tasks/fileDelete", NULL);    
                        exit(0);
                    }
                    else if(strcmp(buffer,"DENY")==0){
                        exit(1);
                    }
                }
                else{   //parent
                    close(p1fd[0]);
                    close(p2fd[1]);

                    char buffer[100];
                    int n=read(p2fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    string received(buffer);
                    
                    const char*permission;
                    string msg;
                    
                    PCB* pcb = nullptr;
                    int requestedRam=stoi(received.substr(8));
                    if(allocateRam(requestedRam) != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        // no RAM was taken, nothing to free
                    }
                    else if(allocateCore() != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        freeRam(requestedRam);  // give RAM back since core failed
                    }
                    else {
                        msg="GRANT";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));
                        pcb = new PCB(pid, getpid(), "fileDelete", Running, FILE_DELETE_RAM);
                        addProcess(pcb);
                    }
                    sleep(4);
                    int status;
                    wait(&status);
                    if(pcb != nullptr){
                        removeProcess(pcb);
                        freeRam(requestedRam);
                        freeCore();
                        delete pcb;
                    }
                }
            }  
            break;
            case 8:{
                int p1fd[2];
                int p2fd[2];
                pipe(p1fd); //pipe1 : parent writes, child reads
                pipe(p2fd); //pipe2 : child writes, parent reads
                pid_t pid=fork();
                if(pid==0){  //child
                    int ram=FILE_CREATE_RAM;
                    close(p1fd[1]);
                    close(p2fd[0]);

                    string message="REQUEST "+to_string(ram);
                    const char *msg=message.c_str();
                    write(p2fd[1],msg,strlen(msg));

                    char buffer[100];
                    int n=read(p1fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    if(strcmp(buffer,"GRANT")==0){
                        execlp("xterm", "xterm", "-e", "/home/mubeen-ahmer/OS/tasks/fileCreate", NULL);    
                        exit(0);
                    }
                    else if(strcmp(buffer,"DENY")==0){
                        exit(1);
                    }
                }
                else{   //parent
                    close(p1fd[0]);
                    close(p2fd[1]);

                    char buffer[100];
                    int n=read(p2fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    string received(buffer);
                    
                    const char*permission;
                    string msg;
                    
                    PCB* pcb = nullptr;
                    int requestedRam=stoi(received.substr(8));
                    if(allocateRam(requestedRam) != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        // no RAM was taken, nothing to free
                    }
                    else if(allocateCore() != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        freeRam(requestedRam);  // give RAM back since core failed
                    }
                    else {
                        msg="GRANT";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));
                        pcb = new PCB(pid, getpid(), "fileCreate", Running, FILE_CREATE_RAM);
                        addProcess(pcb);
                    }
                    sleep(4);
                    int status;
                    wait(&status);
                    if(pcb != nullptr){
                        removeProcess(pcb);
                        freeRam(requestedRam);
                        freeCore();
                        delete pcb;
                    }
                }
            }  
            break;
            case 9:{
                int p1fd[2];
                int p2fd[2];
                pipe(p1fd); //pipe1 : parent writes, child reads
                pipe(p2fd); //pipe2 : child writes, parent reads
                pid_t pid=fork();
                if(pid==0){  //child
                    int ram=FILE_INFO_RAM;
                    close(p1fd[1]);
                    close(p2fd[0]);

                    string message="REQUEST "+to_string(ram);
                    const char *msg=message.c_str();
                    write(p2fd[1],msg,strlen(msg));

                    char buffer[100];
                    int n=read(p1fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    if(strcmp(buffer,"GRANT")==0){
                        execlp("xterm", "xterm", "-e", "/home/mubeen-ahmer/OS/tasks/fileInfo", NULL);    
                        exit(0);
                    }
                    else if(strcmp(buffer,"DENY")==0){
                        exit(1);
                    }
                }
                else{   //parent
                    close(p1fd[0]);
                    close(p2fd[1]);

                    char buffer[100];
                    int n=read(p2fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    string received(buffer);
                    
                    const char*permission;
                    string msg;
                    
                    PCB* pcb = nullptr;
                    int requestedRam=stoi(received.substr(8));
                    if(allocateRam(requestedRam) != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        // no RAM was taken, nothing to free
                    }
                    else if(allocateCore() != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        freeRam(requestedRam);  // give RAM back since core failed
                    }
                    else {
                        msg="GRANT";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));
                        pcb = new PCB(pid, getpid(), "fileInfo", Running, FILE_INFO_RAM);
                        addProcess(pcb);
                    }
                    sleep(4);
                    int status;
                    wait(&status);
                    if(pcb != nullptr){
                        removeProcess(pcb);
                        freeRam(requestedRam);
                        freeCore();
                        delete pcb;
                    }
                }
            }  
            break;
            case 10: break;
            case 11: break;
            case 12: break;
            case 13: {
                int p1fd[2];
                int p2fd[2];
                pipe(p1fd); //pipe1 : parent writes, child reads
                pipe(p2fd); //pipe2 : child writes, parent reads
                pid_t pid=fork();
                if(pid==0){  //child
                    int ram=MUSIC_SIM_RAM;
                    close(p1fd[1]);
                    close(p2fd[0]);

                    string message="REQUEST "+to_string(ram);
                    const char *msg=message.c_str();
                    write(p2fd[1],msg,strlen(msg));

                    char buffer[100];
                    int n=read(p1fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    if(strcmp(buffer,"GRANT")==0){
                        execlp("xterm", "xterm", "-e", "/home/mubeen-ahmer/OS/tasks/musicSim", NULL);    
                        exit(0);
                    }
                    else if(strcmp(buffer,"DENY")==0){
                        exit(1);
                    }
                }
                else{   //parent
                    close(p1fd[0]);
                    close(p2fd[1]);

                    char buffer[100];
                    int n=read(p2fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    string received(buffer);
                    
                    const char*permission;
                    string msg;
                    
                    PCB* pcb = nullptr;
                    int requestedRam=stoi(received.substr(8));
                    if(allocateRam(requestedRam) != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        // no RAM was taken, nothing to free
                    }
                    else if(allocateCore() != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        freeRam(requestedRam);  // give RAM back since core failed
                    }
                    else {
                        msg="GRANT";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));
                        pcb = new PCB(pid, getpid(), "musicSim", Running, MUSIC_SIM_RAM);
                        addProcess(pcb);
                    }
                    sleep(4);
                    int status;
                    wait(&status);
                    if(pcb != nullptr){
                        removeProcess(pcb);
                        freeRam(requestedRam);
                        freeCore();
                        delete pcb;
                    }
                }
            }            
            break;
            case 14:{
                int p1fd[2];
                int p2fd[2];
                pipe(p1fd); //pipe1 : parent writes, child reads
                pipe(p2fd); //pipe2 : child writes, parent reads
                pid_t pid=fork();
                if(pid==0){  //child
                    int ram=PRINT_SIM_RAM;
                    close(p1fd[1]);
                    close(p2fd[0]);

                    string message="REQUEST "+to_string(ram);
                    const char *msg=message.c_str();
                    write(p2fd[1],msg,strlen(msg));

                    char buffer[100];
                    int n=read(p1fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    if(strcmp(buffer,"GRANT")==0){
                        execlp("xterm", "xterm", "-e", "/home/mubeen-ahmer/OS/tasks/printSim", NULL);    
                        exit(0);
                    }
                    else if(strcmp(buffer,"DENY")==0){
                        exit(1);
                    }
                }
                else{   //parent
                    close(p1fd[0]);
                    close(p2fd[1]);

                    char buffer[100];
                    int n=read(p2fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    string received(buffer);
                    
                    const char*permission;
                    string msg;
                    
                    PCB* pcb = nullptr;
                    int requestedRam=stoi(received.substr(8));
                    if(allocateRam(requestedRam) != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        // no RAM was taken, nothing to free
                    }
                    else if(allocateCore() != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        freeRam(requestedRam);  // give RAM back since core failed
                    }
                    else {
                        msg="GRANT";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));
                        pcb = new PCB(pid, getpid(), "printSim", Running, PRINT_SIM_RAM);
                        addProcess(pcb);
                    }
                    sleep(4);
                    int status;
                    wait(&status);
                    if(pcb != nullptr){
                        removeProcess(pcb);
                        freeRam(requestedRam);
                        freeCore();
                        delete pcb;
                    }
                }
            } 
            break;
            case 15:{
                int p1fd[2];
                int p2fd[2];
                pipe(p1fd); //pipe1 : parent writes, child reads
                pipe(p2fd); //pipe2 : child writes, parent reads
                pid_t pid=fork();
                if(pid==0){  //child
                    int ram=RAM_VIEWER_RAM;
                    close(p1fd[1]);
                    close(p2fd[0]);

                    string message="REQUEST "+to_string(ram);
                    const char *msg=message.c_str();
                    write(p2fd[1],msg,strlen(msg));

                    char buffer[100];
                    int n=read(p1fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    if(strcmp(buffer,"GRANT")==0){
                        execlp("xterm", "xterm", "-e", "/home/mubeen-ahmer/OS/tasks/ram_viewer",
                        to_string(totalRam).c_str(),
                        to_string(availableRam).c_str(),
                        to_string(totalHardDisk).c_str(),
                        to_string(availableHardDisk).c_str(),
                        to_string(totalCores).c_str(),
                        to_string(availableCores).c_str(),
                        NULL);
                        exit(0);
                    }
                    else if(strcmp(buffer,"DENY")==0){
                        exit(1);
                    }
                }
                else{   //parent
                    close(p1fd[0]);
                    close(p2fd[1]);

                    char buffer[100];
                    int n=read(p2fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    string received(buffer);
                    
                    const char*permission;
                    string msg;
                    
                    PCB* pcb = nullptr;
                    int requestedRam=stoi(received.substr(8));
                    if(allocateRam(requestedRam) != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        // no RAM was taken, nothing to free
                    }
                    else if(allocateCore() != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        freeRam(requestedRam);  // give RAM back since core failed
                    }
                    else {
                        msg="GRANT";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));
                        pcb = new PCB(pid, getpid(), "ram_viewer", Running, RAM_VIEWER_RAM);
                        addProcess(pcb);
                    }
                    int status;
                    wait(&status);
                    if(pcb != nullptr){
                        removeProcess(pcb);
                        freeRam(requestedRam);
                        freeCore();
                        delete pcb;
                    }
                }
            }
            break;
            case 16: break;
            case 17:{
                int p1fd[2];
                int p2fd[2];
                pipe(p1fd); //pipe1 : parent writes, child reads
                pipe(p2fd); //pipe2 : child writes, parent reads
                pid_t pid=fork();
                if(pid==0){  //child
                    int ram=LOG_GEN_RAM;
                    close(p1fd[1]);
                    close(p2fd[0]);

                    string message="REQUEST "+to_string(ram);
                    const char *msg=message.c_str();
                    write(p2fd[1],msg,strlen(msg));

                    char buffer[100];
                    int n=read(p1fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    if(strcmp(buffer,"GRANT")==0){
                        execlp("xterm", "xterm", "-e", "/home/mubeen-ahmer/OS/tasks/logGenerator", NULL);    
                        exit(0);
                    }
                    else if(strcmp(buffer,"DENY")==0){
                        exit(1);
                    }
                }
                else{   //parent
                    close(p1fd[0]);
                    close(p2fd[1]);

                    char buffer[100];
                    int n=read(p2fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    string received(buffer);
                    
                    const char*permission;
                    string msg;
                    
                    PCB* pcb = nullptr;
                    int requestedRam=stoi(received.substr(8));
                    if(allocateRam(requestedRam) != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        // no RAM was taken, nothing to free
                    }
                    else if(allocateCore() != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        freeRam(requestedRam);  // give RAM back since core failed
                    }
                    else {
                        msg="GRANT";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));
                        pcb = new PCB(pid, getpid(), "logGenerator", Running, LOG_GEN_RAM);
                        addProcess(pcb);
                    }
                    sleep(4);
                    int status;
                    wait(&status);
                    if(pcb != nullptr){
                        removeProcess(pcb);
                        freeRam(requestedRam);
                        freeCore();
                        delete pcb;
                    }
                }
            } 
            break;
            case 18:{
                int p1fd[2];
                int p2fd[2];
                pipe(p1fd); //pipe1 : parent writes, child reads
                pipe(p2fd); //pipe2 : child writes, parent reads
                pid_t pid=fork();
                if(pid==0){  //child
                    int ram=RAND_GEN_RAM;
                    close(p1fd[1]);
                    close(p2fd[0]);

                    string message="REQUEST "+to_string(ram);
                    const char *msg=message.c_str();
                    write(p2fd[1],msg,strlen(msg));

                    char buffer[100];
                    int n=read(p1fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    if(strcmp(buffer,"GRANT")==0){
                        execlp("xterm", "xterm", "-e", "/home/mubeen-ahmer/OS/tasks/randomNumGen", NULL);    
                        exit(0);
                    }
                    else if(strcmp(buffer,"DENY")==0){
                        exit(1);
                    }
                }
                else{   //parent
                    close(p1fd[0]);
                    close(p2fd[1]);

                    char buffer[100];
                    int n=read(p2fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    string received(buffer);
                    
                    const char*permission;
                    string msg;
                    
                    PCB* pcb = nullptr;
                    int requestedRam=stoi(received.substr(8));
                    if(allocateRam(requestedRam) != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        // no RAM was taken, nothing to free
                    }
                    else if(allocateCore() != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        freeRam(requestedRam);  // give RAM back since core failed
                    }
                    else {
                        msg="GRANT";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));
                        pcb = new PCB(pid, getpid(), "randomNumGen", Running, RAND_GEN_RAM);
                        addProcess(pcb);
                    }
                    sleep(4);
                    int status;
                    wait(&status);
                    if(pcb != nullptr){
                        removeProcess(pcb);
                        freeRam(requestedRam);
                        freeCore();
                        delete pcb;
                    }
                }
            }  
            break;
            case 19:{
                int p1fd[2];
                int p2fd[2];
                pipe(p1fd); //pipe1 : parent writes, child reads
                pipe(p2fd); //pipe2 : child writes, parent reads
                pid_t pid=fork();
                if(pid==0){  //child
                    int ram=TIMER_RAM;
                    close(p1fd[1]);
                    close(p2fd[0]);

                    string message="REQUEST "+to_string(ram);
                    const char *msg=message.c_str();
                    write(p2fd[1],msg,strlen(msg));

                    char buffer[100];
                    int n=read(p1fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    if(strcmp(buffer,"GRANT")==0){
                        execlp("xterm", "xterm", "-e", "/home/mubeen-ahmer/OS/tasks/timer", NULL);    
                        exit(0);
                    }
                    else if(strcmp(buffer,"DENY")==0){
                        exit(1);
                    }
                }
                else{   //parent
                    close(p1fd[0]);
                    close(p2fd[1]);

                    char buffer[100];
                    int n=read(p2fd[0],buffer,sizeof(buffer));
                    buffer[n]='\0';

                    string received(buffer);
                    
                    const char*permission;
                    string msg;
                    
                    PCB* pcb = nullptr;
                    int requestedRam=stoi(received.substr(8));
                    if(allocateRam(requestedRam) != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        // no RAM was taken, nothing to free
                    }
                    else if(allocateCore() != 0){
                        msg = "DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));                        
                        printStatus();
                        freeRam(requestedRam);  // give RAM back since core failed
                    }
                    else {
                        msg="GRANT";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));
                        pcb = new PCB(pid, getpid(), "timer", Running, TIMER_RAM);
                        addProcess(pcb);
                    }
                    sleep(4);
                    int status;
                    wait(&status);
                    if(pcb != nullptr){
                        removeProcess(pcb);
                        freeRam(requestedRam);
                        freeCore();
                        delete pcb;
                    }
                }
            } 
            break;
            case 20: break;
            case 0:
                goodbye();
                break;
            default:
                cout << "Invalid choice" << endl;
                sleep(2);
        }
    }while(choice!=0);

    return 0;
}
// a function copied from internet, it move the cursor at x, y cordinate on console
void gotoxy(int x, int y) {
    cout << "\033[" << y << ";" << x << "H";
}
void goodbye(){
    system("clear");
    gotoxy(29, 12);
    cout << "Shutting down" << endl;
    sleep(1);
    system("clear");
}
void welcomeScreen(){
    system("clear");
    gotoxy(29, 12);
    cout << "Welcome to the Windows" << endl;
    sleep(1);
    system("clear");
}