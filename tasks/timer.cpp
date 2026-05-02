#include <iostream>
#include <ctime>
#include <unistd.h>
using namespace std;

int main(){
    int hours, mins, secs;
    cout << "--Timer--" << endl;
    cout << "Enter hours : ";   cin >> hours;
    cout << "Enter minutes : "; cin >> mins;
    cout << "Enter seconds : "; cin >> secs;

    int totalSeconds = hours*3600 + mins*60 + secs;
    cout << "Timer set counting down..." << endl;

    while(totalSeconds > 0){
        int h = totalSeconds / 3600;
        int m = (totalSeconds % 3600) / 60;
        int s = totalSeconds % 60;
        printf("\r%02d:%02d:%02d", h, m, s);
        fflush(stdout);
        sleep(1);
        totalSeconds--;
    }

    cout << "\n***TIME IS UP***" << endl;
    
    cout << "\a\a\a";
    
    cin.ignore();
    cin.get();
    return 0;
}