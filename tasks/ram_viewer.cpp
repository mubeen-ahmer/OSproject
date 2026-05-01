#include <iostream>
#include <unistd.h>
using namespace std;
int main(int argc, char* argv[]){
    system("clear");
    cout << "===== RAM STATUS =====" << endl;
    cout << "Total RAM:       " << argv[1] << " MB" << endl;
    cout << "Available RAM:   " << argv[2] << " MB" << endl;
    cout << "Total HDD:       " << argv[3] << " MB" << endl;
    cout << "Available HDD:   " << argv[4] << " MB" << endl;
    cout << "Total Cores:     " << argv[5] << endl;
    cout << "Available Cores: " << argv[6] << endl;
    cout << "\nPress Enter to close...";
    cin.get();
    return 0;
}
