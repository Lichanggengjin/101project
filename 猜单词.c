#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#define WORD_LENGTH 5
#define MAX_WORDS 14855//单词数量14855
#define MAX_SOLUTIONS 2315
#define MAX_ATTEMPTS 10
char wordList[MAX_WORDS][WORD_LENGTH + 1];
int wordcnt = 0;
bool firstGuess = true;
void loadWordList(const char* filename);
void check_word(const char* target, const char* input, char* res);
char* player_AI(const char* target);
void humanPlay(const char* target);
void AIPlay(const char* target);
double calculateEntropy(const char* word, char possibleAnswers[][WORD_LENGTH + 1], int answercnt);
void getPattern(const char* guess, const char* target, char* pattern);
char* getBestGuess(char wordList[][WORD_LENGTH + 1], int wordcnt,
                   char possibleAnswers[][WORD_LENGTH + 1], int answercnt);
void testAI(int numWords);

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
void getPattern(const char* guess, const char* target, char* pattern) {
	char tempTarget[WORD_LENGTH + 1];
	strcpy(tempTarget, target);

	for (int i = 0; i < WORD_LENGTH; i++) {
		if (guess[i] == target[i]) {
			pattern[i] = 'G';
			tempTarget[i] = '*';
		} else {
			pattern[i] = 'B';
		}
	}

	for (int i = 0; i < WORD_LENGTH; i++) {
		if (pattern[i] == 'B') {
			for (int j = 0; j < WORD_LENGTH; j++) {
				if (guess[i] == tempTarget[j]) {
					pattern[i] = 'Y';
					tempTarget[j] = '*';
					break;
				}
			}
		}
	}
	pattern[WORD_LENGTH] = '\0';
}

double calculateEntropy(const char* word, char possibleAnswers[][WORD_LENGTH + 1], int answercnt) {
	typedef struct {
		char pattern[WORD_LENGTH + 1];
		int count;
	} Pattern;
	Pattern patterns[243]; 
	int patterncnt = 0;
	double entropy = 0.0;
	for (int i = 0; i < answercnt; i++) {
		char pattern[WORD_LENGTH + 1];
		getPattern(word, possibleAnswers[i], pattern);
		int found = 0;
		for (int j = 0; j < patterncnt; j++) {
			if (strcmp(patterns[j].pattern, pattern) == 0) {
				patterns[j].count++;
				found = 1;
				break;
			}
		}
		if (!found) {
			strcpy(patterns[patterncnt].pattern, pattern);
			patterns[patterncnt].count = 1;
			patterncnt++;
		}
	}
	for (int i = 0; i < patterncnt; i++) {
		double probability = (double)patterns[i].count / answercnt;
		if (probability > 0) { 
			entropy -= probability * log2(probability);
		}
	}
	return entropy;
}

char* getBestGuess(char wordList[][WORD_LENGTH + 1], int wordcnt,
                   char possibleAnswers[][WORD_LENGTH + 1], int answercnt) {
	static char bestWord[WORD_LENGTH + 1];
	double maxEntropy = -1.0;
	if (answercnt == 2315) {
		strcpy(bestWord, "slate");
		return bestWord;
	}
	for (int i = 0; i < wordcnt; i++) {
		double entropy = calculateEntropy(wordList[i], possibleAnswers, answercnt);
		for (int j = 0; j < answercnt; j++) {
			if (strcmp(wordList[i], possibleAnswers[j]) == 0) {
				entropy += 0.5; 
				break;
			}
		}
		if (entropy > maxEntropy) {
			maxEntropy = entropy;
			strcpy(bestWord, wordList[i]);
		}
	}
	return bestWord;
}
char* player_AI(const char* target) {
	static char possibleAnswers[MAX_WORDS][WORD_LENGTH + 1];
	char (*source)[WORD_LENGTH + 1] = &wordList[MAX_WORDS - MAX_SOLUTIONS];
	static int answercnt = 0;
	static char currentGuess[WORD_LENGTH + 1];
	if (firstGuess) {
		memcpy(possibleAnswers, source, sizeof(char) *MAX_SOLUTIONS * (WORD_LENGTH + 1));
		answercnt = MAX_SOLUTIONS;
		firstGuess = false;
		strcpy(currentGuess, getBestGuess(wordList, wordcnt, possibleAnswers, answercnt));
	} else {
		char pattern[WORD_LENGTH + 1];
		getPattern(currentGuess, target, pattern);
		int newAnswercnt = 0;
		for (int i = 0; i < answercnt; i++) {
			char testPattern[WORD_LENGTH + 1];
			getPattern(currentGuess, possibleAnswers[i], testPattern);
			if (strcmp(pattern, testPattern) == 0) {
				if (newAnswercnt != i) {
					strcpy(possibleAnswers[newAnswercnt], possibleAnswers[i]);
				}
				newAnswercnt++;
			}
		}
		answercnt = newAnswercnt;
		strcpy(currentGuess, getBestGuess(wordList, wordcnt, possibleAnswers, answercnt));
	}
	return currentGuess;
}

//人类玩家
void humanPlay(const char* target) {
	char guess[WORD_LENGTH + 1];
	char res[WORD_LENGTH + 1];
	int att = 0;
	FILE* output = fopen("game_ress.txt", "a");

	while (att < MAX_ATTEMPTS) {
		printf("第%d次尝试): ", att + 1);
		scanf("%s", guess);

		//检查输入是否合法
		if (strlen(guess) != WORD_LENGTH) {
			printf("不合法\n");
			continue;
		}

		//检查输入是否在词库中
		int valid = 0;
		for (int i = 0; i < wordcnt; i++) {
			if (strcmp(wordList[i], guess) == 0) {
				valid = 1;
				break;
			}
		}
		if (!valid) {
			printf("不在词库中\n");
			continue;
		}

		check_word(target, guess, res);
		printf("结果: %s\n", res);
		fprintf(output, "�²� %d: %s -> %s\n", att + 1, guess, res);

		if (strcmp(res, "GGGGG") == 0) {
			printf("尝试次数为%d次\n", att + 1);
			fprintf(output, "��Ϸʤ����ʹ�� %d �β²�\n\n", att + 1);
			fclose(output);
			return;
		}
		att++;
	}

	printf("结果: %s\n", target);
	fprintf(output, "目标是: %s\n\n", target);
	fclose(output);
}

void AIPlay(const char* target) {
	int att = 0;
	char res[WORD_LENGTH + 1];
	char* guess;
	FILE* output = fopen("game_ress.txt", "a");
	firstGuess = true;  

	while (att < MAX_ATTEMPTS) {
		guess = player_AI(target);
		check_word(target, guess, res);

		printf("AI第%d次尝试: %s -> %s\n", att + 1, guess, res);

		if (strcmp(res, "GGGGG") == 0) {
			printf("AI成绩为%d \n", att + 1);
			fprintf(output, "AI成绩为 %d \n\n", att + 1);
			fclose(output);
			return;
		}
		att++;
	}
	fclose(output);
}
double calculate_average_attempts(int total_attempts, int numWords) {
    return (double)total_attempts / numWords;
}
//AI测试
void testAI(int numWords) {
    FILE *output = fopen("ai_test_ress.txt", "w"); 
    clock_t start_time = clock(); 

    int total_attempts = 0; // 用于记录总的猜测次数

    for (int i = 12540; i < numWords + 12540 && i < wordcnt; i++) { 
        char target[WORD_LENGTH + 1];
        strcpy(target, wordList[i]); 

        int att = 0;
        char res[WORD_LENGTH + 1];
        char *guess;
        firstGuess = true; 

        fprintf(output, "目标: %s\n", target);

        while (att < MAX_ATTEMPTS) {
            guess = player_AI(target);
            check_word(target, guess, res);

            fprintf(output, "第 %d次: %s -> %s\n", att + 1, guess, res);

            if (strcmp(res, "GGGGG") == 0) {
                fprintf(output, "AI尝试%d 次\n\n", att + 1);
                total_attempts += (att + 1); // 累加猜测次数
                break;
            }
            att++;
        }

        if (att == MAX_ATTEMPTS) {
            fprintf(output, "AI失败: %s\n\n", target);
            total_attempts += MAX_ATTEMPTS; // 累加最大猜测次数
        }
    }

    double average_attempts = calculate_average_attempts(total_attempts, numWords);
    fprintf(output, "平均猜测次数: %.2f\n", average_attempts);

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    fprintf(output, "总时间: %.2f 秒\n", elapsed_time);


    fclose(output);
}
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
///usr/bin/gcc -o ./game /Users/lijinshuo/Downloads/projectcopy/猜单词.c && ./game