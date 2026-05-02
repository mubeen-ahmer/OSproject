#include <iostream>
#include <ctime>
using namespace std;

int main(){
    time_t now = time(0);
    tm* ltm = localtime(&now);
    int day   = ltm->tm_mday;
    int month = ltm->tm_mon + 1;
    int year  = ltm->tm_year + 1900;
    int wday  = ltm->tm_wday;

    int totalDays;
    if(month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12){
        totalDays = 31;
    }
    else if(month==4 || month==6 || month==9 || month==11){
        totalDays = 30;
    }
    else{
        if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) totalDays = 29;
        else totalDays = 28;
    }

    int firstDay = (wday - (day - 1) % 7 + 7) % 7;

    string months[] = {"January","February","March","April","May","June",
                       "July","August","September","October","November","December"};

    cout << "     " << months[month-1] << " " << year << endl;
    cout << "Sun Mon Tue Wed Thu Fri Sat" << endl;

    for(int i = 0; i < firstDay; i++){
        cout << "    ";
    }

    int col = firstDay;
    for(int i = 1; i <= totalDays; i++){
        if(i == day)
            printf("[%d]", i);  
        else
            printf(" %2d ", i);

        col++;
        if(col == 7){
            cout << endl;
            col = 0;
        }
    }
    cout << endl;
    cin.ignore();
    cin.get();
    cin.ignore();

    return 0;
}