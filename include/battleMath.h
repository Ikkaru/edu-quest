#pragma once
#include "global.h"

// ==================== BATTLE FUNCTIONS ====================

// Initialize battle with specific stage/enemy
void InitBattle(int stage);

// Update battle logic (call every frame)
GameState UpdateBattle();

// Draw battle UI
void DrawBattleUI();

// Internal functions
void StartPlayerQuiz(PlayerChoice choice);
void ProcessPlayerAnswer();
void StartEnemyTurn();
void ProcessDefenseAnswer();
int GetDamageForChoice(PlayerChoice choice);
