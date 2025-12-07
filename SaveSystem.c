#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SaveSystem.h"
#define SAVEDIR "history/"

void saveScore(const char* player_name, int score) {
    char filePath[1000];

    // Concatenation File Path with player Name
    sprintf(filePath, "%s%s.txt", SAVEDIR, player_name);

    // Open the file 
    FILE *file = fopen(filePath, "a");
    printf("[SaveSystem] Opening %s\n", filePath);

    if (file != NULL) {
        fprintf(file, "%d\n", score);
        fclose(file);
        printf("[SaveSystem] Score %d saved for %s to %s\n", score, player_name, filePath);
    } else {
        printf("[SaveSystem] Error: Failed to Find Save Folder\n");
    }   
}

playerHistory loadScore(const char* player_name) {
    playerHistory history;

    history.count = 0;
    history.scores =  NULL;

    // Check the save score file
    char filePath[1000];
    sprintf(filePath, "%s%s.txt", SAVEDIR, player_name);
    FILE *file = fopen(filePath, "r");

    if (file != NULL) {
        int readScore;
        // Count for the saved score
        while (fscanf(file, "%d", &readScore) == 1) {
            history.count++;
        }

        rewind(file);

        // Allocate Memmory for score buffer
        history.scores = (int*)malloc(sizeof(int) * history.count);

        // Input Score data
        for (int i = 0; i < history.count; i++) {
            fscanf(file, "%d", &history.scores[i]);
        }
        
        fclose(file);

        return history;
    } else {
        printf("Cannot Find Saved File Try to Create one");
    }
}


int main() {
    playerHistory player = loadScore("ikhsan");

    for (int i = 0; i < player.count; i++) {
        printf("%d\n", player.scores[i]);
    }

    free(player.scores);
}

