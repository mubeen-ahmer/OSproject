#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <pthread.h>
using namespace std;

string filePath = "";
bool printing   = false;

void* printThread(void* arg){
    ifstream file(filePath.c_str());
    if(!file){
        cout << "File not found!" << endl;
        printing = false;
        return nullptr;
    }

    cout << "\n-- Printing --" << endl;
    char ch;
    while(file.get(ch) && printing){
        cout << ch;
        fflush(stdout);
        usleep(100000);  // 0.1 second delay per character
    }
    file.close();

    cout << "\nPrint complete!" << endl;
    printing = false;
    return nullptr;
}

int main(){
    string name;
    cout << "-- Print Simulator --" << endl;
    cout << "All files:" << endl;
    system("ls tasks/notepadsavedfiles/");
    cout << "Enter file name : "; cin >> name;
    cin.ignore();

    filePath = "tasks/notepadsavedfiles/" + name + ".txt";

    printing = true;
    pthread_t tid;
    pthread_create(&tid, nullptr, printThread, nullptr);

    cout << "Sending to printer..." << endl;
    cout << "Press any key to cancel..." << endl;
    cin.get();

    if(printing){
        printing = false;
        cout << "Print cancelled!" << endl;
    }

    pthread_join(tid, nullptr);
    sleep(1);
    return 0;
}

/*
 * ============================================================
 *  HOW PRINT SIM WORKS
 * ============================================================
 *
 *  Simulates printing a file by reading and displaying it
 *  character by character with a small delay using a thread.
 *
 *  FLOW:
 *      1. User selects a file
 *      2. A thread starts and reads file character by character
 *      3. Each character is printed with 0.1 second delay
 *      4. Main thread waits for user to press a key to cancel
 *      5. If user cancels → printing = false → thread exits
 *      6. If file finishes → thread exits on its own
 *
 *  KEY CONCEPTS USED:
 *      pthread    → background thread for printing
 *      printing   → shared bool to cancel from main
 *      usleep()   → microsecond delay between characters
 *      fflush()   → forces each character to print immediately
 *
 * ============================================================
 */