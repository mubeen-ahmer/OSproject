#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand(time(0));

    char choice;
    cout << "=== Coin Flip ===" << endl;

    do {
        int flip = rand() % 2;
        cout << "\nResult: " << (flip == 0 ? "HEADS" : "TAILS") << endl;
        cout << "Flip again? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    cout << "Goodbye!" << endl;
    return 0;
}