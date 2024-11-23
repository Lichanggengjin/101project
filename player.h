#ifndef PLAYER_H
#define PLAYER_H
#include "wordle.h"
char* player_AI(const char* target);
void humanPlay(const char* target);
void AIPlay(const char* target);
double calculateEntropy(const char* word, char possibleAnswers[][WORD_LENGTH + 1], int answercnt);
void getPattern(const char* guess, const char* target, char* pattern);
char* getBestGuess(char wordList[][WORD_LENGTH + 1], int wordcnt,char possibleAnswers[][WORD_LENGTH + 1], int answercnt);
#endif