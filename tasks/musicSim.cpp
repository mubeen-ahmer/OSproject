#include <iostream>
#include <string>
#include <unistd.h>
#include <pthread.h>
using namespace std;

bool playing = false;
int duration = 10; 

void* musicThread(void* arg){
    for(int i = 0; i <= duration && playing; i++){
        int percent = (i * 100) / duration;
        int bars    = percent / 5;  // 20 bars total

        printf("\r[");
        for(int j = 0; j < 20; j++){
            if(j < bars) printf("=");
            else if(j == bars) printf(">");
            else printf(" ");
        }
        printf("] %d%%", percent);
        fflush(stdout);
        sleep(1);
    }
    cout << "\nSong finished!" << endl;
    playing = false;
    return nullptr;
}

int main(){
    string song;
    cout << "--Music Simulator--" << endl;
    cout << "Enter song name : ";
    getline(cin, song);
    cout << "Enter song duration(seconds) : "; cin >> duration;

    playing = true;
    pthread_t tid;
    pthread_create(&tid, nullptr, musicThread, nullptr);

    cout << "Playing: " << song << endl;
    cout << "Press any key to stop..." << endl;
    cin.ignore();
    cin.get();
    playing = false;
    pthread_join(tid, nullptr);
    cout << "Music stopped!" << endl;
    sleep(1);
    return 0;
}

/*
 * ============================================================
 *  HOW MUSIC SIM WORKS
 * ============================================================
 *
 *  Since we cannot play real music, we simulate it using
 *  a background thread and a progress bar.
 *
 *  FLOW:
 *      1. User enters song name and duration
 *      2. A thread starts and runs in background
 *      3. Thread updates progress bar every second
 *      4. Main thread waits for user to press a key to stop
 *      5. If user stops → playing = false → thread exits
 *      6. If song finishes → thread exits on its own
 *
 *  KEY CONCEPTS USED:
 *      pthread    → background thread for progress bar
 *      playing    → shared bool to stop thread from main
 *      \r         → reprints on same line (progress bar effect)
 *      fflush()   → forces output to print immediately
 *
 * ============================================================
 */