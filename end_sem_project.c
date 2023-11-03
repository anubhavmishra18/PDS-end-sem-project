#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 9
#define S 3

int K;
int mat[N][N];
int num;
int copymat[N][N];

//for selecting level of the game
int Levelselector() {
    int n;
    printf("Select level:\nEnter\n1 for EASY\n2 for MEDIUM\n3 for HARD\n");
    scanf("%d", &n);
    switch (n) {
        case 1:
            K = 28;
            break;
        case 2:
            K = 40;
            break;
        case 3:
            K = 54;
            break;
        default:
            printf("Enter a valid level");
            exit(0);
    }
}

//for checking whether a number is used  in the 3x3 subgrid
int unUsedInBox(int rowStart, int colStart, int num) {
    for (int i = 0; i < S; i++)
        for (int j = 0; j < S; j++)
            if (mat[rowStart + i][colStart + j] == num)
                return 0;
    return 1;
}

//to check whether a number is used in a particular column
int unUsedInCol(int col, int num) {
    for (int row = 0; row < N; row++)
        if (mat[row][col] == num)
            return 0;
    return 1;
}

//to check whether a number is used in a particular row
int unUsedInRow(int row, int num) {
    for (int col = 0; col < N; col++)
        if (mat[row][col] == num)
            return 0;
    return 1;
}

//if the above 3 functions conditions are satisfied then number is safe to fill in cell
int checkIfSafe(int row, int col, int num) {
    return unUsedInRow(row, num) && unUsedInCol(col, num) && unUsedInBox(row - row % S, col - col % S, num);
}

//for filling numbers in the 3x3 subgrid
void fillBox(int row, int col) {
    void initializeRandomSeed();
    int num, i, j;
    for (i = 0; i < S; i++)
        for (j = 0; j < S; j++)
            do {
                num = rand() % N + 1;
            } while (!unUsedInBox(row, col, num));

         mat[row + i][col + j] = num;
}

//for filling the diagonal elements
void fillDiagonal() {
    int i;
    for (i = 0; i < N; i = i + S)
        fillBox(i, i);
}

//filling the remaining elements
int fillRemaining(int i, int j) {
    if (i == N - 1 && j == N)
        return 1;

    if (j == N) {
        i++;
        j = 0;
    }

    if (mat[i][j] != 0)
        return fillRemaining(i, j + 1);

    for (int num = 1; num <= N; num++) {
        if (checkIfSafe(i, j, num)) {
            mat[i][j] = num;
            if (fillRemaining(i, j + 1))
                return 1;
            mat[i][j] = 0;
        }
    }
    return 0;
}

//creating the game by removing random k digits(based on level) from the 9x9 matrix
void removeKDigits() {
    int count = K;
    while (count != 0) {
        int i = rand() % N;
        int j = rand()% N;
        if (mat[i][j] != 0) {
            mat[i][j] = 0;
            count--;
        }
    }
}

//printing the sudoku puzzle
void printSudoku() {
    printf("Sudoku Puzzle:\n");
    printf("+=====+=====+=====+=====+=====+=======+\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++){
            printf("| %d ", mat[i][j]);
            if((j+1)%3==0){
                printf("|");
            }
        }
        printf("|\n");
        if ((i+1)%3 == 0) 
            printf("+=====+=====+=====+=====+=====+=======+\n");
    }
}

//copying the matrix to another matrix for checking answer purpose
void copymatrix(){
    for(int i=0;  i<N; i++){
        for(int j=0; j<N; j++){
            copymat[i][j]=mat[i][j];
        }    
    }
}

//for checking if element is safe to fill in the given sudoku puzzle(for answer purpose)
int canfill(int puzzle[9][9], int row, int col, int num){
    int rowStart = (row/3) * 3;
    int colStart = (col/3) * 3;
    int i, j;

    for(i=0; i<9; ++i){
        if (puzzle[row][i] == num) 
        return 0;
        if (puzzle[i][col] == num) 
        return 0;
        if (puzzle[rowStart + (i%3)][colStart + (i/3)] == num) 
        return 0;
    }
    return 1;
}

//taking inputs from the user and playing the game without breaking the rules
void solvingsudoku(int matrix[9][9]) {
    int row, col, newValue;

    do {
        printf("Enter the row and column no. of cell to fill (row and column should be in the range 1-9, 0 0 to exit): ");
        scanf("%d %d", &row, &col);

        if (row == 0 && col == 0) {
            break; 
        }

        if (row >= 1 && row <= 9 && col >= 1 && col <= 9) {
            printf("Enter the value (from 1 to 9) or 0 to clear the cell: ");
            scanf("%d", &newValue);
            if (newValue >= 0 && newValue <= 9) {
                if (canfill(matrix, row - 1, col - 1, newValue)) {
                    matrix[row - 1][col - 1] = newValue;
                    printSudoku(matrix);
                }
            else{printf("Invalid input.\n");}
            } 
            else {
                printf("Invalid value. Please enter a number between 0 and 9.\n");
            }
        } 
        else {
            printf("Invalid row or column index. Element not replaced.\n");
        }
    } while (1);  
}

//filling the copied sudoku for answer purpose
int fillSudoku(int puzzle[9][9], int row, int col){
    int i;
    if(row<9 && col<9){
        if(puzzle[row][col] != 0){
            if((col+1)<9) 
            return fillSudoku(puzzle, row, col+1);
            else if((row+1)<9) 
            return fillSudoku(puzzle, row+1, 0);
            else 
            return 1;
        }
        else{
            for(i=0; i<9; ++i){
                if(canfill(puzzle, row, col, i+1)){
                    puzzle[row][col] = i+1;
                    if((col+1)<9){
                        if(fillSudoku(puzzle, row, col +1)) 
                        return 1;
                        else puzzle[row][col] = 0;
                    }
                    else if((row+1)<9){
                        if(fillSudoku(puzzle, row+1, 0)) 
                        return 1;
                        else puzzle[row][col] = 0;
                    }
                    else 
                    return 1;
                    }
            }
        }
        return 0;
    }
    else return 1;
}

//main block of code which contains the necessary functions and executes the main game
int main() {
    printf("RULES OF THE SUDOKU GAMEGAME:\n");
    printf("1. FILL THE 9X9 GRID WITHOUT REPITITION OF NUMBERS IN ROWS, COLUMNS AND EVEN THE 3X3 SUBGRID\n");
    printf("2. TO WIN FILL ALL THE NUMBERS OF THE 9X9 GRID BY FOLLOWING THE ABOVE RULE\n");
    printf("3. ENTER  NUMBERS ONLY BETWEEN 1 TO 9\n");
    printf("ALL THE BEST\n");
    Levelselector();
    fillDiagonal();
    fillRemaining(0, S);
    removeKDigits();
    printSudoku();
    copymatrix();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            while (mat[i][j] == 0) {
                solvingsudoku(mat);
                printSudoku();
            }
        }
    }

    if (fillSudoku(copymat, 0, 0)) {
        int isCorrect = 1; 

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (mat[i][j] != copymat[i][j]) {
                    isCorrect = 0; 
                    break;
                }
            }
        }

        if (isCorrect) {
            printf("Congratulations! Your solution is correct.\n");
        } else {
            printf("Wrong solution. \n");
            printf("The correct solution is: \n");
            if(fillSudoku(copymat, 0, 0)){
                printf("+=====+=====+=====+=====+=====+=======+\n");
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < N; j++){
                    printf("| %d ", copymat[i][j]);
                    if((j+1)%3==0){
                        printf("|");
                    }
                }
                printf("|\n");
                if ((i+1)%3 == 0) 
                printf("+=====+=====+=====+=====+=====+=======+\n");
                }
    }
               
        }
    } else {
        printf("Invalid user solution. The Sudoku puzzle is not correctly filled.\n");
    }

    return 0;
}