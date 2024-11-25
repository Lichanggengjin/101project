#include "wordle.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

char wordList[MAX_WORDS][WORD_LENGTH + 1];
int wordCount = 0;
char solutionList[SOLUTION_LENGTH][WORD_LENGTH + 1];
int solutionCount = 0;

char* checkWord(const char solution[WORD_LENGTH + 1], const char guess[WORD_LENGTH + 1]) {
    char* match_g = (char*)malloc((WORD_LENGTH + 1) * sizeof(char));
    char matched_solution[WORD_LENGTH] = {0};
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] == solution[i]) {
            match_g[i] = 'G';
            matched_solution[i] = 'G';
        } else {
            match_g[i] = 'B';
        }
    }
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (match_g[i] == 'B') {
            for (int j = 0; j < WORD_LENGTH; j++) {
                if (matched_solution[j] == 0 && guess[i] == solution[j]) {
                    match_g[i] = 'Y';
                    matched_solution[j] = 'Y';
                    break;
                }
            }
        }
    }
    match_g[WORD_LENGTH] = '\0';
    return match_g;
}

void loadWords(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file1111");
        exit(EXIT_FAILURE);
    }
    char buffer[WORD_LENGTH + 2]; // +2 for newline and null terminator
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        wordList[wordCount][WORD_LENGTH] = '\0'; // Null terminator
        strcpy(wordList[wordCount], buffer);
        wordCount++;
        if (wordCount >= MAX_WORDS) {
            break;
        }
    }
    fclose(file);
}

void loadSolution(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    char buffer[WORD_LENGTH + 2]; // +2 for newline and null terminator
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        solutionList[solutionCount][WORD_LENGTH] = '\0'; // Null terminator
        strcpy(solutionList[solutionCount], buffer);
        solutionCount++;
        if (solutionCount >= SOLUTION_LENGTH) {
            break;
        }
    }
    fclose(file);
}

int wordle(const char solution[WORD_LENGTH + 1], Player *player) {
    if (wordCount == 0) {
        loadWords("wordList.txt");
    }
    if (solutionCount == 0) {
        loadSolution("solutionList.txt");
    }
    if (invalid(solution)) {
        return 0;
    }
    char* lastResult = (char*)malloc(WORD_LENGTH * sizeof(char));
    for (int i = 0; i < WORD_LENGTH; i++) {
        lastResult[i] = ' ';
    }
    lastResult[WORD_LENGTH] = '\0';
    int i;
    for (i = 0; i < MAX_ROUNDS; i++) {
        char* guess = (*player)(lastResult);
        while (not_legal(guess)) {
            guess = (*player)(lastResult);
        }
        printf("%s\n", guess);
        char* result = checkWord(solution, guess);
        if (strcmp(solution, guess) == 0) {
            break;
        }
        free(guess);
        free(lastResult);
        lastResult = result;
    }
    if (i == MAX_ROUNDS) {
        return 0;
    } else {
        return 10 - i;
    }
    free(lastResult);
}

bool not_legal(const char* guess) {
    for (int i = 0; i < wordCount; i++) {
        if (strcmp(wordList[i], guess) == 0) {
            return false;
        }
    }
    return true;
}

bool invalid(const char* solution) {
    for (int i = 0; i < SOLUTION_LENGTH; i++) {
        if (strcmp(solution, solutionList[i]) == 0) {
            return false;
        }
    }
    return true;
}