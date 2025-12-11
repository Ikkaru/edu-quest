#include "global.h"
#include "MathQuest.h"
#include "animation.h"
#include "battle.h"
#include "raygui.h"
#include <stdbool.h>
#include <stdio.h>  

// Variabel Soal 
Question question;

// Konstanta Timer
const float QUESTION_TIME_LIMIT = 20.0f;
float timer = 0.0f;

BattleState currentBattleState = BATTLE_PLAYER_CHOICE;

PlayerChoice playerChoice;  


void Battle(int stage) {
    currentEnemy = enemies[stage - 1];
    float dt = GetFrameTime();

    // State Check
    switch (currentBattleState) {
        case BATTLE_PLAYER_CHOICE:
            switch (playerChoice)
            {
            case BASIC_ATTACK:
                currentBattleState = BATTLE_PLAYER_QUIZ;
                BasicAttack();
                break;
            
            case SKILL:
                break;
            }
    } 
    // Update Player Annimation
    UpdatePlayerAnimation(dt);
    // UpdateEnemyAnimation(&currentEnemy, deltaTime);
    UpdateEnemyAnimation(&currentEnemy, dt);
}

void BasicAttack() {
    question = generateQuestion(player.stage);
    playerChoice = BASIC_ATTACK;
    // Logic for asking question and processing answer
}

// DRAW BATTLE GUI CORE
void DrawBattleGUI() { 
    // Gui Style
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 3); 

    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();


    ClearBackground(BLACK);
    // Draw Player Sprite
    DrawPlayerSprite(200, 80, 2.0f);
    char hpLabel[100];
    DrawHealthBar(100, 410, player.HP, player.maxHP, 200, GREEN, TextFormat("%s HP", player.name));

    // Draw Enemy Sprite
    DrawEnemySprite(&currentEnemy, 800, 80, 2.0f);

    // Draw Current Energy
    DrawText(TextFormat("Energy: %d / %d", player.energy, player.maxEnergy), 20, 20, 20, YELLOW);
    DrawHealthBar(850, 410, currentEnemy.HP, currentEnemy.maxHP, 200, RED, TextFormat("%s HP", currentEnemy.name));

    switch (currentBattleState)
    {
    case BATTLE_PLAYER_CHOICE:
        int dialogW = 1100;
        int dialogH = 150;
        Rectangle dialogRec = { (screenW - dialogW) / 2, 450 , dialogW, dialogH };

        GuiGroupBox(dialogRec, NULL);

        DrawText("Your Turn", dialogRec.x + 120, dialogRec.y + 60, 30, WHITE);

        break;
    
    default:
        break;
    }
}

// Function to draw health bars
void DrawHealthBar(int x, int y, int currentHP, int maxHP, int width, Color barColor, const char* label) {
    // Bar Background (Abu-abu)
    DrawRectangle(x, y, 200, 20, LIGHTGRAY);
    DrawRectangleLines(x, y, 200, 20, DARKGRAY); // Bar Border

    if (currentHP < 0.3 * maxHP) {
        // Bar Value (Merah)
        DrawRectangle(x, y, (int)(200 * ((float)currentHP / (float)maxHP)), 20, RED);
    }
    else {
        // Bar Value (Hijau)
        float hpPercent = (float)currentHP / (float)maxHP;
        DrawRectangle(x, y, (int)(200 * hpPercent), 20, barColor);
    }

    // Text Label
    DrawText(label, x, y - 30, 20, WHITE);
    DrawText(TextFormat("%d / %d", currentHP, maxHP), x + 210, y, 20, WHITE);
}