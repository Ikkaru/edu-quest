#pragma once
#include "global.h"


// Initialize battle with specific stage/enemy
void InitBattle(int stage);

// Update battle logic (call every frame)
GameState UpdateBattle();

// Draw battle UI
void DrawBattleGUI();
void DrawQuizInterface(int screenW, int screenH);

// Internal functions
void DrawHealthBar(int x, int y, int currentHP, int maxHP, int width, Color barColor, const char* label);
void ExecutePlayerDamage(bool iscorrect);
void ExecuteEnemyDamage(bool iscorrect);
bool IsAnswerCorrect();
void DrawTurnIndicator();
void UnloadBattle();
void QuestionGeneration();


// Floating Damage Text System
typedef struct
{
    char text[20];
    float x, y;
    float alpha;
    float lifetime;
    Color color;
    bool active;
} DamageText;

extern DamageText damageText;
