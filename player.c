#include "player.h"
#include "wordle.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#define GUESS_COUNT 12546
#define WORD_COUNT 2309
extern char wordList[MAX_WORDS][WORD_LENGTH+1];
int result[GUESS_COUNT][WORD_COUNT];
double calculate_variance(double arr[], int size) {
    double sum = 0.0, mean, variance = 0.0;

    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    mean = sum / size;
    for (int i = 0; i < size; i++) {
        variance += pow(arr[i] - mean, 2);
    }
    return variance / size;
}
void AIdelete(char result[], char words[MAX_WORDS][WORD_LENGTH + 1], char input[], int *wordcount) {
    int wordCount = *wordcount;
    if (strlen(result) != WORD_LENGTH || strlen(input) != WORD_LENGTH) {
        return; 
    }
    int inputLetterCount[26] = {0};
    for (int i = 0; i < WORD_LENGTH; i++) {
        inputLetterCount[input[i] - 'A']++;
    }

    for (int i = 0; i < WORD_LENGTH; i++) {
        char inputChar = input[i];
        char resChar = result[i];

        if (resChar == 'G') {
            for (int j = 0; j < wordCount; j++) {
                if (words[j][i] != inputChar) {
                    words[j][0] = '\0';
                }
            }
        } else if (resChar == 'Y') {
            for (int j = 0; j < wordCount; j++) {
                if (!strchr(words[j], inputChar) || words[j][i] == inputChar) {
                    words[j][0] = '\0';
                }
            }
        } else if (resChar == 'B') {
            int remainingCount = inputLetterCount[inputChar - 'A'];

            for (int j = 0; j < wordCount; j++) {
                int wordCountChar = 0;
                for (int k = 0; k < WORD_LENGTH; k++) {
                    if (words[j][k] == inputChar) {
                        wordCountChar++;
                    }
                }
                if (wordCountChar >= remainingCount) {
                    words[j][0] = '\0';
                }
            }
        }
    }

    if (strcmp(result, "GGGGG") != 0) {
        for (int j = 0; j < wordCount; j++) {
            if (strcmp(words[j], input) == 0) {
                words[j][0] = '\0';
                break;
            }
        }
    }

    int validIndex = 0;
    for (int i = 0; i < wordCount; i++) {
        if (words[i][0] != '\0') {
            if (validIndex != i) {
                strcpy(words[validIndex], words[i]);
            }
            validIndex++;
        }
    }
    for (int i = validIndex; i < wordCount; i++) {
        words[i][0] = '\0';
    }

    *wordcount = validIndex;
}

void calculate_letter_frequencies(char words[MAX_WORDS][WORD_LENGTH + 1], double frequency[26]) {
    int letter_count[26] = {0};
    int total_letters = 0;
    for (int i = 0; i < MAX_WORDS && words[i][0] != '\0'; i++) {
        for (int j = 0; j < WORD_LENGTH; j++) {
            char c = words[i][j];
            if (c >= 'a' && c <= 'z') {
                letter_count[c - 'a']++;
                total_letters++;
            } else if (c >= 'A' && c <= 'Z') {
                letter_count[c - 'A']++;
                total_letters++;
            }
        }
    }
    for (int i = 0; i < 26; i++) {
        if (letter_count[i] > 0) {
            frequency[i] = (double)letter_count[i] / total_letters * 100.0;
        } else {
            frequency[i] = 0.0;
        }
    }
}
char AIdecision(char wordList[MAX_WORDS][WORD_LENGTH + 1], double frequency[26], char *guess) {
    double max_score = -1.0;
    int best_word_index = -1;
    double min_variance = INFINITY;

    for (int i = 0; i < MAX_WORDS && wordList[i][0] != '\0'; i++) {
        double score = 0.0;
        double letter_frequencies[WORD_LENGTH] = {0.0};

        for (int j = 0; j < WORD_LENGTH; j++) {
            char c = wordList[i][j];
            if (c >= 'a' && c <= 'z') {
                score += frequency[c - 'a'];
                letter_frequencies[j] = frequency[c - 'a'];
            } else if (c >= 'A' && c <= 'Z') {
                score += frequency[c - 'A'];
                letter_frequencies[j] = frequency[c - 'A'];
            }
        }

        if (score > max_score) {
            max_score = score;
            best_word_index = i;
            min_variance = calculate_variance(letter_frequencies, WORD_LENGTH);
        } else if (score == max_score) {
            double variance = calculate_variance(letter_frequencies, WORD_LENGTH);
            if (variance < min_variance) {
                best_word_index = i;
                min_variance = variance;
            }
        }
    }

    if (best_word_index != -1) {
        strcpy(guess, wordList[best_word_index]);
    } else {
        guess[0] = '\0';
    }

    return *guess;
}
char *player_AI(const char lastResult[WORD_LENGTH+1]){
    char* guess = NULL;
    char firstGuess[WORD_LENGTH+1] = "salet";
    char jieguo[WORD_LENGTH+1];
    jieguo[5]='\0';
    double frequency[26]={0};
    int wordcount =14855;
    check_word(firstGuess,lastResult,jieguo);
    AIdelete(jieguo,wordList,firstGuess,&wordcount);
    while (strcmp(jieguo, "GGGGG") != 0){
        calculate_letter_frequencies(wordList,frequency);
        AIdecision(wordList,frequency,guess);
        check_word(guess,lastResult,jieguo);
        AIdelete(jieguo,wordList,guess,&wordcount);
    }
    return guess;
}
