#include <iostream>
#include <string>
#include <sys/stat.h>
#include <ctime>
#include <unistd.h>
using namespace std;

int main(){
    string name;
    cout << "--file info--" << endl;
    cout << "All files:" << endl;
    system("ls tasks/notepadsavedfiles/");
    cout << "Enter file name : "; cin >> name;
    cin.ignore();
    name = "tasks/notepadsavedfiles/" + name + ".txt";
    struct stat info;
    if(stat(name.c_str(), &info) != 0){
        cout << "File not found!" << endl;
        cin.get();
        return 1;
    }


    cout <<endl<< "--File Information--"<< endl;
    cout << "Name  : " << name<< endl;
    cout << "Size  : " << info.st_size << " bytes" << endl;
    cout << "Last Modified : " << ctime(&info.st_mtime);

    cin.get();
    sleep(1);
    return 0;
}

/*
 * ============================================================
 *  HOW stat() WORKS
 * ============================================================
 *
 *  stat() is a system call that retrieves metadata about a
 *  file without opening or reading its content.
 *
 *  USAGE:
 *      struct stat info;
 *      stat("filename.txt", &info);
 *
 *      - First argument  → file path
 *      - Second argument → address of stat struct to fill
 *      - Returns 0 if success, -1 if file not found
 *
 *  USEFUL FIELDS IN struct stat:
 *
 *      info.st_size    → file size in bytes
 *      info.st_mtime   → last modified time
 *      info.st_atime   → last accessed time
 *      info.st_ctime   → last status change time
 *      info.st_mode    → file type and permissions
 *      info.st_uid     → user ID of owner
 *      info.st_gid     → group ID of owner
 *
 *  HOW TIMES WORK:
 *      times are stored as time_t
 *      use ctime() to convert to readable string:
 *      cout << ctime(&info.st_mtime);
 *
 *  HEADER REQUIRED:
 *      #include <sys/stat.h>
 *
 *  SIMPLE ANALOGY:
 *      ifstream → reads INSIDE the file
 *      stat()   → reads ABOUT the file without opening it
 *
 * ============================================================
 */