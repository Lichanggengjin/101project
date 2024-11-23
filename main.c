#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "wordle.h"
#include "player.h"
int main() {
	
	
	loadWordList("wordlist.txt");
    int numWordsToTest = 1000;
    testAI(numWordsToTest);//测试AI
	printf("Wordle游戏开始!\n");
	printf("1. 人类玩家\n2. AI玩家\n");
	int cho;
	scanf("%d", &cho);

	char target[WORD_LENGTH + 1];
	printf("1.手动答案？\n2.自动答案？\n");
	int tar_Cho;
	scanf("%d", &tar_Cho);

	if (tar_Cho == 1) {
		printf("请输入设置答案: ");
		scanf("%s", target);
		int valid = 0;
		for (int i = 12540; i < wordcnt; i++) {
			if (strcmp(wordList[i], target) == 0) {
				valid = 1;
				break;
			}
		}
		if (!valid) {
			printf("该答案不存在\n");
			return 1;
		}
	} else {
		srand(time(NULL));
		int randomIndex = 12540 + rand() % 2316;
		strcpy(target, wordList[randomIndex]);
		printf("随机答案完成\n");
	}

	if (cho == 1) {
		printf("人类玩家开始\n");
		humanPlay(target);
	} else if (cho == 2) {
		printf("AI玩家开始\n");
		AIPlay(target);
	} else {
		printf("无效输入\n");
		return 1;
	}

	return 0;
}
///usr/bin/gcc -o ./main /Users/lijinshuo/Downloads/projectcopy/main.c /Users/lijinshuo/Downloads/projectcopy/wordle.c  /Users/lijinshuo/Downloads/projectcopy/player.c  && ./main