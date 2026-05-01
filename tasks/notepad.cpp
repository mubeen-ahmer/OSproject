#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <pthread.h>
using namespace std;

bool writing = true;

void* autosave(void* arg){
    ofstream* file = (ofstream*)arg;  // cast void* back to ofstream*
    while(writing){
        sleep(5);
        file->flush();
        cout << "\n[autosaved]" << endl;
    }
    return NULL;
}

int main(int argc, const char** argv) {
    cout<<"==Welcome to notepad=="<<endl;
    int choice;
    do{
        cout<<"1. Create a New file"<<endl
            <<"2. open a file"<<endl
            <<"3. Test autosave (demo)"<<endl
            <<"0. Close notepad"<<endl
            <<"select : ";cin>>choice;
        
        switch(choice){
            case 1:{
                string name;
                cout<<"--creating a new file--"<<endl;
                cout<<"Enter the name of the file";cin>>name;
                name = "tasks/notepadsavedfiles/"+name+".txt";
                ofstream file(name.c_str());
                if(!file){
                    cout<<"Failed to create a new file"<<endl;
                    break;
                }
                cout<<"File created! start writing (auto saving)"<<endl;
                char ch;
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
                cout<<"--creating a new file--"<<endl;
                cout<<"Enter the name of the file : ";cin>>name;
                name = "tasks/notepadsavedfiles/"+name+".txt";
                ofstream file(name.c_str());
                if(!file){
                    cout<<"Failed to create a new file"<<endl;
                    break;
                }
                writing = true;

            }
            break;
            case 4:{
                
                break;
            }
            case 3:{
                // demo: open a test file, start autosave thread, let user type
                ofstream file("tasks/notepadsavedfiles/autosave_demo.txt");
                if(!file){
                    cout<<"Failed to create file"<<endl;
                    break;
                }
                writing = true;
                pthread_t t;
                pthread_create(&t, NULL, autosave, &file);  // start autosave thread
                cout<<"Autosave demo started. Type text, press ` to stop."<<endl;
                char ch;
                do{
                    cin.get(ch);
                    file << ch;
                }while(ch != '`');
                writing = false;  // signal thread to stop
                pthread_join(t, NULL);  // wait for thread to finish
                file.close();
                cout<<"File saved and closed."<<endl;
            }break;
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