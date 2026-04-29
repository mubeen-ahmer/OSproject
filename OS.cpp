#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/wait.h>
#include "config.h"
#include <cstring>
#include "resource.h"
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
        cout << "1.  Calculator      [RAM: " << CALC_RAM        << " MB]" << endl;
        cout << "2.  Notepad         [RAM: " << NOTEPAD_RAM     << " MB]" << endl;
        cout << "3.  Clock           [RAM: " << CLOCK_RAM       << " MB]" << endl;
        cout << "4.  Calendar        [RAM: " << CALENDAR_RAM    << " MB]" << endl;
        cout << "5.  File Copy       [RAM: " << FILE_COPY_RAM   << " MB]" << endl;
        cout << "6.  File Move       [RAM: " << FILE_MOVE_RAM   << " MB]" << endl;
        cout << "7.  File Delete     [RAM: " << FILE_DELETE_RAM << " MB]" << endl;
        cout << "8.  File Create     [RAM: " << FILE_CREATE_RAM << " MB]" << endl;
        cout << "9.  File Info       [RAM: " << FILE_INFO_RAM   << " MB]" << endl;
        cout << "10. File Editor     [RAM: " << FILE_EDITOR_RAM << " MB]" << endl;
        cout << "11. Text Search     [RAM: " << TEXT_SEARCH_RAM << " MB]" << endl;
        cout << "12. Minesweeper     [RAM: " << MINESWEEPER_RAM << " MB]" << endl;
        cout << "13. Music Sim       [RAM: " << MUSIC_SIM_RAM   << " MB]" << endl;
        cout << "14. Print Sim       [RAM: " << PRINT_SIM_RAM   << " MB]" << endl;
        cout << "15. RAM Viewer      [RAM: " << RAM_VIEWER_RAM  << " MB]" << endl;
        cout << "16. Process Viewer  [RAM: " << PROC_VIEWER_RAM << " MB]" << endl;
        cout << "17. Log Generator   [RAM: " << LOG_GEN_RAM     << " MB]" << endl;
        cout << "18. Random Number   [RAM: " << RAND_GEN_RAM    << " MB]" << endl;
        cout << "19. Timer/Alarm     [RAM: " << TIMER_RAM       << " MB]" << endl;
        cout << "20. Auto Backup     [RAM: " << AUTO_BACKUP_RAM << " MB]" << endl;
        cout << "0.  Shutdown" << endl;
        cout << "\nSelect: ";
        cin >> choice;

        switch(choice){
            case 1:
            {
                cout<<"ENTERING"<<endl;
                int p1fd[2];
                int p2fd[2];
                pipe(p1fd); //pipe1 : parent writes, child reads
                pipe(p2fd); //pipe2 : child writes, parent reads
                pid_t pid=fork();
                if(pid==0){  //child
                     cout<<"childstart"<<endl;
                    int ram=CALC_RAM;
                    
                    close(p1fd[1]);
                    close(p2fd[0]);
                    string message="REQUEST "+to_string(ram);
                    const char *msg=message.c_str();
                    write(p2fd[1],msg,strlen(msg));
                    // close(p2fd[1]);
                    cout<<"child wrote"<<endl;
                    char buffer[100];
                    int n=read(p1fd[0],buffer,sizeof(buffer));
                    // close(p1fd[0]);
                    cout<<"child read"<<endl;

                    buffer[n]='\0';
                    if(strcmp(buffer,"GRANT")==0){
                        cout << "launching xterm" << endl;
                        sleep(4);
                        execlp("xterm", "xterm", "-e", "/home/mubeen-ahmer/OS/calculator", NULL);
                        // cout<<"Eror";
                        
                        exit(0);
                    }
                    else if(strcmp(buffer,"DENY")==0){
                        cout<<"deny"<<endl;   
                        exit(1);
                    }
                    // cout<<"childend"<<endl;
                    // exit(1);
                    //  sleep(7);
                }
                else{   //parent
                    cout<<"parentstart"<<endl;
                    close(p1fd[0]);
                    close(p2fd[1]);
                    char buffer[100];
                    int n=read(p2fd[0],buffer,sizeof(buffer));
                    // close(p2fd[0]);
                    cout<<"parent read"<<endl;

                    buffer[n]='\0';
                    string received(buffer);
                    int requestedRam=stoi(received.substr(8));
                    const char*permission;
                    string msg;
                    if(allocateRam(requestedRam)!=0 || allocateCore() !=0){
                        msg="DENY";
                        permission=msg.c_str();
                        write(p1fd[1],permission,strlen(permission));
                        // close(p1fd[0]);
                        cout<<"parent wrote deny"<<endl;
                        
                        printStatus();
                        
                    } 
                    else {
                    msg="GRANT";
                    permission=msg.c_str();
                    
                    write(p1fd[1],permission,strlen(permission));
                    // close(p1fd[0]);
                    cout<<"parent wrote grant"<<endl;
                    
                    }
                    int status;
                    wait(&status);
                    cout<<status<<endl;
                    // cin.ignore();
                    // cin.get();
                    // cout<<"exting"<<endl;
                    // sleep(5);
                }
            }
            break;
            case 2:  break;
            case 3:  break;
            case 4:  break;
            case 5:  break;
            case 6:  break;
            case 7:  break;
            case 8:  break;
            case 9:  break;
            case 10: break;
            case 11: break;
            case 12: break;
            case 13: break;
            case 14: break;
            case 15: break;
            case 16: break;
            case 17: break;
            case 18: break;
            case 19: break;
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