#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
using namespace std;

int main(){
    string logFile = "tasks/notepadsavedfiles/system.log";

    int choice;
    do{
        system("clear");
        cout << "== Log Viewer ==" << endl;
        cout << "1. View all logs"    << endl;
        cout << "2. Clear all logs"   << endl;
        cout << "3. Search in logs"   << endl;
        cout << "0. Exit"             << endl;
        cout << "Select : "; cin >> choice;

        switch(choice){
            case 1:{
                system("clear");
                cout << "== All Logs ==" << endl;
                cout << "------------------------" << endl;

                ifstream file(logFile.c_str());
                if(!file){
                    cout << "No logs found!" << endl;
                    cin.ignore();
                    cin.get();
                    break;
                }

                string line;
                int count = 1;
                while(getline(file, line)){
                    cout << count++ << ". " << line << endl;
                }
                file.close();

                cout << "------------------------" << endl;
                cout << "Press any key to go back...";
                cin.ignore();
                cin.get();
            }
            break;

            case 2:{
                char ch;
                cout << "Are you sure you want to clear all logs? [y/n] : "; 
                cin >> ch;
                if(ch == 'y' || ch == 'Y'){
                    ofstream file(logFile.c_str());  // open without append clears it
                    file.close();
                    cout << "Logs cleared!" << endl;
                } else {
                    cout << "Cancelled!" << endl;
                }
                sleep(1);
            }
            break;

            case 3:{
                string keyword;
                cout << "Enter keyword to search : "; 
                cin >> keyword;
                cin.ignore();

                ifstream file(logFile.c_str());
                if(!file){
                    cout << "No logs found!" << endl;
                    cin.get();
                    break;
                }

                system("clear");
                cout << "== Search Results for: " << keyword << " ==" << endl;
                cout << "------------------------" << endl;

                string line;
                int count  = 0;
                int lineNo = 1;
                while(getline(file, line)){
                    if(line.find(keyword) != string::npos){
                        cout << lineNo << ". " << line << endl;
                        count++;
                    }
                    lineNo++;
                }
                file.close();

                if(count == 0)
                    cout << "No results found!" << endl;
                else
                    cout << "------------------------" << endl
                         << count << " result(s) found!" << endl;

                cout << "Press any key to go back...";
                cin.get();
            }
            break;

            case 0:
                cout << "Closing Log Viewer..." << endl;
                sleep(1);
            break;

            default:
                cout << "Invalid choice!" << endl;
                sleep(1);
        }
    }while(choice != 0);

    return 0;
}