#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>

using namespace std;

int main(){
    string name;
    cout << "-- File Creation --" << endl;
    cout << "Enter the name of the file : "; cin >> name;
    cin.ignore();
    name = "tasks/notepadsavedfiles/" + name + ".txt";
    ofstream file(name.c_str());
    if(!file){
        cout << "Failed to create file!" << endl;
        cin.get();
        sleep(1);
        return 1;
    }
    cout << "File created successfully!" << endl;
    sleep(1);
    return 0;
}