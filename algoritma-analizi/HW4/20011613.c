#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

// Kullan�lan fonksiyonlar
bool placeQueenCorrect(int **board, int n, int row, int col);
void printBoard(int **board, int n);
bool backtracking(int **board, int n, int col);
void bruteForce(int n);
void optimized1(int n);
void optimized2(int n);
void backtrackingMode(int n);
void runAllModes(int n);
int **createBoard(int n);
void freeBoard(int **board, int n);
void copyBoard(int **source, int **dest, int n);
bool isValidPlacement(int **board, int n);
void generateCombination(int *positions, int n, int totalCells, int index, int start, int **board);
void generateCombinationOptimized1(int *positions, int n, int totalCells, int index, int start, int **board);
void generateCombinationOptimized2(int *positions, int n, int totalCells, int index, int start, int **board);
bool isUniqueSolution(int **board, int ***solutions, int count, int n);

// Toplam cevap saymak i�in
int solutionCount = 0;

/*
@brief Program�n ba�lang�� noktas�. Kullan�c�dan N de�eri alarak N-Queens problemini ��zmek i�in bir ��z�m y�ntemi se�mesini sa�lar.
       Se�ilen y�nteme g�re ilgili algoritma �al��t�r�l�r ve sonu�lar yazd�r�l�r.
@param void
@return int Program�n ba�ar�yla tamamlan�p tamamlanmad���n� g�sterir.
*/
int main() {
    int n, mode;

    printf("Enter the size of the board (N): ");
    scanf("%d", &n);

    do {
        printf("\nSelect mode:\n");
        printf("1. Brute Force\n");
        printf("2. Optimized 1 (Same row check)\n");
        printf("3. Optimized 2 (Same row and column check)\n");
        printf("4. Backtracking\n");
        printf("5. Run all modes\n");
        printf("6. Exit\n");
        printf("Enter mode (1-6): ");
        scanf("%d", &mode);

        switch (mode) {
            case 1:
                printf("\nRunning Brute Force Mode...\n");
                bruteForce(n);
                break;
            case 2:
                printf("\nRunning Optimized 1 Mode...\n");
                optimized1(n);
                break;
            case 3:
                printf("\nRunning Optimized 2 Mode...\n");
                optimized2(n);
                break;
            case 4:
                printf("\nRunning Backtracking Mode...\n");
                backtrackingMode(n);
                break;
            case 5:
                printf("\nRunning All Modes...\n");
                runAllModes(n);
                break;
            case 6:
                printf("\nExiting program.\n");
                break;
            default:
                printf("\nInvalid mode selected! Please try again.\n");
        }
    } while (mode != 6);

    return 0;
}


/*
@brief Dinamik bellek ile N boyutunda bir satran� tahtas� olu�turur.
@param n tahtan�n boyutu
@return int** Tahtay� temsil eden 2D dizi d�nd�r�r.
*/
int **createBoard(int n) {
    int **board = (int **)malloc(n * sizeof(int *));
    int i;
    for (i = 0; i < n; i++) {
        board[i] = (int *)calloc(n, sizeof(int));
    }
    return board;
}

/*
@brief Satran� tahtas� i�in ayr�lan belle�i serbest b�rak�r.
@param board satran� tahtas�
@param n     tahtan�n boyutu
@return void
*/
void freeBoard(int **board, int n) {
    int i;
    for (i = 0; i < n; i++) {
        free(board[i]);
    }
    free(board);
}

/*
@brief Bir tahtadan di�erine t�m elemanlar� kopyalar.
@param source kaynak tahta
@param dest   hedef tahta
@param n      tahtan�n boyutu
@return void
*/
void copyBoard(int **source, int **dest, int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            dest[i][j] = source[i][j];
        }
    }
}

/*
@brief N-Queens tahtas�n� konsola yazd�r�r.
@param board satran� tahtas�
@param n     tahtan�n boyutu
@return void
*/
void printBoard(int **board, int n) {
    int i, j;
    printf("\nSolution %d:\n", solutionCount);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%2d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/*
@brief Brute force algoritmas� ile N-Queens problemini ��zer.
@param n tahtan�n boyutu
@return void
*/
void bruteForce(int n) {
    printf("\nBrute Force Mode Started\n");
    clock_t start = clock();

    int **board = createBoard(n);
    int totalCells = n * n;
    int *positions = (int *)malloc(n * sizeof(int));
    int i, j;

    solutionCount = 0;

    // T�m vezir yerle�imlerini ger�ekle�tir
    generateCombination(positions, n, totalCells, 0, 0, board);

    printf("\nTotal unique solutions found: %d\n", solutionCount);

    // Bellek temizle
    free(positions);
    freeBoard(board, n);

    clock_t end = clock(); // zaman� tut
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_spent); // zaman� yaz
}


/*
@brief Verilen N boyutlar�na g�re t�m ge�erli vezir kombinasyonlar�n� �retir.
@param positions vezirlerin pozisyonlar�n� tutan dizi
@param n         tahtan�n boyutu
@param totalCells toplam h�cre say�s� (N^2)
@param index     �u anki indeks
@param start     ba�lang�� indeksi
@param board     satran� tahtas�
@return void
*/
void generateCombination(int *positions, int n, int totalCells, int index, int start, int **board) {
    int i, row, col;

    if (index == n) {
        for (i = 0; i < n; i++) {
            memset(board[i], 0, n * sizeof(int));
        }

        for (i = 0; i < n; i++) {
            row = positions[i] / n;
            col = positions[i] % n;
            board[row][col] = 1;
        }

        if (isValidPlacement(board, n)) {
            solutionCount++;
            printBoard(board, n);
        }
        return;
    }

    for (i = start; i < totalCells; i++) {
        positions[index] = i;
        generateCombination(positions, n, totalCells, index + 1, i + 1, board);
    }
}

/*
@brief Bir tahtan�n ge�erli bir yerle�im i�erip i�ermedi�ini kontrol eder.
@param board satran� tahtas�
@param n     tahtan�n boyutu
@return bool E�er yerle�im ge�erliyse true, aksi takdirde false d�ner.
*/
bool isValidPlacement(int **board, int n) {
    int i, j, k;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                for (k = j + 1; k < n; k++) {
                    if (board[i][k] == 1) return false;
                }
                for (k = i + 1; k < n; k++) {
                    if (board[k][j] == 1) return false;
                }
                for (k = 1; i + k < n && j + k < n; k++) {
                    if (board[i + k][j + k] == 1) return false;
                }
                for (k = 1; i + k < n && j - k >= 0; k++) {
                    if (board[i + k][j - k] == 1) return false;
                }
            }
        }
    }
    return true;
}


/*
@brief Her sat�rda tek bir vezir yerle�tiren optimize edilmi� bir Brute Force algoritmas�. (Queens cannot share a row)
@param n tahtan�n boyutu
@return void
*/
void optimized1(int n) {
    printf("\nOptimized 1 Mode Started\n");
    clock_t start = clock();

    int **board = createBoard(n);
    int totalCells = n * n;
    int *positions = (int *)malloc(n * sizeof(int));
    int i;

    solutionCount = 0;

    // Generate combinations with the optimization that queens cannot share the same row
    generateCombinationOptimized1(positions, n, totalCells, 0, 0, board);

    printf("\nTotal unique solutions found: %d\n", solutionCount);

    free(positions);
    freeBoard(board, n);

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_spent);
}

/*
@brief Optimize edilmi� Brute Force kombinasyonlar� (Queens cannot share the same row)
@param positions vezirlerin pozisyonlar�n� tutan dizi
@param n         tahtan�n boyutu
@param totalCells toplam h�cre say�s� (N^2)
@param index     �u anki indeks
@param start     ba�lang�� indeksi
@param board     satran� tahtas�
@return void
*/
void generateCombinationOptimized1(int *positions, int n, int totalCells, int index, int start, int **board) {
    int i, j, row, col;
	
    if (index == n) {
        for (i = 0; i < n; i++) {
            memset(board[i], 0, n * sizeof(int));
        }

        for (i = 0; i < n; i++) {
            row = positions[i] / n;
            col = positions[i] % n;
            board[row][col] = 1;
        }

        if (isValidPlacement(board, n)) {
            solutionCount++;
            printBoard(board, n);
        }
        return;
    }

    for (i = start; i < totalCells; i++) {
        int currentRow = i / n;
        bool valid = true;

        for (j = 0; j < index; j++) {
            if ((positions[j] / n) == currentRow) {
                valid = false;
                break;
            }
        }

        if (valid) {
            positions[index] = i;
            generateCombinationOptimized1(positions, n, totalCells, index + 1, i + 1, board);
        }
    }
}



/*
@brief Her sat�r ve s�tunda tek bir vezir yerle�tiren optimize edilmi� bir Brute Force algoritmas�. (Queens cannot share the same row or column)
@param n tahtan�n boyutu
@return void
*/
void optimized2(int n) {
    printf("\nOptimized 2 Mode Started\n");
    clock_t start = clock();

    int **board = createBoard(n);
    int totalCells = n * n;
    int *positions = (int *)malloc(n * sizeof(int));
    int i;

    solutionCount = 0;

    // Generate combinations with the optimization that queens cannot share the same row or column
    generateCombinationOptimized2(positions, n, totalCells, 0, 0, board);

    printf("\nTotal unique solutions found: %d\n", solutionCount);

    free(positions);
    freeBoard(board, n);

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_spent);
}

/*
@brief Optimize edilmi� Brute Force kombinasyonlar� (Queens cannot share the same row or column)
@param positions vezirlerin pozisyonlar�n� tutan dizi
@param n         tahtan�n boyutu
@param totalCells toplam h�cre say�s� (N^2)
@param index     �u anki indeks
@param start     ba�lang�� indeksi
@param board     satran� tahtas�
@return void
*/
void generateCombinationOptimized2(int *positions, int n, int totalCells, int index, int start, int **board) {
    int i, j, row, col;

    if (index == n) {
        for (i = 0; i < n; i++) {
            memset(board[i], 0, n * sizeof(int));
        }

        for (i = 0; i < n; i++) {
            row = positions[i] / n;
            col = positions[i] % n;
            board[row][col] = 1;
        }

        if (isValidPlacement(board, n)) {
            solutionCount++;
            printBoard(board, n);
        }
        return;
    }

    for (i = start; i < totalCells; i++) {
        int currentRow = i / n;
        int currentCol = i % n;
        bool valid = true;

        for (j = 0; j < index; j++) {
            if ((positions[j] / n) == currentRow || (positions[j] % n) == currentCol) {
                valid = false;
                break;
            }
        }

        if (valid) {
            positions[index] = i;
            generateCombinationOptimized2(positions, n, totalCells, index + 1, i + 1, board);
        }
    }
}


/*
@brief Backtracking algoritmas� ile N-Queens problemini ��zer.
@param board satran� tahtas�
@param n     tahtan�n boyutu
@param col   yerle�tirilen s�tun
@return bool E�er ��z�m bulunursa true, aksi takdirde false d�ner.
*/
bool backtracking(int **board, int n, int col) {
    int i;

    if (col >= n) {
        printBoard(board, n);
        solutionCount++;
        return false;
    }

    for (i = 0; i < n; i++) {
        if (placeQueenCorrect(board, n, i, col)) {
            board[i][col] = 1;
            backtracking(board, n, col + 1);
            board[i][col] = 0;
        }
    }
    return false;
}

/*
@brief N-Queens tahtas�nda bir h�creye vezir yerle�tirmenin g�venli olup olmad���n� kontrol eder.
@param board satran� tahtas�
@param n     tahtan�n boyutu
@param row   kontrol edilen sat�r
@param col   kontrol edilen s�tun
@return bool E�er pozisyon g�venliyse true, aksi takdirde false d�ner.
*/
bool placeQueenCorrect(int **board, int n, int row, int col) {
    int i, j;

    for (i = 0; i < col; i++) {
        if (board[row][i]) {
            return false;
        }
    }

    for (i = row, j = col; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j]) {
            return false;
        }
    }

    for (i = row, j = col; i < n && j >= 0; i++, j--) {
        if (board[i][j]) {
            return false;
        }
    }

    return true;
}

/*
@brief Backtracking algoritmas�n� �al��t�r�r ve ��z�m� yazd�r�r.
@param n tahtan�n boyutu
@return void
*/
void backtrackingMode(int n) {
    printf("\nBacktracking Mode Started\n");
    clock_t start = clock();

    int **board = createBoard(n);
    solutionCount = 0;

    backtracking(board, n, 0);

    printf("\nTotal solutions found: %d\n", solutionCount);

    freeBoard(board, n);

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_spent);
}

/*
@brief B�t�n y�ntemleri �al��t�r�r ve her birinin sonu�lar�n� yazd�r�r. 
       Her y�ntemin �al��ma s�resi sonunda �zetlenir.
@param n tahtan�n boyutu
@return void
*/
void runAllModes(int n) {
    printf("\nRunning All Modes\n");
    printf("=================\n");

    double bruteForceTime, optimized1Time, optimized2Time, backtrackingTime;
    clock_t start, end;

    // Brute Force
    printf("\n1. Brute Force:\n");
    start = clock();
    bruteForce(n);
    end = clock();
    bruteForceTime = (double)(end - start) / CLOCKS_PER_SEC;

    // Optimized 1
    printf("\n2. Optimized 1 (Same Row Check):\n");
    start = clock();
    optimized1(n);
    end = clock();
    optimized1Time = (double)(end - start) / CLOCKS_PER_SEC;

    // Optimized 2
    printf("\n3. Optimized 2 (Same Row and Column Check):\n");
    start = clock();
    optimized2(n);
    end = clock();
    optimized2Time = (double)(end - start) / CLOCKS_PER_SEC;

    // Backtracking
    printf("\n4. Backtracking:\n");
    start = clock();
    backtrackingMode(n);
    end = clock();
    backtrackingTime = (double)(end - start) / CLOCKS_PER_SEC;

    // Summary of Times
    printf("\nSummary of Time Taken:\n");
    printf("=======================\n");
    printf("Brute Force: %.6f seconds\n", bruteForceTime);
    printf("Optimized 1: %.6f seconds\n", optimized1Time);
    printf("Optimized 2: %.6f seconds\n", optimized2Time);
    printf("Backtracking: %.6f seconds\n", backtrackingTime);
    printf("=======================\n");
}
