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

int selectedOption = 0;

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
        
        // Dialog Box
        int dialogWidth = 1100;
        int dialogHeight = 140;
        Rectangle dialogRec = { (screenW - dialogWidth) / 2, 450 , dialogWidth, dialogHeight };

        GuiGroupBox(dialogRec, NULL);

        DrawText("Your Turn", dialogRec.x + 100, dialogRec.y + 60, 30, WHITE);

        // Action Buttons
        const char* btnLabels[] = { "Basic Attack", "Skill", "Ultimate", "Skip" };
        const char* energyCost[] = { "+1", "-2", "-3", "+2" };

        int btnWidth = 200;
        int btnHeight = 60;
        int gap = 20;

        int startX = (screenW - (btnWidth * 4 + gap * 3)) / 2;
        int startY = dialogRec.y + dialogRec.height + 30;

        // Buttons Logic
        if (IsKeyPressed(KEY_RIGHT)) {
            selectedOption = (selectedOption + 1) % 4;
        }
        if (IsKeyPressed(KEY_LEFT)) {
            selectedOption = (selectedOption - 1 + 4) % 4;
        }

        for (int i = 0; i < 4; i++) {
            Rectangle btnRec = { startX + i * (btnWidth + gap), startY, btnWidth, btnHeight };

            bool isUsable = true;
            if (i == 1 && player.energy < 2) isUsable = false; // Skill
            if (i == 2 && player.energy < 3) isUsable = false; // Ultimate

            if (!isUsable) {
                GuiSetState(STATE_DISABLED);
            }
            else {
                GuiSetState(STATE_NORMAL);
            }

            // Input Detection
            bool mouseClicked = GuiButton(btnRec, btnLabels[i]);
            bool enterPressed = (IsKeyPressed(KEY_ENTER) && i == selectedOption);

            if ((mouseClicked || enterPressed) && isUsable) {
                selectedOption = i;
                switch (i) {
                    case 0:
                        playerChoice = BASIC_ATTACK;
                        currentBattleState = BATTLE_PLAYER_QUIZ;
                        BasicAttack();
                        break;
                    case 1:
                        playerChoice = SKILL;
                        // Implement Skill Logic
                        break;
                    case 2:
                        playerChoice = ULTIMATE;
                        // Implement Ultimate Logic
                        break;
                    case 3:
                        playerChoice = SKIP;
                        // Implement Skip Logic
                        break;
                }
            }
            // Selected Button Highlight
            if (i == selectedOption) {
                Color highlightColor = isUsable ? YELLOW : RED; 
                DrawRectangleLinesEx(btnRec, 4, highlightColor);

                Vector2 v1 = { btnRec.x + btnWidth / 2 - 10, btnRec.y - 15};
                Vector2 v2 = { btnRec.x + btnWidth / 2 + 10, btnRec.y - 15};
                Vector2 v3 = { btnRec.x + btnWidth / 2, btnRec.y - 5};

                DrawTriangle(v1, v2, v3, WHITE);
            }
        }



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