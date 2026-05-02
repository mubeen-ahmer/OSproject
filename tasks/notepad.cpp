#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <pthread.h>
using namespace std;



int main(int argc, const char** argv) {
    cout<<"==Welcome to notepad=="<<endl;
    int choice;
    do{
        cout<<"1. Create a New file"<<endl
            <<"2. open a file"<<endl
            <<"0. Close notepad"<<endl
            <<"select : ";cin>>choice;
        
        switch(choice){
            case 1:{
                string name;
                cout<<"--creating a new file--"<<endl;
                cout<<"Enter the name of the file : ";cin>>name;
                name = "tasks/notepadsavedfiles/"+name+".txt";
                ofstream file(name.c_str());
                if(!file){
                    cout<<"Failed to create a new file."<<endl;
                    sleep(1);
                    break;
                }
                cout<<"File created! start writing (auto saving)"<<endl;
                char ch;
                cin.ignore();
                do{
                    cin.get(ch);
                    file<<ch;
                }while(ch != '`');
                file.close();   
            }
            break;
            case 2:
            {
                string name;
                cout<<"Showing all the saved files :"<<endl;
                system("ls tasks/notepadsavedfiles/");
                cout<<"Enter the name of the saved file you want to open : ";cin>>name;
                name = "tasks/notepadsavedfiles/"+name+".txt";
                ifstream in(name.c_str());
                if(!in){
                    cout<<"Failed to open a file."<<endl;
                    sleep(1);
                    break;
                }
                char ch;
                while(in.get(ch)){
                    cout<<ch;
                }
                in.close();
                ofstream out(name.c_str(),ios::app); 
                cin.ignore();
                char c;
                do{
                    cin.get(c);
                    out<<c;
                }while(c != '`');
                out.close();  

            }
            break;
            case 0:
                cout<<"Closing notepad"<<endl;
                sleep(1);
            break;
            default:
                cout<<"Invalid choice"<<endl;
        }
    }while(choice!=0);
    
    return 0;
}