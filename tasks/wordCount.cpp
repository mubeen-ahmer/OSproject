#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
using namespace std;

int main(){
    string name;
    system("clear");
    cout << "== Word Count ==" << endl;
    cout << "All files:" << endl;
    system("ls tasks/notepadsavedfiles/");
    cout << "Enter file name : "; cin >> name;
    cin.ignore();

    name = "tasks/notepadsavedfiles/" + name + ".txt";

    ifstream file(name.c_str());
    if(!file){
        cout << "File not found!" << endl;
        cin.get();
        return 1;
    }

    int words    = 0;
    int lines    = 0;
    int chars    = 0;
    bool inWord  = false;
    char ch;

    while(file.get(ch)){
        chars++;
        if(ch == '\n') lines++;

        if(ch == ' ' || ch == '\n' || ch == '\t'){
            if(inWord){
                words++;
                inWord = false;
            }
        } else {
            inWord = true;
        }
    }

    // count last word if file doesn't end with space
    if(inWord) words++;

    file.close();

    cout << endl;
    cout << "------------------------" << endl;
    cout << "File    : " << name   << endl;
    cout << "Lines   : " << lines  << endl;
    cout << "Words   : " << words  << endl;
    cout << "Chars   : " << chars  << endl;
    cout << "------------------------" << endl;

    cout << "Press any key to exit...";
    cin.get();
    sleep(1);
    return 0;
}