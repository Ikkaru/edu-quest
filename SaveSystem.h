#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

// Struct untuk data player ketika di load
typedef struct
{
    int *scores;
    int count;
    int capacity;
} playerHistory;

// Protoype untuk menyimpan skor
void saveScore(const char* player_name, int score);

// Prototype untuk load skor
playerHistory loadScore(const char* player_name);

#endif