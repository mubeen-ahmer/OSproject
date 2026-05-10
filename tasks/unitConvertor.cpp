#include <iostream>
using namespace std;

int main() {
    int choice;
    double value, result;

    cout << "=== Unit Converter ===" << endl;

    do {
        cout << "\n1. KB to MB"
             << "\n2. MB to GB"
             << "\n3. KB to GB"
             << "\n4. Miles to KM"
             << "\n5. KG to Pounds"
             << "\n6. Celsius to Fahrenheit"
             << "\n0. Exit"
             << "\nChoice: ";
        cin >> choice;

        if (choice == 0) break;

        cout << "Enter value: ";
        cin >> value;

        switch (choice) {
            case 1: result = value / 1024;          cout << value << " KB = " << result << " MB" << endl; break;
            case 2: result = value / 1024;          cout << value << " MB = " << result << " GB" << endl; break;
            case 3: result = value / (1024 * 1024); cout << value << " KB = " << result << " GB" << endl; break;
            case 4: result = value * 1.60934;       cout << value << " Miles = " << result << " KM" << endl; break;
            case 5: result = value * 2.20462;       cout << value << " KG = " << result << " Pounds" << endl; break;
            case 6: result = (value * 9/5) + 32;   cout << value << " C = " << result << " F" << endl; break;
            default: cout << "Invalid choice!" << endl;
        }
    } while (true);

    cout << "Goodbye!" << endl;
    return 0;
}