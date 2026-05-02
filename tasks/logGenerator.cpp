#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <unistd.h>
using namespace std;

int main(){
    string log = "tasks/notepadsavedfiles/system.log";
    char ch;
    string message;

    do{
        cout << "--Log Generator--" << endl;
        cout << "Enter log message : "; 
        cin.ignore();
        getline(cin, message);

        time_t now = time(0);
        char* dt = ctime(&now);
        dt[strlen(dt)-1] = '\0';  // remove \n from ctime
        ofstream file(log.c_str(), ios::app);
        if(!file){
            cout << "Failed to open log file!" << endl;
            break;
        }
        file << "[" << dt << "] " << message << endl;
        file.close();
        cout << "Log saved!" << endl;
        cout << "Add another log [y/n] : "; cin >> ch;
        system("clear");

    }while(ch == 'y' || ch == 'Y');

    sleep(1);
    return 0;
}