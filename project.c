#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 9  // Grid size (9x9)
#define BOMBS 10  // Number of bombs

// Function declarations
void initializeBoard(char board[SIZE][SIZE], char value);
void printBoard(char board[SIZE][SIZE]);
void placeBombs(char board[SIZE][SIZE]);
int isValid(int row, int col);
int countAdjacentBombs(char board[SIZE][SIZE], int row, int col);
void revealCell(char realBoard[SIZE][SIZE], char displayBoard[SIZE][SIZE], int row, int col);
int isGameWon(char displayBoard[SIZE][SIZE]);

int main() {
    char realBoard[SIZE][SIZE];  // Board with bombs and numbers
    char displayBoard[SIZE][SIZE];  // Board shown to the player
    int row, col;
    int movesLeft;
    int gameOver;
    char playAgain;

    do {
        // Initialize variables for a new game
        movesLeft = SIZE * SIZE - BOMBS;
        gameOver = 0;

        // Initialize both boards
        initializeBoard(realBoard, '0');
        initializeBoard(displayBoard, '*');

        // Place bombs randomly on the real board
        placeBombs(realBoard);

        // Game loop
        while (!gameOver) {
            printBoard(displayBoard);
            printf("Enter row and column to reveal (0-%d): ", SIZE - 1);
            scanf("%d %d", &row, &col);

            if (!isValid(row, col)) {
                printf("Invalid move! Try again.\n");
                continue;
            }

            if (realBoard[row][col] == 'B') {
                printf("Boom! You hit a bomb. You Lost!\n");
                printBoard(realBoard);  // Reveal the real board
                gameOver = 1;
            } else {
                revealCell(realBoard, displayBoard, row, col);
                movesLeft--;
                if (movesLeft == 0) {
                    printf("Congratulations! You've cleared the board. You Win!\n");
                    gameOver = 1;
                }
            }
        }

        // Ask for a rematch or exit
        printf("Do you want to play again? (y/n): ");
        scanf(" %c", &playAgain);

    } while (playAgain == 'y' || playAgain == 'Y');

    printf("Thank you for playing! Goodbye!\n");
    return 0;
}

// Initialize the board with a specific value
void initializeBoard(char board[SIZE][SIZE], char value) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = value;
        }
    }
}

// Print the board to the console
void printBoard(char board[SIZE][SIZE]) {
    printf("   ");
    for (int i = 0; i < SIZE; i++) printf("%d ", i);
    printf("\n");

    for (int i = 0; i < SIZE; i++) {
        printf("%d  ", i);
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

// Randomly place bombs on the board
void placeBombs(char board[SIZE][SIZE]) {
    srand(time(0));
    int bombsPlaced = 0;
    while (bombsPlaced < BOMBS) {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        if (board[row][col] != 'B') {
            board[row][col] = 'B';
            bombsPlaced++;
        }
    }
}

// Check if a given row and column are within the board's boundaries
int isValid(int row, int col) {
    return (row >= 0 && row < SIZE && col >= 0 && col < SIZE);
}

// Count the number of bombs adjacent to a given cell
int countAdjacentBombs(char board[SIZE][SIZE], int row, int col) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int newRow = row + i;
            int newCol = col + j;
            if (isValid(newRow, newCol) && board[newRow][newCol] == 'B') {
                count++;
            }
        }
    }
    return count;
}

// Reveal a cell and recursively reveal adjacent cells if they have no bombs nearby
void revealCell(char realBoard[SIZE][SIZE], char displayBoard[SIZE][SIZE], int row, int col) {
    if (!isValid(row, col) || displayBoard[row][col] != '*') return;

    int bombs = countAdjacentBombs(realBoard, row, col);
    displayBoard[row][col] = bombs + '0';

    // If no adjacent bombs, reveal neighboring cells recursively
    if (bombs == 0) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i != 0 || j != 0) {
                    revealCell(realBoard, displayBoard, row + i, col + j);
                }
            }
        }
    }
}

// Check if the player has won the game
int isGameWon(char displayBoard[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (displayBoard[i][j] == '*' && displayBoard[i][j] != 'B') {
                return 0;  // Game not yet won
            }
        }
    }
    return 1;  // All non-bomb cells revealed
}
