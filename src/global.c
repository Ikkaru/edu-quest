#include "global.h"
#define MAX_ENEMIES 10

PlayerData player = {
    .maxHP = 100,
    .HP = 100,
    .energy = 3,
    .maxEnergy = 3,
    .score = 100,
    .stage = 1
};

Enemy enemies[MAX_ENEMIES] = {
    {"Slime", 80, 80, 30},
    {"FireSlime", 80, 80, 35},
    {"Plant", 80, 80, 40},
    {"Orc", 100, 100, 40},
    {"HighOrc", 120, 120, 45},
    {"Skeleton", 100, 100, 50},
    {"Werewolf", 150, 150, 60},
    {"FireWizard", 200, 200, 70},
    {"DarkWizard", 250, 250, 80},
    {"Minotaur", 300, 300, 100}
};

Enemy currentEnemy;


