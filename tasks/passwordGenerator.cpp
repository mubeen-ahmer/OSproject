#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <unistd.h>
using namespace std;

int main(){
    srand(time(0));

    string lowercase = "abcdefghijklmnopqrstuvwxyz";
    string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string numbers   = "0123456789";
    string symbols   = "!@#$%^&*()_+-=[]{}|;:,.<>?";

    char ch;
    do{
        system("clear");
        cout << "== Password Generator ==" << endl;

        int length;
        cout << "Enter password length : "; cin >> length;
        cin.ignore();

        if(length < 4){
            cout << "Length must be at least 4!" << endl;
            sleep(1);
            continue;
        }

        // ask what to include
        char useLower, useUpper, useNums, useSymbols;
        cout << "Include lowercase? [y/n] : "; cin >> useLower;
        cout << "Include uppercase? [y/n] : "; cin >> useUpper;
        cout << "Include numbers?   [y/n] : "; cin >> useNums;
        cout << "Include symbols?   [y/n] : "; cin >> useSymbols;
        cin.ignore();

        // build character pool
        string pool = "";
        if(useLower   == 'y' || useLower   == 'Y') pool += lowercase;
        if(useUpper   == 'y' || useUpper   == 'Y') pool += uppercase;
        if(useNums    == 'y' || useNums    == 'Y') pool += numbers;
        if(useSymbols == 'y' || useSymbols == 'Y') pool += symbols;

        if(pool.empty()){
            cout << "You must select at least one option!" << endl;
            sleep(1);
            continue;
        }

        // generate password
        string password = "";
        for(int i = 0; i < length; i++){
            password += pool[rand() % pool.size()];
        }

        cout << endl;
        cout << "------------------------"   << endl;
        cout << "Generated: " << password    << endl;
        cout << "------------------------"   << endl;

        cout << "Generate another? [y/n] : "; cin >> ch;

    }while(ch == 'y' || ch == 'Y');

    sleep(1);
    return 0;
}