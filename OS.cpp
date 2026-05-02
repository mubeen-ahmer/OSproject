#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/wait.h>
#include <pthread.h>
#include "config.h"
#include "kernel.h"
#include "resource.h"
#include "process.h"
using namespace std;
void gotoxy(int x, int y);
void welcomeScreen();
void goodbye();


volatile bool osRunning = true;

void* reaperThread(void* arg) {
    while (osRunning) {
        pthread_mutex_lock(&gMutex);
        checkAndCleanProcesses();
        pthread_mutex_unlock(&gMutex);
        sleep(1);
    }
    return nullptr;
}

void launchTask(const char* name, const char* execPath, int ramAmount) {
    int p1fd[2];// pipe1 : parent writes, child reads
    int p2fd[2];// pipe2 : child writes, parent reads

    if (pipe(p1fd) == -1 || pipe(p2fd) == -1) {
        perror("pipe");
        return;
    }
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        close(p1fd[0]); close(p1fd[1]);
        close(p2fd[0]); close(p2fd[1]);
        return;
    }
    if (pid == 0) { // child
        close(p1fd[1]);
        close(p2fd[0]);

        string message = "REQUEST " + to_string(ramAmount);
        const char* msg = message.c_str();
        write(p2fd[1], msg, strlen(msg));
        close(p2fd[1]); 

        char buffer[100];
        int n = read(p1fd[0], buffer, sizeof(buffer));
        buffer[n] = '\0';
        close(p1fd[0]); 

        if (strcmp(buffer, "GRANT") == 0) {
            execlp("xterm", "xterm", "-fa", "Monospace", "-fs", "11", "-e", execPath, NULL);
            perror("execlp: xterm failed"); // only reached if execlp fails
        }
        exit(1);
    }
    else { // parent
        close(p1fd[0]);
        close(p2fd[1]);

        char buffer[100];
        int n = read(p2fd[0], buffer, sizeof(buffer));
        buffer[n] = '\0';
        close(p2fd[0]); 

        string received(buffer);

        const char* permission;
        string msg;

        PCB* pcb = nullptr;
        int requestedRam = stoi(received.substr(8));

        pthread_mutex_lock(&gMutex); // FIX: lock before touching resources + PCB

        if (allocateRam(requestedRam) != 0) {
            msg = "DENY";
            permission = msg.c_str();
            write(p1fd[1], permission, strlen(permission));
            close(p1fd[1]); 
            printStatus();
            cin.ignore();
            cin.get();
        }
        else if (allocateCore() != 0) {
            msg = "DENY";
            permission = msg.c_str();
            write(p1fd[1], permission, strlen(permission));
            close(p1fd[1]); 
            printStatus();
            freeRam(requestedRam);
            cin.ignore();
            cin.get();
        }
        else {
            msg = "GRANT";
            permission = msg.c_str();
            write(p1fd[1], permission, strlen(permission));
            close(p1fd[1]); 
            pcb = new PCB(pid, getpid(), name, Running, ramAmount);
            addProcess(pcb);
        }

        pthread_mutex_unlock(&gMutex); // FIX: unlock after done
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "Usage: ./OS <RAM_MB> <HDD_GB> <Cores>" << endl;
        return 1;
    }
    int ram  = atoi(argv[1]);
    int hdd  = atoi(argv[2]);
    int core = atoi(argv[3]);
    if (initialize(ram, hdd, core) != 0) {
        cout << "Invalid arguments — values must be positive" << endl;
        return 1;
    }

    welcomeScreen();
    pthread_t reaperTid;
    pthread_create(&reaperTid, nullptr, reaperThread, nullptr);
    pthread_detach(reaperTid);   

    int choice;
    do {
        system("clear");
        printStatus();
        cout << "\n===== Windows =====" << endl;
        cout << "1.  Calculator         [RAM: " << CALC_RAM           << " MB]" << endl;
        cout << "2.  Notepad            [RAM: " << NOTEPAD_RAM        << " MB]" << endl;
        cout << "3.  Clock              [RAM: " << CLOCK_RAM          << " MB]" << endl;
        cout << "4.  Calendar           [RAM: " << CALENDAR_RAM       << " MB]" << endl;
        cout << "5.  File Copy          [RAM: " << FILE_COPY_RAM      << " MB]" << endl;
        cout << "6.  File Move          [RAM: " << FILE_MOVE_RAM      << " MB]" << endl;
        cout << "7.  File Delete        [RAM: " << FILE_DELETE_RAM    << " MB]" << endl;
        cout << "8.  File Create        [RAM: " << FILE_CREATE_RAM    << " MB]" << endl;
        cout << "9.  File Info          [RAM: " << FILE_INFO_RAM      << " MB]" << endl;
        cout << "10. Random Number      [RAM: " << RAND_GEN_RAM       << " MB]" << endl;
        cout << "11. Word Count         [RAM: " << WORD_COUNT_RAM     << " MB]" << endl;
        cout << "12. Mine Sweeper       [RAM: " << MINESWEEPER_RAM    << " MB]" << endl;
        cout << "13. Music Sim          [RAM: " << MUSIC_SIM_RAM      << " MB]" << endl;
        cout << "14. Print Sim          [RAM: " << PRINT_SIM_RAM      << " MB]" << endl;
        cout << "15. RAM Viewer         [RAM: " << RAM_VIEWER_RAM     << " MB]" << endl;
        cout << "16. Process Viewer     [RAM: " << PROC_VIEWER_RAM    << " MB]" << endl;
        cout << "17. Log Generator      [RAM: " << LOG_GEN_RAM        << " MB]" << endl;
        cout << "18. Log Viewer         [RAM: " << LOG_VIEWER_RAM     << " MB]" << endl;
        cout << "19. Timer/Alarm        [RAM: " << TIMER_RAM          << " MB]" << endl;
        cout << "20. Password Generator [RAM: " << PASSWORD_GENERATOR_RAM << " MB]" << endl;
        cout << "0.  Shutdown" << endl;
        cout << "\nSelect: ";
        cin >> choice;

        switch (choice) {
            case  1: launchTask("calculator", "tasks/calculator", CALC_RAM);    break;
            case  2: launchTask("notepad", "tasks/notepad", NOTEPAD_RAM);    break;
            case  3: launchTask("clock", "tasks/clock", CLOCK_RAM);    break;
            case  4: launchTask("calender", "tasks/calender", CALENDAR_RAM);    break;
            case  5: launchTask("fileCopy", "tasks/fileCopy", FILE_COPY_RAM);    break;
            case  6: launchTask("fileMove", "tasks/fileMove", FILE_MOVE_RAM);    break;
            case  7: launchTask("fileDelete", "tasks/fileDelete", FILE_DELETE_RAM);    break;
            case  8: launchTask("fileCreate", "tasks/fileCreate", FILE_CREATE_RAM);    break;
            case  9: launchTask("fileInfo", "tasks/fileInfo", FILE_INFO_RAM);    break;
            case 10: launchTask("randomNumGen", "tasks/randomNumGen", RAND_GEN_RAM);    break;
            case 11: launchTask("wordCount", "tasks/wordCount", WORD_COUNT_RAM);    break;
            case 12: launchTask("minesweeper", "tasks/minesweeper", MINESWEEPER_RAM);    break;
            case 13: launchTask("musicSim", "tasks/musicSim", MUSIC_SIM_RAM);    break;
            case 14: launchTask("printSim", "tasks/printSim", PRINT_SIM_RAM);    break;
            case 15: launchTask("ramViewer", "tasks/ramViewer", RAM_VIEWER_RAM);    break;
            case 16: launchTask("procViewer", "tasks/procViewer", PROC_VIEWER_RAM);    break;
            case 17: launchTask("logGenerator", "tasks/logGenerator", LOG_GEN_RAM);    break;
            case 18: launchTask("logViewer", "tasks/logViewer", LOG_VIEWER_RAM);    break;
            case 19: launchTask("timer", "tasks/timer", TIMER_RAM);    break;
            case 20: launchTask("passwordGenerator","tasks/passwordGenerator", PASSWORD_GENERATOR_RAM); break;
            case  0: goodbye(); break;
            default:
                cout << "Invalid choice" << endl;
                sleep(1);
        }

    } while (choice != 0);

    osRunning = false;
    sleep(2);

    return 0;
}

// a function copied from internet, it move the cursor at x, y cordinate on console
void gotoxy(int x, int y) {
    cout << "\033[" << y << ";" << x << "H";
}

void welcomeScreen() {
    system("clear");
    gotoxy(29, 12);
    cout << "Welcome to the Windows" << endl;
    sleep(1);
    system("clear");
}

void goodbye() {
    system("clear");
    gotoxy(29, 12);
    cout << "Shutting down..." << endl;
    sleep(1);
    system("clear");
}