#include "wordle.h"
#include "player.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINES 1000
#define FILE_NAME "solutionList.txt"

int main() {
    char solutions[MAX_LINES][WORD_LENGTH + 1];
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    int count = 0;
    while (count < MAX_LINES && fscanf(file, "%s", solutions[count]) == 1) {
        count++;
    }
    fclose(file);

    Player player = player_AI;
    int solution = 0;
    for (int i = 0; i < count; i++) {
        solution +=wordle(solutions[i], &player);
        printf("Solution: %d\n", solution);
    }

    return 0;
}
///usr/bin/gcc -o ./game /Users/lijinshuo/Downloads/test2/main.c /Users/lijinshuo/Downloads/test2/player.c /Users/lijinshuo/Downloads/test2/wordle.c && ./game