#include <iostream>
#include <unistd.h>
using namespace std;

int main(){
    int choice;
    double a,b;
    do {
        system("clear");
        cout<<"==Welcome to the calculator=="<<endl;
        cout<<"1. Addition of two numbers"<<endl;
        cout<<"2. Subtraction of two numers"<<endl;
        cout<<"3. Multiplication of two numbers"<<endl;
        cout<<"4. division of two numbers"<<endl;
        cout<<"0. close the calculator "<<endl;
        cout<<endl<<"select : ";cin>>choice;
        switch(choice){
            case 1 :
                cout<<"Enter first number : ";cin>>a;
                cout<<"Enter second number : ";cin>>b;
                cout<<"Result : "<<a+b<<endl;
                cin.ignore();
                cin.get();
            break;
            case 2 :
                cout<<"Enter first number : ";cin>>a;
                cout<<"Enter second number : ";cin>>b;
                cout<<"Result : "<<a-b<<endl;
                cin.ignore();
                cin.get();

            break;
            case 3 :
                cout<<"Enter first number : ";cin>>a;
                cout<<"Enter second number : ";cin>>b;
                cout<<"Result : "<<a*b<<endl;
                cin.ignore();
                cin.get();

            break;
            case 4 :
                cout<<"Enter first number : ";cin>>a;
                cout<<"Enter second number : ";cin>>b;
                if(b==0) cout<<"division with 0 isn't allowed."<<endl;
                else cout<<"Result : "<<a/b<<endl;
                cin.ignore();
                cin.get();

            break;
            case 0 :
                cout<<"Closing the calculator"<<endl;
                sleep(1);
            break;
            default:
                cout<<"Invalid choice"<<endl;
                cin.ignore();
                cin.get();

        }
    }while(choice!=0);
    return 0;
}