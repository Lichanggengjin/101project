#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "wordle.h"
void check_word(const char *target, const char *input, char *result) {
    bool used[WORD_LENGTH] = {false};  
    memset(result, 'B', WORD_LENGTH); 
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (input[i] == target[i]) {
            result[i] = 'G'; 
            used[i] = true;  
        }
    }
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (result[i] == 'G') continue; 
        for (int j = 0; j < WORD_LENGTH; j++) {
            if (!used[j] && input[i] == target[j]) {
                result[i] = 'Y'; 
                used[j] = true;  
                break;
            }
        }
    }
}

bool read_random_word(const char *filename, char *random_word) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("无法打开文件");
        return false;
    }

    int line_count = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        // 去除换行符和空白字符
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) > 0) {
            line_count++;
        }
    }

    if (line_count < ANSWER_COUNT) {
        printf("文件中的行数少于 %d 行\n", ANSWER_COUNT);
        fclose(file);
        return false;
    }

    srand(time(NULL)); 
    int random_line_number = line_count - ANSWER_COUNT + (rand() % ANSWER_COUNT);
    printf("文件总行数: %d\n", line_count);
    printf("随机选择的行号: %d\n", random_line_number);

    fseek(file, 0, SEEK_SET);
    int current_line = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        // 去除换行符和空白字符
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) {
            continue;
        }
        if (current_line == random_line_number) {
            // 去除所有空白字符
            int j = 0;
            for (int i = 0; i < strlen(line); i++) {
                if (line[i] != ' ' && line[i] != '\t') {
                    random_word[j++] = line[i];
                }
            }
            random_word[j] = '\0'; 

            break;
        }
        current_line++;
    }
    fclose(file);
    printf("读取的单词: '%s'\n", random_word);

    return true;
}

bool is_word_in_list(const char *word, char words[][WORD_LENGTH + 1], int word_count) {
    for (int i = 0; i < word_count; i++) {
        if (strncmp(word, words[i], WORD_LENGTH) == 0) {
            return true;
        }
    }
    return false;
}
bool load_commands(const char *filename, char words[][WORD_LENGTH + 1], int *word_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("无法打开文件");
        return false;
    }
    *word_count = 0;
    while (fscanf(file, "%5s", words[*word_count]) == 1) {
        (*word_count)++;
    }
    fclose(file);
    return true;
}
