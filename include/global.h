#pragma once

// Game State
typedef enum {
    MAIN_MENU,
    GAMEPLAY,
    LOBBY,
    SPECIAL_EVENT,
    BATTLE,
    GAME_OVER,
    EXIT
} GameState;

// Game Mode
typedef enum {
    MATH,
    LOGIC,
} GameMode;


// Gamedata
typedef struct
{   
    GameMode currentMode;
    int score; 
    int energy;
    int maxEnergy;
    int HP;
    int maxHP;
    char name[101];
} PlayerData;

extern PlayerData player;
extern GameState currentState;

