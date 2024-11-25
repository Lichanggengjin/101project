#ifndef WORDLE_H
#define WORDLE_H

#include <stdbool.h>

#define WORD_LENGTH 5
#define MAX_ROUNDS 10
#define MAX_WORDS 15000
#define SOLUTION_LENGTH 2500

typedef char* (*Player)(const char lastResult[WORD_LENGTH + 1]);

char* checkWord(const char solution[WORD_LENGTH + 1], const char guess[WORD_LENGTH + 1]);
int wordle(const char solution[WORD_LENGTH + 1], Player *player);

bool not_legal(const char* guess);
bool invalid(const char* solution);
// You can define more functions here
// For example:
// char* player_input(const char lastResult[WORD_LENGTH + 1]);
// char* player_random(const char lastResult[WORD_LENGTH + 1]);
#endif