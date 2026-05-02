#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <termios.h>
using namespace std;

const int ROWS    = 16;
const int COLS    = 16;
const int MINES   = 40;

// grid data
int  board[ROWS][COLS];      // actual board (-1 = mine, 0-8 = number)
bool revealed[ROWS][COLS];   // is cell revealed
bool flagged[ROWS][COLS];    // is cell flagged

int  cursorRow = 0;
int  cursorCol = 0;
bool gameOver  = false;
bool won       = false;

// ============================================================
//  TERMINAL RAW MODE - to read arrow keys without Enter
// ============================================================
struct termios orig_termios;

void enableRawMode(){
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disableRawMode(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

// ============================================================
//  READ KEY - handles arrow keys (3 byte sequences)
// ============================================================
int readKey(){
    char c;
    read(STDIN_FILENO, &c, 1);
    if(c == 27){  // escape sequence
        char seq[2];
        read(STDIN_FILENO, &seq[0], 1);
        read(STDIN_FILENO, &seq[1], 1);
        if(seq[0] == '['){
            if(seq[1] == 'A') return 1;  // up
            if(seq[1] == 'B') return 2;  // down
            if(seq[1] == 'C') return 3;  // right
            if(seq[1] == 'D') return 4;  // left
        }
    }
    return c;
}

// ============================================================
//  PLACE MINES randomly on the board
// ============================================================
void placeMines(){
    int placed = 0;
    while(placed < MINES){
        int r = rand() % ROWS;
        int c = rand() % COLS;
        if(board[r][c] != -1){
            board[r][c] = -1;
            placed++;
        }
    }
}

// ============================================================
//  CALCULATE numbers for each cell
// ============================================================
void calculateNumbers(){
    for(int r = 0; r < ROWS; r++){
        for(int c = 0; c < COLS; c++){
            if(board[r][c] == -1) continue;
            int count = 0;
            for(int dr = -1; dr <= 1; dr++){
                for(int dc = -1; dc <= 1; dc++){
                    int nr = r + dr;
                    int nc = c + dc;
                    if(nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && board[nr][nc] == -1)
                        count++;
                }
            }
            board[r][c] = count;
        }
    }
}

// ============================================================
//  FLOOD FILL - reveal empty cells automatically
// ============================================================
void reveal(int r, int c){
    if(r < 0 || r >= ROWS || c < 0 || c >= COLS) return;
    if(revealed[r][c] || flagged[r][c]) return;
    revealed[r][c] = true;
    if(board[r][c] == 0){
        for(int dr = -1; dr <= 1; dr++)
            for(int dc = -1; dc <= 1; dc++)
                reveal(r + dr, c + dc);
    }
}

// ============================================================
//  CHECK WIN - all non-mine cells revealed
// ============================================================
bool checkWin(){
    for(int r = 0; r < ROWS; r++)
        for(int c = 0; c < COLS; c++)
            if(board[r][c] != -1 && !revealed[r][c])
                return false;
    return true;
}

// ============================================================
//  DISPLAY BOARD
// ============================================================
void display(){
    system("clear");
    cout << "=== MINESWEEPER ===" << endl;
    cout << "Mines: " << MINES << " | Arrow keys: move | Enter: reveal | f: flag | q: quit" << endl;
    cout << endl;

    // column numbers
    cout << "   ";
    for(int c = 0; c < COLS; c++)
        printf("%2d", c);
    cout << endl;

    // top border
    cout << "   ";
    for(int c = 0; c < COLS; c++) cout << "--";
    cout << endl;

    for(int r = 0; r < ROWS; r++){
        printf("%2d|", r);  // row number
        for(int c = 0; c < COLS; c++){
            bool isCursor = (r == cursorRow && c == cursorCol);

            if(isCursor) cout << "[";
            else         cout << " ";

            if(gameOver && board[r][c] == -1){
                cout << "*";  // show all mines on game over
            } else if(flagged[r][c] && !revealed[r][c]){
                cout << "F";
            } else if(!revealed[r][c]){
                cout << "#";
            } else if(board[r][c] == 0){
                cout << ".";
            } else if(board[r][c] == -1){
                cout << "*";
            } else {
                cout << board[r][c];
            }

            if(isCursor) cout << "]";
            else         cout << " ";
        }
        cout << endl;
    }
    cout << endl;

    if(gameOver) cout << "*** BOOM! Game Over! ***" << endl;
    if(won)      cout << "*** YOU WIN! ***"          << endl;
}

// ============================================================
//  INIT - reset everything
// ============================================================
void init(){
    srand(time(0));
    for(int r = 0; r < ROWS; r++)
        for(int c = 0; c < COLS; c++){
            board[r][c]    = 0;
            revealed[r][c] = false;
            flagged[r][c]  = false;
        }
    placeMines();
    calculateNumbers();
    cursorRow = 0;
    cursorCol = 0;
    gameOver  = false;
    won       = false;
}

// ============================================================
//  MAIN
// ============================================================
int main(){
    init();
    enableRawMode();

    while(true){
        display();

        if(gameOver || won){
            cout << "Press r to restart or q to quit: ";
            char ch;
            read(STDIN_FILENO, &ch, 1);
            if(ch == 'r') init();
            else if(ch == 'q') break;
            continue;
        }

        int key = readKey();

        switch(key){
            case 1:  // up
                if(cursorRow > 0) cursorRow--;
                break;
            case 2:  // down
                if(cursorRow < ROWS-1) cursorRow++;
                break;
            case 3:  // right
                if(cursorCol < COLS-1) cursorCol++;
                break;
            case 4:  // left
                if(cursorCol > 0) cursorCol--;
                break;
            case '\r':  // enter - reveal
            case '\n':
                if(!flagged[cursorRow][cursorCol]){
                    if(board[cursorRow][cursorCol] == -1){
                        revealed[cursorRow][cursorCol] = true;
                        gameOver = true;
                    } else {
                        reveal(cursorRow, cursorCol);
                        if(checkWin()) won = true;
                    }
                }
                break;
            case 'f':  // flag
                if(!revealed[cursorRow][cursorCol])
                    flagged[cursorRow][cursorCol] = !flagged[cursorRow][cursorCol];
                break;
            case 'q':  // quit
                disableRawMode();
                return 0;
        }
    }

    disableRawMode();
    sleep(1);
    return 0;
}