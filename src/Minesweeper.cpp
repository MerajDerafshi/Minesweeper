#include <bits/stdc++.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <fstream>
#include <ctime>
#include <conio.h>
#include <chrono>
#include <thread>
#include <iomanip> 

#define PURPLE "\033[35m"
#define AMETHYSTINE "\033[1;35m"
#define TURQUOISE "\033[1;36m"
#define COBALTBLUE "\033[36m"
#define GRAY "\033[1;90m"
#define RED "\033[1;31m"
#define BLUE "\033[1;34m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[1;92m"
#define RESET "\033[0m"


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
bool gameover = false;
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

void pauseForMilliseconds(int seconds) {
    // Pauses the program execution for a specified duration.
    this_thread::sleep_for(chrono::milliseconds(seconds));
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
            file << players[i].name << " " 
                 << players[i].score << " " 
                 << players[i].time << endl;
        }
    }
    file.close();
    // After saving, display the updated leaderboard.
    leaderboard();
}

void leaderboard() {
    clearScreen();
    // ASCII Art for the title
    cout << TURQUOISE << " _                _           _                         _ \n";            pauseForMilliseconds(500);
    cout << "| | ___  __ _  __| | ___ _ __| |__   ___   __ _ _ __ __| |\n";                         pauseForMilliseconds(500);
    cout << "| |/ _ \\/ _` |/ _` |/ _ \\ '__| '_ \\ / _ \\ / _` | '__/ _` |\n" << RESET;            pauseForMilliseconds(500);
    cout << AMETHYSTINE << "| |  __/ (_| | (_| |  __/ |  | |_) | (_) | (_| | | | (_| |\n";          pauseForMilliseconds(500);
    cout << "|_|\\___|\\__,_|\\__,_|\\___|_|  |_.__/ \\___/ \\__,_|_|  \\__,_|\n" << "\n" << RESET; pauseForMilliseconds(1500);
    

    // Table Headers
    cout << GRAY << setw(13) << left << "Name"
         << setw(15) << "Score"
         << setw(10) << "Time(s)" << RESET << "\n";
    cout << PURPLE << string(40, '_') << RESET << "\n";

    // Print each player's data
    for (int i = 0; i < NumOfPlayers; i++) {
        cout << GRAY << setw(15) << left << players[i].name
             << setw(15) << players[i].score
             << setw(10) << players[i].time << RESET << "\n";
    }
}

// --- Core Game Setup ---

void showMenu() {
    cout << PURPLE << " __  __ _                                                   \n";
    pauseForMilliseconds(500);
    cout << "|  \\/  (_)_ __   ___ "             << RESET << YELLOW << " _____      _____  ___ _ __   ___ _ __ \n"        << RESET;
    pauseForMilliseconds(500);
    cout << PURPLE << "| |\\/| | | '_ \\ / _ \\" << RESET << YELLOW << "/ __\\ \\ /\\ / / _ \\/ _ \\ '_ \\ / _ \\ '__|\n" << RESET;
    pauseForMilliseconds(500);
    cout << PURPLE << "| |  | | | | | |  __/"    << RESET << YELLOW << "\\__ \\\\ V  V /  __/  __/ |_) |  __/ |   \n"     << RESET;
    pauseForMilliseconds(500);
    cout << PURPLE << "|_|  |_|_|_| |_|\\___|"   << RESET << YELLOW << "|___/ \\_/\\_/ \\___|\\___| .__/ \\___|_|   \n";
    pauseForMilliseconds(500);
    cout << "                                           |_|              \n"                                              << RESET;
    pauseForMilliseconds(1500);

    cout << string(20, ' ') << PURPLE << "┌────────────────────┐\n";
    pauseForMilliseconds(250);
    cout << string(20, ' ') << "│" << RESET << YELLOW << "     1.New Game     " << RESET << PURPLE << "│\n";
    pauseForMilliseconds(250);
    cout << string(20, ' ') << "└────────────────────┘\n" << RESET;
    pauseForMilliseconds(250);
    cout << string(20, ' ') << PURPLE << "┌────────────────────┐\n";
    pauseForMilliseconds(250);
    cout << string(20, ' ') << "│" << RESET << YELLOW << "    2.Leaderboard   " << RESET << PURPLE << "│\n";
    pauseForMilliseconds(250);
    cout << string(20, ' ') << "└────────────────────┘\n" << RESET;
    pauseForMilliseconds(250);
    cout << string(20, ' ') << PURPLE << "┌────────────────────┐\n";
    pauseForMilliseconds(250);
    cout << string(20, ' ') << "│" << RESET << YELLOW << "       3.Exit       " << RESET << PURPLE << "│\n";
    pauseForMilliseconds(250);
    cout << string(20, ' ') << "└────────────────────┘\n" << RESET;
}


void getPlayerName() {
    clearScreen();
    cout << "\033[1;37;46m" << "\nEnter your name:\n" << RESET;
    cin  >> playerName;
}


void getGameDimensions() {
    clearScreen();
    cout << "\033[1;37;46m" << "Enter the number of rows:\n" << RESET;
    cin  >> rows;
    cout << "\033[1;37;46m" << "Enter the number of columns:\n" << RESET;
    cin  >> cols;
    // Basic validation for dimensions
    if (rows < 5 || cols < 5) {
        cout << "\033[1;30;43m" << "Invalid dimensions. Setting to default 10x10.\n" << RESET;
        rows = 10;
        cols = 10;
        pauseForMilliseconds(2000);
    }
}


void getGameDifficulty() {
    clearScreen();
    cout << TURQUOISE << " ____       _           _  "     << RESET << YELLOW << " ____  _  __  __ _            _ _         \n"     << RESET;
    pauseForMilliseconds(250);
    cout << TURQUOISE << "/ ___|  ___| | ___  ___| |_"     << RESET << YELLOW << "|  _ \\(_)/ _|/ _(_) ___ _   _| | |_ _   _ \n"    << RESET;
    pauseForMilliseconds(250);
    cout << TURQUOISE << "\\___ \\ / _ \\ |/ _ \\/ __| __" << RESET << YELLOW << "| | | | | |_| |_| |/ __| | | | | __| | | |\n"     << RESET;
    pauseForMilliseconds(250);
    cout << TURQUOISE << " ___) |  __/ |  __/ (__| |_"     << RESET << YELLOW << "| |_| | |  _|  _| | (__| |_| | | |_| |_| |\n"     << RESET;
    pauseForMilliseconds(250);
    cout << TURQUOISE << "|____/ \\___|_|\\___|\\___|\\__" << RESET << YELLOW << "|____/|_|_| |_| |_|\\___|\\__,_|_|\\__|\\__, |\n" << RESET;
    pauseForMilliseconds(250);
    cout << YELLOW    << "                                                               |___/ \n"                                  << RESET;
    pauseForMilliseconds(500);

    cout << string(20, ' ') << TURQUOISE << "┌────────────────────┐\n";
    pauseForMilliseconds(250);
    cout << string(20, ' ') << "│" << RESET << YELLOW << "       1.Easy       " << RESET << TURQUOISE << "│\n";
    pauseForMilliseconds(250);
    cout << string(20, ' ') << "└────────────────────┘\n" << RESET;
    pauseForMilliseconds(250);
    cout << string(20, ' ') << TURQUOISE << "┌────────────────────┐\n";
    pauseForMilliseconds(250);
    cout << string(20, ' ') << "│" << RESET << YELLOW << "      2.Medium      " << RESET << TURQUOISE << "│\n";
    pauseForMilliseconds(250);
    cout << string(20, ' ') << "└────────────────────┘\n" << RESET;
    pauseForMilliseconds(250);
    cout << string(20, ' ') << TURQUOISE << "┌────────────────────┐\n";
    pauseForMilliseconds(250);
    cout << string(20, ' ') << "│" << RESET << YELLOW << "       3.Hard       " << RESET << TURQUOISE << "│\n";
    pauseForMilliseconds(250);
    cout << string(20, ' ') << "└────────────────────┘\n" << RESET;

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
        cout << "\033[1;37;46m" << "Invalid selection. Defaulting to Easy.\n" << RESET;
        bombCount = (rows * cols) / 10;
        pauseForMilliseconds(2000);
        break;
    }
    flagsRemaining = bombCount;
}


int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif

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
            cout  << "\033[1;37m" << "\nPress Enter to return to the menu..." << RESET;
            getch();
            break;
        case '3': // Exit
            cout << RED << "Exiting game. Goodbye!\n";
            pauseForMilliseconds(2000);
            return 0; // Exit the program
        default:
            cout << "\033[1;37;46m" << "\nInvalid choice. Please press 1, 2, or 3." << RESET;
            pauseForMilliseconds(2000);
            getch();
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
            cout << GREEN << "\nCongratulations! You Win!\n";
            cout << "Time: " << timer << " seconds.\n" << RESET;
            pauseForMilliseconds(4000);
            return; // Exit MainGame and return to the main menu
        }

        // Display UI and Board
        cout << COBALTBLUE << "Hi, " << playerName << "\n" << RESET;
        cout << "Use W,A,S,D to move. Enter to select." << "\n";
        cout << "f -> Flag/unFlag" << "\n";
        cout << "Esc -> Return to Menu" << "\n";

        printBorder(cols);

        for (int i = 0; i < rows; i++) {
            cout << TURQUOISE << "│" << RESET;
            for (int j = 0; j < cols; j++) {
                if (i == x && j == y) { // Highlight current cursor position
                    cout << YELLOW << cell[i][j] << " " << RESET;
                } else {
                    cout << cell[i][j] << " ";
                }
            }
            cout << TURQUOISE << "│\n" << RESET;
        }

        cout << TURQUOISE << "└";
        for (int i = 0; i < cols * 2; i++) {
            cout << "─";
        } 
        cout << "┘\n" << RESET;
        cout << COBALTBLUE << "Flags Remaining: " << RESET << flagsRemaining << "\n";

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
    cout << RED << "GameOver!\n" << RESET;
    printBorder(cols);
    for (int i = 0; i < rows; i++) {
        cout << TURQUOISE << "│" << RESET;
        for (int j = 0; j < cols; j++) {
            if (min[i][j] == '9') {
                cout << RED << "B " << RESET; // Show all Bombs
            } else {
                cout << cell[i][j] << " ";
            }
        }
        cout << TURQUOISE << "│\n" << RESET;
    }
    cout << TURQUOISE << "└";
    for (int i = 0; i < cols * 2; i++) {
        cout << "─";
    } 
    cout << "┘\n" << RESET;
    pauseForMilliseconds(5000);
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
    cout << TURQUOISE << "┌";
    for (int i = 0; i < cols * 2; i++) {
        cout << "─";
    } 
    cout << "┐\n" << RESET;
}
