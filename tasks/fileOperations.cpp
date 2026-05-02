#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>

using namespace std;
int main(){
    int choice;
    do
    {
        system("clear");
        cout<<"1. delete a file"<<endl
            <<"2. Copy/Past a file"<<endl
            <<"3. Cut/paste a file"<<endl
            <<"0. Exit"<<endl;
        cout<<endl<<"Select : ";cin>>choice;
        cin.ignore();
        switch(choice){
            case 1:{
                char ch;
                string name;
                cout << "All the files of your system" << endl;
                system("ls tasks/notepadsavedfiles/");
                cout << "\nEnter the name of the file you want to delete : "; cin >> name;
                cin.ignore();  
                cout << "Are you sure you want to delete this file [y/n] : "; ch = cin.get();
                if(ch=='y' || ch=='Y'){  
                    name = "tasks/notepadsavedfiles/" + name + ".txt";
                    if(remove(name.c_str()) == 0)  {
                        cout << "File deleted successfully!" << endl;
                        cin.ignore();
                        cin.get();
                    } else{
                        cout << "Failed to delete file!" << endl;
                        cin.ignore();
                        cin.get();
                    }
                }
            }
            break;
            case 2:{
                string source, dest;
                cout << "All the files:" << endl;
                system("ls tasks/notepadsavedfiles/");
                cout << "\nEnter the name of the file you want to copy : "; cin >> source;
                cout << "Enter the name of the new file : "; cin >> dest;
                
                source = "tasks/notepadsavedfiles/" + source + ".txt";
                dest   = "tasks/notepadsavedfiles/" + dest   + ".txt";
                
                ifstream in(source.c_str());
                if(!in){
                    cout << "File not found!" << endl;
                    sleep(1);
                    break;
                }
                
                ofstream out(dest.c_str());
                char ch;
                while(in.get(ch)){
                    out << ch;
                }
                in.close();
                out.close();
                cout << "File copied successfully!" << endl;
                cin.ignore();
                cin.get();
            }
            break;

            case 3:{
                string source, dest;
                cout << "All the files:" << endl;
                system("ls tasks/notepadsavedfiles/");
                cout << "\nEnter the name of the file you want to move : "; cin >> source;
                cout << "Enter the name of the new file : "; cin >> dest;
                
                source = "tasks/notepadsavedfiles/" + source + ".txt";
                dest   = "tasks/notepadsavedfiles/" + dest   + ".txt";
                
                ifstream in(source.c_str());
                if(!in){
                    cout << "File not found!" << endl;
                    sleep(1);
                    break;
                }
                
                ofstream out(dest.c_str());
                char ch;
                while(in.get(ch)){
                    out << ch;
                }
                in.close();
                out.close();
                
                // delete source after copying
                if(remove(source.c_str()) == 0)
                    cout << "File moved successfully!" << endl;
                else
                    cout << "Failed to move file!" << endl;
                cin.ignore();
                cin.get();
            }
            break;
            case 0:
                cout<<"Closing the fileOperations"<<endl;
                sleep(1);
            break;
            default:
                cout<<"invalid choice"<<endl;
                sleep(1);
        }
    } while (choice!=0);
    
}