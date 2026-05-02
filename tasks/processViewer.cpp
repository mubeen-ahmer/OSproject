#include <iostream>
#include <unistd.h>
#include "../kernel.h"
using namespace std;

int main(){
    system("clear");
    cout << "--Process Viewer--" << endl;
    
    listProcess();
    
    cout <<endl<< "Press any key to exit...";
    cin.get();
    return 0;
}