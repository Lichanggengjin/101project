#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "wordle.h"
int main() {
    char words[MAX_WORDS][WORD_LENGTH + 1]; 
    int word_count;
    const char *filename = "wordList.txt";
    char target[WORD_LENGTH + 1]; 
    char input[WORD_LENGTH + 1];
    char result[WORD_LENGTH + 1];
    int attempts = 0;

    if (!load_commands(filename, words, &word_count)) {
        return 1; 
    }
    srand(time(NULL)); 
    if (!read_random_word(filename, target)) {
        return 1; 
    }

    printf("猜单词游戏开始！\n");
    printf("请选择自动进行还是手动游玩\n");
    printf("1. 自动\n");
    printf("2. 手动\n");
    int xuanze;
    scanf("%d", &xuanze);
    while ((getchar()) != '\n' && getchar() != EOF); 

    if (xuanze != 2) {
        printf("自动进行在本part1中不需要，请开启part2获得全部游戏体验\n");
        return 0;
    }

    while (1) {
        printf("尝试次数: %d\n", attempts + 1);
        printf("请输入一个5个字母的单词:\n");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if (is_word_in_list(input, words, word_count) == false) {
            printf("不是有效的单词\n");
            continue;
        }

        check_word(target, input, result); 
        printf("结果: %s\n", result); 

        if (strcmp(target, input) == 0) {
            printf("猜对了！\n");
            printf("分数为: %d\n", MAX_ATTEMPTS - attempts);
            break;
        }

        attempts++;
        if (attempts >= MAX_ATTEMPTS) {
            printf("达到最大尝试次数，正确单词是: %s 分数为0\n", target);
            break;
        }
    }

    return 0;
}
