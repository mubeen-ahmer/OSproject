#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

int main(int argc,char* argv[]){
    cout<<argv[3]<<" "<<argv[1]<<" "<<argv[2];
    int argv1=atoi(argv[1]);
    int argv2=atoi(argv[2]);
    int argv3=atoi(argv[3]);
    cout<<argv1<<" "<< argv2<<" "<< argv3;
    return 0;
}