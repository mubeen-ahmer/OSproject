#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>

using namespace std;
int main(){
    string source, dest;
    cout << "All the files:" << endl;
    system("ls tasks/notepadsavedfiles/");
    cout << "\nEnter the name of the file you want to copy : "; cin >> source;
    cout << "Enter the name of the new file : "; cin >> dest;
    
    source = "tasks/notepadsavedfiles/" + source + ".txt";
    dest   = "tasks/notepadsavedfiles/" + dest   + ".txt";
    
    ifstream in(source.c_str());
    if(!in){
        cout << "File not found!" << endl;
        sleep(1);
        return 1;
    }
    
    ofstream out(dest.c_str());
    char ch;
    while(in.get(ch)){
        out << ch;
    }
    in.close();
    out.close();
    cout << "File copied successfully!" << endl;
    cin.ignore();
    cin.get();
    return 0;
}