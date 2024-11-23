#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "wordle.h"
void check_word(const char* target, const char* input, char* res) {
	char tempTarget[WORD_LENGTH + 1];
	strcpy(tempTarget, target);
	for (int i = 0; i < WORD_LENGTH; i++) {
		if (input[i] == target[i]) {
			res[i] = 'G';
			tempTarget[i] = '*';
		} else {
			res[i] = 'B';
		}
	}
	for (int i = 0; i < WORD_LENGTH; i++) {
		if (res[i] == 'B') {
			for (int j = 0; j < WORD_LENGTH; j++) {
				if (input[i] == tempTarget[j]) {
					res[i] = 'Y';
					tempTarget[j] = '*';
					break;
				}
			}
		}
	}
	res[WORD_LENGTH] = '\0';
}
void loadWordList(const char* filename) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		printf("打不开: %s\n", filename);
		exit(1);
	}

	while (wordcnt < MAX_WORDS &&
	        fscanf(file, "%s", wordList[wordcnt]) == 1) {
		if (strlen(wordList[wordcnt]) == WORD_LENGTH) {
			wordcnt++;
		}
	}
	fclose(file);
	printf("文件中有%d个单词 \n", wordcnt);
}
double calculate_average_attempts(int total_attempts, int numWords) {
    return (double)total_attempts / numWords;
}
void testAI(int numWords) {
    clock_t start_time = clock(); 

    int total_attempts = 0; 

    for (int i = 13540; i < numWords + 13540 && i < wordcnt; i++) { 
        char target[WORD_LENGTH + 1];
        strcpy(target, wordList[i]); 

        int att = 0;
        char res[WORD_LENGTH + 1];
        char *guess;
        firstGuess = true; 

        while (att < MAX_ATTEMPTS) {
            guess = player_AI(target);
            check_word(target, guess, res);

            if (strcmp(res, "GGGGG") == 0) {
                total_attempts += (att + 1); // 累加猜测次数
                break;
            }
            att++;
        }

        if (att == MAX_ATTEMPTS) {
            total_attempts += MAX_ATTEMPTS; // 累加最大猜测次数
			printf("AI无法猜测: %s\n", target);
			
        }
    }

    double average_attempts = calculate_average_attempts(total_attempts, numWords);
    printf("平均猜测次数: %.2f\n", average_attempts);

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("总时间: %.2f 秒\n", elapsed_time);
}
