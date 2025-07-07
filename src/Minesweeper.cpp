#include <bits/stdc++.h>
#include <windows.h>
#include <fstream>
#include <ctime>
#include "MineSweeper12Segment.h"
#include <conio.h>
#include <chrono>
#include <thread>
#include <iomanip> // Required for formatting the leaderboard output

using namespace std;
using namespace chrono;

// Structure to hold player data for the leaderboard
struct Player {
    string name;
    long long int time = 0;
    int score = 0;
};

// --- Global Variables ---
string playerName;
int rows, cols, bombCount, flagsRemaining;
bool gameover = false; // This flag will be true only when the game is lost
time_point<high_resolution_clock> theStartTime;

// Leaderboard-related global variables
Player players[100];
int NumOfPlayers = 0;


// --- Function Prototypes ---
// Core Game
void showMenu();
void getPlayerName();
void getGameDimensions();
void getGameDifficulty();
void printBorder(int cols);
void MainGame(char** min, char** cell, int rows, int cols);
void ifIsZero(char** min, char** cell, int i, int j, int rows, int cols);
void game(char** min, char** cell, int i, int j, int rows, int cols);

// Leaderboard
void leaderboard();
void sortLeaderboard();
void loadData();
void saveData(string name, long long int time, bool win);
void saveInfo();

// Utility
void clearScreen();
void pauseForSeconds(int seconds);


// --- Utility Functions ---
void clearScreen() {
    // Clears the console screen.
    system("cls");
}

void pauseForSeconds(int seconds) {
    // Pauses the program execution for a specified duration.
    this_thread::sleep_for(chrono::seconds(seconds));
}


// --- Leaderboard Functions ---

void saveData(string name, long long int time, bool win) {
    // Score is higher for faster wins, and 0 for a loss.
    int score = win ? max(0LL, 1000 - time) : 0;
    ofstream file("leaderboard.txt", ios::app); // Open in append mode
    if (file.is_open()) {
        file << name << " " << score << " " << time << endl;
    }
    file.close();
}
 
void loadData() {
    ifstream file("leaderboard.txt");
    if (file.is_open()) {
        NumOfPlayers = 0; // Reset player count before loading
        // Read each line until the end of the file
        while (NumOfPlayers < 100 && file >> players[NumOfPlayers].name >> players[NumOfPlayers].score >> players[NumOfPlayers].time) {
            NumOfPlayers++;
        }
    }
    file.close();
    // After loading, sort the data to ensure the leaderboard is in order.
    sortLeaderboard();
}
void sortLeaderboard() {
    for (int i = 0; i < NumOfPlayers - 1; i++) {
        for (int j = i + 1; j < NumOfPlayers; j++) {
            // Swap if the current player's score is less than the next one's(Bubble sort)
            if (players[i].score < players[j].score) {
                Player temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }
    // After sorting the data in memory, save it back to the file.
    saveInfo();
}
void saveInfo() {
    ofstream file("leaderboard.txt", ios::trunc);
    if (file.is_open()) {
        for (int i = 0; i < NumOfPlayers; i++) {
            file << players[i].name << " " << players[i].score << " " << players[i].time << endl;
        }
    }
    file.close();
    // After saving, display the updated leaderboard.
    leaderboard();
}

void leaderboard() {
    clearScreen();
    // ASCII Art for the title
    cout << "\033[93m" << " █   ██▀ ▄▀▄ █▀▄ ██▀ █▀▄ ██▄ ▄▀▄ ▄▀▄ █▀▄ █▀▄\n" << "\033[35m" << " █▄▄ █▄▄ █▀█ █▄▀ █▄▄ █▀▄ █▄█ ▀▄▀ █▀█ █▀▄ █▄▀\n" << "\033[0m" << "\n";
    cout << "\n";

    // Table Headers
    cout << setw(15) << left << "Name"
         << setw(15) << "Score"
         << setw(10) << "Time(s)" << endl;
    cout << string(40, '_') << endl;

    // Print each player's data
    for (int i = 0; i < NumOfPlayers; i++) {
        cout << setw(15) << left << players[i].name
             << setw(15) << players[i].score
             << setw(10) << players[i].time << endl;
    }
}

// --- Core Game Setup ---

void showMenu() {
    print(); // Displays a 12-segment ASCII art representation of "Minesweeper".

    cout << string(32, ' ') << "\u2554" << string(16, '\u2550') << "\u2557\n";
    cout << string(32, ' ') << "\u2551" << string(16, ' ') << "\u2551\n";
    cout << string(32, ' ') << "\u2551" << "\033[32m" << "1. New Game" << "\033[0m" << "     " << "\u2551\n";
    cout << string(32, ' ') << "\u2551" << string(16, ' ') << "\u2551\n";
    cout << string(32, ' ') << "\u2551" << "\033[32m" << "2. Leaderboard" << "\033[0m" << "  " << "\u2551\n";
    cout << string(32, ' ') << "\u2551" << string(16, ' ') << "\u2551\n";
    cout << string(32, ' ') << "\u2551" << "\033[32m" << "3. Exit" << "\033[0m" << "         " << "\u2551\n";
    cout << string(32, ' ') << "\u2551" << string(16, ' ') << "\u2551\n";
    cout << string(32, ' ') << "\u255A" << string(16, '\u2550') << "\u255D\n";
}


void getPlayerName() {
    clearScreen();
    cout << "Enter Your Name: ";
    cin >> playerName;
}


void getGameDimensions() {
    clearScreen();
    cout << "Enter the number of rows: ";
    cin >> rows;
    cout << "Enter the number of columns: ";
    cin >> cols;
    // Basic validation for dimensions
    if (rows < 5 || cols < 5) {
        cout << "Invalid dimensions. Setting to default 10x10.\n";
        rows = 10;
        cols = 10;
        pauseForSeconds(2);
    }
}


void getGameDifficulty() {
    clearScreen();
    cout << "\u2554" << string(16, '\u2550') << "\u2557\n";
    cout << "\u2551" << "SelectDifficulty" << "\u2551\n";
    cout << "\u2560" << string(16, '\u2550') << "\u2563\n";
    cout << "\u2551" << "1. Easy         " << "\u2551\n";
    cout << "\u2551" << "                " << "\u2551\n";
    cout << "\u2551" << "2. Medium       " << "\u2551\n";
    cout << "\u2551" << "                " << "\u2551\n";
    cout << "\u2551" << "3. Hard         " << "\u2551\n";
    cout << "\u255A" << string(16, '\u2550') << "\u255D\n";

    char click1 = getch();
    switch (click1) {
    case '1':
        bombCount = (rows * cols) / 10; // ~10% bombs
        break;
    case '2':
        bombCount = (rows * cols) / 6;  // ~16% bombs
        break;
    case '3':
        bombCount = (rows * cols) / 4;  // 25% bombs
        break;
    default: // Default to easy if input is invalid
        cout << "Invalid selection. Defaulting to Easy.\n";
        bombCount = (rows * cols) / 10;
        pauseForSeconds(2);
        break;
    }
    flagsRemaining = bombCount;
}


int main() {
    SetConsoleOutputCP(CP_UTF8); // Enable UTF-8 characters for box drawing

    while (true) {
        clearScreen();
        showMenu();
        char click = getch();

        switch (click) {
        case '1': { // New Game
            gameover = false; // Reset game state for a new game
            getPlayerName();
            getGameDimensions();
            getGameDifficulty();

            // Dynamically allocate memory for the game boards
            char** cell = new char*[rows]; // Player-visible board
            char** min = new char*[rows];  // Hidden board with mines
            for (int i = 0; i < rows; i++) {
                cell[i] = new char[cols];
                min[i] = new char[cols];
                for (int j = 0; j < cols; j++) {
                    cell[i][j] = '#'; // Unrevealed cell
                    min[i][j] = '0';  // Empty cell
                }
            }

            theStartTime = high_resolution_clock::now(); // Record game start time

            // Randomly place bombs on the hidden board
            srand(time(0));
            int realmin = 0;
            while (realmin < bombCount) {
                int x = rand() % rows;
                int y = rand() % cols;
                if (min[x][y] == '0') {
                    min[x][y] = '9'; // '9' represents a bomb
                    realmin++;
                }
            }

            // Calculate the numbers for cells adjacent to bombs
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (min[i][j] == '0') {
                        int mincounter = 0;
                        for (int dx = -1; dx <= 1; dx++) {
                            for (int dy = -1; dy <= 1; dy++) {
                                int nx = i + dx;
                                int ny = j + dy;
                                if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && min[nx][ny] == '9') {
                                    mincounter++;
                                }
                            }
                        }
                        min[i][j] = '0' + mincounter;
                    }
                }
            }

            MainGame(min, cell, rows, cols); // Start the actual game loop

            // Clean up dynamically allocated memory
            for (int i = 0; i < rows; i++) {
                delete[] cell[i];
                delete[] min[i];
            }
            delete[] cell;
            delete[] min;
            break;
        }
        case '2': // Leaderboard
            loadData(); // This will load, sort, and display the leaderboard
            cout << "\nPress Enter to return to the menu...";
            getch();
            break;
        case '3': // Exit
            cout << "Exiting game. Goodbye!\n";
            pauseForSeconds(2);
            return 0; // Exit the program
        default:
            cout << "\nInvalid choice. Please press 1, 2, or 3.";
            pauseForSeconds(2);
            break;
        }
    }
    return 0;
}


// --- Game Logic ---

/**
 * @brief The main game loop where player interaction happens.
 * @param min The hidden board with mines and numbers.
 * @param cell The player-visible board.
 * @param rows The number of rows in the grid.
 * @param cols The number of columns in the grid.
 */
void MainGame(char** min, char** cell, int rows, int cols) {
    int x = 0, y = 0; // Player's cursor position

    do {
        clearScreen();

        // Check for a win condition: all non-bomb cells are revealed
        int counter = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (cell[i][j] != '#' && cell[i][j] != 'f') {
                    counter++;
                }
            }
        }
        if (counter == (rows * cols - bombCount)) {
            long long timer = duration_cast<seconds>(high_resolution_clock::now() - theStartTime).count();
            saveData(playerName, timer, true); // Save win data

            clearScreen();
            cout << "Congratulations! You Win!" << "\n";
            cout << "Time: " << timer << " seconds." << "\n";
            pauseForSeconds(4);
            return; // Exit MainGame and return to the main menu
        }

        // Display UI and Board
        cout << "\033[35m" << "Player Name : " << "\033[0m" << playerName << "\n";
        cout << "Use W,A,S,D to move. Enter to select." << "\n";
        cout << "f -> Flag/unFlag" << "\n";
        cout << "Esc -> Return to Menu" << "\n";
        printBorder(cols);

        for (int i = 0; i < rows; i++) {
            cout << "\u2551";
            for (int j = 0; j < cols; j++) {
                if (i == x && j == y) { // Highlight current cursor position
                    cout << "\033[44m" << cell[i][j] << " " << "\033[0m";
                } else {
                    cout << cell[i][j] << " ";
                }
            }
            cout << "\u2551\n";
        }

        cout << "\u255A" << string(cols * 2, '\u2550') << "\u255D\n";
        cout << "\033[35m" << "Flags Remaining: " << "\033[0m" << flagsRemaining << "\n";

        // Handle user input
        int input = getch();
        switch (input) {
        case 'd': y++; break;
        case 'a': y--; break;
        case 'w': x--; break;
        case 's': x++; break;
        case 13: // Enter key
            if (cell[x][y] == '#') game(min, cell, x, y, rows, cols);
            break;
        case 27: // Escape key
            return; // Return to main menu
        case 'f': // Flag key
            if (cell[x][y] == '#' && flagsRemaining > 0) {
                flagsRemaining--;
                cell[x][y] = 'f';
            } else if (cell[x][y] == 'f') {
                flagsRemaining++;
                cell[x][y] = '#';
            }
            break;
        }

        // Keep cursor in bounds
        if (x < 0) x = 0;
        if (x >= rows) x = rows - 1;
        if (y < 0) y = 0;
        if (y >= cols) y = cols - 1;

    } while (!gameover);

    // This part is reached only on loss (when gameover becomes true)
    long long timer = duration_cast<seconds>(high_resolution_clock::now() - theStartTime).count();
    saveData(playerName, timer, false); // Save loss data
    
    clearScreen();
    cout << "GameOver!\n";
    printBorder(cols);
    for (int i = 0; i < rows; i++) {
        cout << "\u2551";
        for (int j = 0; j < cols; j++) {
            if (min[i][j] == '9') {
                cout << "\033[91m" << "B " << "\033[0m"; // Show all Bombs
            } else {
                cout << cell[i][j] << " ";
            }
        }
        cout << "\u2551\n";
    }
    cout << "\u255A" << string(cols * 2, '\u2550') << "\u255D\n";
    pauseForSeconds(5);
}

/**
 * @brief Handles the logic for clicking a single cell. Updates the board state.
 * @param min The hidden board.
 * @param cell The visible board.
 * @param i The row of the clicked cell.
 * @param j The column of the clicked cell.
 */
void game(char** min, char** cell, int i, int j, int rows, int cols) {
    if (min[i][j] == '9') {
        gameover = true; // Signal a loss to the MainGame loop
        return;
    }
    if (min[i][j] == '0') {
        ifIsZero(min, cell, i, j, rows, cols); // Reveal adjacent cells
    } else {
        cell[i][j] = min[i][j]; // Reveal the number
    }
}

/**
 * @brief Recursively reveals adjacent cells if an empty ('0') cell is clicked.
 * This is a flood-fill algorithm.
 */
void ifIsZero(char** min, char** cell, int i, int j, int rows, int cols) {
    // Boundary and state checks to prevent infinite recursion
    if (i < 0 || i >= rows || j < 0 || j >= cols || cell[i][j] != '#') {
        return;
    }

    cell[i][j] = min[i][j];

    // If the revealed cell is '0', recurse for all its neighbors
    if (min[i][j] == '0') {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                // Skip the center cell itself
                if (dx == 0 && dy == 0) continue;
                ifIsZero(min, cell, i + dx, j + dy, rows, cols);
            }
        }
    }
}

/**
 * @brief Prints the top or bottom border of the game grid.
 * @param cols The number of columns to size the border correctly.
 */
void printBorder(int cols) {
    cout << "\u2554" << string(cols * 2, '\u2550') << "\u2557\n";
}
