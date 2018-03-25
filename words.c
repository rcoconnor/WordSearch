//words.c
//by: Rory O'Connor
//this is a program which takes a size and file from the
//command line, and then generates a wordsearch of that
//size using the contents from that file
//it then prompts the user for a word, and then searches
//the wordsearch for that word, printing out where they
//are at the end of it
//it uses four functions in order to scan the wordsearch
//for the word, one for each direction

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void fromFileLoadArray(FILE* inputFile, char** matrix, int matrixSize);
void printArray(char** matrix, int matrixSize);
void clearArray(char** matrix, int matrixSize);
void reverseString(char userWord[]);
int findWordLeftToRight(char userWord[], char** matrix, int matrixSize, char** emptyArray);
int findWordRightToLeft(char userWord[], char** matrix, int matrixSize, char** emptyArray);
int findWordTopToBottom(char userWord[], char** matrix, int matrixSize, char** emptyArray);
int findWordBottomToTop(char userWord[], char** matrix, int matrixSize, char** emptyArray);


int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("ERROR: NOT ENOUGH ARGUEMENTS\n");
        return -1;
    }
    
    int matrixSize = atoi(argv[1]);
    FILE* inputFile = fopen(argv[2], "r");
    
    //check if the file exists
    if (inputFile == NULL) {
        printf("ERROR: can't find file\n");
        return -1;
    }
    
    //allocate memory for rows
    char **matrix = malloc(sizeof(char*) * matrixSize);
    if (matrix == NULL) {
        printf("Out of memory\n");
        return -1;
    }
    
    //allocate memory for columns
    for (int r = 0; r < matrixSize; r++) {
        matrix[r] = malloc(sizeof(char) * matrixSize);
        if(matrix[r] == NULL) {
            printf("Out of memory for row %d\n", r);
            return -1;
        }
    }
    
    //allocate memory for rows
    char **emptyArray = malloc(sizeof(char*) * matrixSize);
    if (emptyArray == NULL) {
        printf("Out of memory for empty array\n");
        return -1;
    }

    //allocate memory for columns
    for (int r = 0; r < matrixSize; r++) {
        emptyArray[r] = malloc(sizeof(char) * matrixSize);
        if (matrix[r] == NULL) {
            printf("Out of memory for empty array in row %d\n", r);
            return -1;
        }
    }
    
    clearArray(emptyArray, matrixSize); 
    
    
    
    fromFileLoadArray(inputFile, matrix, matrixSize);
    printArray(matrix, matrixSize);
    
    //ask them repeatedley to find a word
    char userWord[100];
    printf("Enter a word to find in the puzzle: ");
    scanf("%s", userWord);
    while (strcmp(userWord, "zzz") != 0) {
        int numTimesLR = findWordLeftToRight(userWord, matrix, matrixSize, emptyArray);
        int numTimesRL = findWordRightToLeft(userWord, matrix, matrixSize, emptyArray);
        int numTimesTB = findWordTopToBottom(userWord, matrix, matrixSize, emptyArray);
        int numTimesBT = findWordBottomToTop(userWord, matrix, matrixSize, emptyArray);
        
        int total = numTimesLR + numTimesRL + numTimesTB + numTimesBT;
        printf("\nThe word %s was found %d times in total\n", userWord, total);
        printf("%d times left-to-right\n", numTimesLR);
        printf("%d times right-to-left\n", numTimesRL);
        printf("%d times top-to-bottom\n", numTimesTB);
        printf("%d times bottom-to-top\n", numTimesBT);
        printf("\n\n");
        printArray(emptyArray, matrixSize);
        clearArray(emptyArray, matrixSize);
        printf("\n\n");
        
        printf("Enter a word to find in the puzzle: ");
        scanf("%s", userWord);
    }
    
    for (int i = 0; i < matrixSize; i++) free(matrix[i]);
    for (int i = 0; i < matrixSize; i++) free(emptyArray[i]);
    free(matrix);
    free(emptyArray);
    fclose(inputFile);
    
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void reverseString(char userWord[])
{
    int min = 0;
    int max = strlen(userWord) - 1;
    char tempvar;
    
    while(max > min) {
        tempvar = userWord[min];
        userWord[min] = userWord[max];
        userWord[max] = tempvar;
        min++;
        max--;
    }
    
}

void clearArray(char** matrix, int matrixSize)
{
    for (int r = 0; r < matrixSize; r++) {
        for (int c = 0; c < matrixSize; c++) {
            matrix[r][c] = '.';
        }
    }
}

void fromFileLoadArray(FILE* inputFile, char** matrix, int matrixSize)
{
    for (int r = 0; r < matrixSize; r++) {
        for (int c = 0; c < matrixSize; c++) {
            fscanf(inputFile, " %c", &matrix[r][c]);
        }
    }
}

void printArray(char** matrix, int matrixSize)
{
    for (int r = 0; r < matrixSize; r++) {
        printf("      ");
        for (int c = 0; c < matrixSize; c++) {
            printf("%c", matrix[r][c]);
        }
        printf("\n");
    }
}

int findWordLeftToRight(char userWord[], char** matrix, int matrixSize, char** emptyArray)
{
    bool isWord = false;
    int numTimes = 0;
    
    //iterate through loop
    for (int r = 0; r < matrixSize; r++) {
        for (int c = 0; c < matrixSize; c++) {
            isWord = true;
            for (int i = 0; i < strlen(userWord); i++) {
                if (c + i >= matrixSize) {
                    isWord = false;
                    break;
                }
                if(userWord[i] != matrix[r][c+i]) {
                    isWord = false;
                    break;
                }
            }
            
            if (isWord == true) {
                for (int i = 0; i < strlen(userWord); i++) {
                    emptyArray[r][c+i] = userWord[i];
                }
                numTimes++;
            }
        }
    }
    return numTimes;
}

int findWordRightToLeft(char userWord[], char** matrix, int matrixSize, char**emptyArray)
{
    int numTimes = 0;
    bool isWord = false;
    char reverseWord[strlen(userWord)+1];
    strcpy(reverseWord, userWord);
    reverseString(reverseWord);
    
    for (int r = 0; r < matrixSize; r++) {
        for (int c = 0; c < matrixSize; c++) {
            isWord = true;
            for (int i = 0; i < strlen(reverseWord); i++) {
                if (c + i >= matrixSize) {
                    isWord = false;
                    break;
                }
                if (reverseWord[i] != matrix[r][c+i]) {
                    isWord = false;
                    break;
                }
            }
            if (isWord == true) {
                for (int i = 0; i < strlen(reverseWord); i++) {
                    emptyArray[r][c+i] = reverseWord[i];
                }
                numTimes++;
            }
        }
    }
    return numTimes;
}

int findWordTopToBottom(char userWord[], char** matrix, int matrixSize, char** emptyArray)
{
    bool isWord = false;
    int numTimes = 0;
    
    for (int c = 0; c < matrixSize; c++) {
        for (int r = 0; r < matrixSize; r++) {
            isWord = true;
            for (int i = 0; i < strlen(userWord); i++) {
                if (r+i >= matrixSize) {
                    isWord = false;
                    break;
                }
                if (userWord[i] != matrix[r+i][c]) {
                    isWord = false;
                    break;
                }
            }
            if (isWord == true) {
                for (int i = 0; i < strlen(userWord); i++) {
                    emptyArray[r+i][c] = userWord[i];
                }
                numTimes++;
            }
        }
    }
    return numTimes;
}

int findWordBottomToTop(char userWord[], char** matrix, int matrixSize, char** emptyArray)
{
    int numTimes = 0;
    bool isWord = false;
    char reverseWord[strlen(userWord)+ 1];
    strcpy(reverseWord, userWord);
    reverseString(reverseWord);
    
    for (int c = 0; c < matrixSize; c++) {
        for (int r = 0; r < matrixSize; r++) {
            isWord = true;
            for (int i = 0; i < strlen(reverseWord); i++) {
                //solution
                if (r+i >= matrixSize) {
                    isWord = false;
                    break;
                }
                //end solution
                if (reverseWord[i] != matrix[r+i][c]) {
                    isWord = false;
                    break;
                }
            }
            if (isWord == true) {
                for (int i = 0; i < strlen(reverseWord); i++) {
                    emptyArray[r+i][c] = reverseWord[i];
                }
                numTimes++;
            }
        }
    }
    return numTimes;
}
