#ifndef PLAYER_H
#define PLAYER_H
#include "wordle.h"
typedef char*(*Player)(const char lastResult[WORD_LENGTH+1]);
char *player_AI(const char lastResult[WORD_LENGTH + 1]);
double calculate_variance(double arr[], int size);
void AIdelete(char result[], char words[MAX_WORDS][WORD_LENGTH + 1], char input[], int *wordcount);
void calculate_letter_frequencies(char words[MAX_WORDS][WORD_LENGTH + 1], double frequency[26]) ;
char AIdecision(char wordList[MAX_WORDS][WORD_LENGTH + 1], double frequency[26], char *guess);

#endif