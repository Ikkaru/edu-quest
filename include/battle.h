#pragma once
#include "global.h"


// Initialize battle with specific stage/enemy
void InitBattle(int stage);

// Update battle logic (call every frame)
GameState UpdateBattle();

// Draw battle UI
void DrawBattleGUI();


// Internal functions
void BasicAttack();
void DrawHealthBar(int x, int y, int currentHP, int maxHP, int width, Color barColor, const char* label);
void Battle(int stage);