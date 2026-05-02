#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>

using namespace std;
int main(){
    char ch;
    string name;
    cout << "All the files of your system" << endl;
    system("ls tasks/notepadsavedfiles/");
    cout << "\nEnter the name of the file you want to delete : "; cin >> name;
    cin.ignore();  
    cout << "Are you sure you want to delete this file [y/n] : "; ch = cin.get();
    if(ch=='y' || ch=='Y'){  
        name = "tasks/notepadsavedfiles/" + name + ".txt";
        if(remove(name.c_str()) == 0)  {
            cout << "File deleted successfully!" << endl;
            cin.ignore();
            cin.get();
        } else{
            cout << "Failed to delete file!" << endl;
            cin.ignore();
            cin.get();
        }
    }
    return 0;
}