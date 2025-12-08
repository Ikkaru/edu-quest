#pragma once

// Game State
typedef enum {
    MAIN_MENU,
    GAMEPLAY,
    BATTLE,
    GAME_OVER
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
    int energy;
    int maxEnergy;
    int HP;
    int maxHP;
    char name[100];
} PlayerData;

extern PlayerData player;

