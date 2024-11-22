#ifndef WORDLE_H
#define WORDLE_H
#define WORD_LENGTH 5
#define MAX_WORDS 14855
#define ANSWER_COUNT 2309   
#define MAX_ATTEMPTS 10      
#define MAX_LINE_LENGTH 10
void check_word(const char *target, const char *input, char *result) ;
bool read_random_word(const char *filename, char *random_word) ;
bool is_word_in_list(const char *word, char words[][WORD_LENGTH + 1], int word_count) ;
bool load_commands(const char *filename, char words[][WORD_LENGTH + 1], int *word_count) ;
#endif 