#include "global.h"
#define MAX_ENEMIES 10

PlayerData player = {
    .maxHP = 100,
    .HP = 100,
    .energy = 3,
    .maxEnergy = 3,
    .score = 0,
    .stage = 0,
    .currentMode = MATH
};

Enemy enemies[MAX_ENEMIES] = {
    {"Slime", 50, 50, 20},
    {"Goblin", 80, 80, 25},
    {"Plant", 80, 80, 27},
    {"Water Slime", 100, 100, 28},
    {"High Goblin", 120, 120, 27},
    {"Vampire", 100, 100, 29},
    {"Fire Plant", 130, 130, 27},
    {"Fire Slime", 150, 150, 28},
    {"Empror Goblin", 250, 250, 29},
    {"Demon King", 300, 300, 30}
};

Enemy currentEnemy;

