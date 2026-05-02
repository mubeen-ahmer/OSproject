#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
using namespace std;

int main(){
    srand(time(0));

    char ch;
    do{
        int min, max;
        cout << "--Random Number Generator--" << endl;
        cout << "Enter minimum : "; cin >> min;
        cout << "Enter maximum : "; cin >> max;
        if(min > max){
            cout << "Minimum cannot be greater than maximum!" << endl;
        } else {
            int result = rand() % (max - min + 1) + min;
            cout << "Generated : " << result << endl;
        }

        cout << "Generate another? [y/n] : "; 
        cin >> ch;
        system("clear");

    }while(ch == 'y' || ch == 'Y');

    sleep(1);
    return 0;
}