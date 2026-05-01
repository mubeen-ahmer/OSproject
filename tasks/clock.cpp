#include <iostream>
#include <unistd.h>
using namespace std;
#include <ctime>

// a function copied from internet, it move the cursor at x, y cordinate on console
void gotoxy(int x, int y) {
    cout << "\033[" << y << ";" << x << "H";
}
int main(){
    while(true){
        time_t now = time(0);  // update every iteration
        char* dt = ctime(&now);
        gotoxy(10, 20);
        cout << "Current time: " << dt;
        sleep(1);  // wait 1 second
    }
    return 0;
}