#ifndef WORDLE_H
#define WORDLE_H
#include <stdbool.h>
#define WORD_LENGTH 5
#define MAX_WORDS 14855
#define MAX_SOLUTIONS 2315
#define MAX_ATTEMPTS 10
char wordList[MAX_WORDS][WORD_LENGTH + 1];
int wordcnt = 0;
bool firstGuess = true;
void loadWordList(const char* filename);
void check_word(const char* target, const char* input, char* res) ;
void testAI(int numWords);
double calculate_average_attempts(int total_attempts, int numWords);
#endif 