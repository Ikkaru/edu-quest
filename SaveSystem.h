#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

// Struct untuk data player ketika di load
typedef struct
{
    int *scores;
    int count;
} playerHistory;

// Protoype untuk menyimpan skor
void saveScore(const char* player_name, char* mode, int score);

// Prototype untuk load skor
playerHistory loadScore(const char* player_name, char* mode);

#endif