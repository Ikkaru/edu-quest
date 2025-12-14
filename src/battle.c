#include "global.h"
#include "MathQuest.h"
#include "animation.h"
#include "battle.h"
#include "logika.h"
#include "raygui.h"
#include <stdbool.h>
#include <stdio.h>  
#include <stdlib.h>
#include <string.h>

Music bgm;

// Variabel Soal 
Question question;

// answer Variabel
char userAnswer[64];
bool answerEditMode = true; // Apakah user sedang mengedit jawaban
bool correct = false;

// Konstanta Timer
const float QUESTION_TIME_LIMIT = 10.0f;
float timer = 0.0f;

// Konstanta Floating Damage
DamageText damageText = {0};
const float DAMAGE_TEXT_DURATION = 2.0f;
const float DAMAGE_TEXT_SPEED = 0.8f;
float animationTimer = 0.0f;

// Enemy Turn Dialog
bool enemyTurnDialogActive = false;
float enemyTurnDialogTimer = 0.0f;
const float ENEMY_TURN_DIALOG_DURATION = 3.0f;  

BattleState currentBattleState = BATTLE_PLAYER_CHOICE;
PlayerChoice playerChoice;
GameState nextGameState = BATTLE;

// Intitalize Soal Logika
Soal daftarSoal[MAKS_SOAL];
KunciJawaban daftarJawaban[MAKS_SOAL];
int jumlahSoal = 0;
int jumlahJawaban = 0;
Soal CurrentQuestion;
char correctAnswer;

int selectedOption = 0;

void InitBattle(int stage) {
    // Initialize Battle State
    nextGameState = BATTLE;
    currentBattleState = BATTLE_PLAYER_CHOICE;
    currentEnemy = enemies[stage - 1];
    playerChoice = NOT_SELECTED;
    selectedOption = 0;
    InitPlayerAnimations();
    InitEnemyAnimations(&currentEnemy, stage);

    // Setup All Global Variables
    memset(userAnswer, 0, sizeof(userAnswer));
    answerEditMode = true;
    correct = false;
    timer = QUESTION_TIME_LIMIT;
    animationTimer = 0.0f;
    enemyTurnDialogActive = false;  
    enemyTurnDialogTimer = 0.0f;

    // Play Background Audio
    bgm = LoadMusicStream("assets/audio/bgm/battle.mp3");
    PlayMusicStream(bgm);


    jumlahSoal = muatSoal("quest.txt", daftarSoal, MAKS_SOAL);
    jumlahJawaban = muatJawaban("answ.txt", daftarJawaban, MAKS_SOAL);
}

GameState UpdateBattle() {

    float dt = GetFrameTime();

    // State Check
    switch (currentBattleState) {
        case BATTLE_PLAYER_CHOICE:
            answerEditMode = true;
            timer = QUESTION_TIME_LIMIT;
        break;

        case BATTLE_PLAYER_QUIZ:
            if (timer > 0) {
                timer -= dt;
            }
            else {
                timer = 0;
                TraceLog(LOG_INFO, "Time's up!");
                ExecutePlayerDamage(false);
            }
            if (IsKeyPressed(KEY_ENTER)) {
                animationTimer = 0.0f; // Reset animation timer
                TraceLog(LOG_INFO, "Player Answered: %s", userAnswer);
                correct = IsAnswerCorrect();
                ExecutePlayerDamage(correct);
                memset(userAnswer, 0, sizeof(userAnswer)); // Clear answer
                answerEditMode = false;

            }
        break;

        case BATTLE_ANIMATION:
            animationTimer += dt;

            // Trigger enemy hurt animation selesai
            if (IsPlayerAnimationFinished()) {
                damageText.active = true;
                if (!correct) {
                    enemyTurnDialogActive = true;
                    timer = QUESTION_TIME_LIMIT;
                    currentBattleState = BATTLE_ENEMY_TURN;
                    enemyTurnDialogTimer = ENEMY_TURN_DIALOG_DURATION;
                    playerChoice = NOT_SELECTED;
                }
                else {
                    PlayEnemyAnimation(&currentEnemy, E_HURT);
                }

                ResetPlayerToIdle();
            }

            if (IsEnemyAnimationFinished(&currentEnemy) && animationTimer > 0.5f) {

                // Check if enemy defeated
                if (currentEnemy.HP <= 0) {
                    TraceLog(LOG_INFO, "Enemy Defeated!");
                    PlayEnemyAnimation(&currentEnemy, E_DEATH);
                    currentBattleState = BATTLE_VICTORY;
                    break;
                }

                ResetEnemyToIdle(&currentEnemy);

                correct = false; // Reset correct for next turn
                

                // Enemy Turn
                enemyTurnDialogActive = true;
                currentBattleState = BATTLE_ENEMY_TURN;
                timer = QUESTION_TIME_LIMIT;
                enemyTurnDialogTimer = ENEMY_TURN_DIALOG_DURATION;
                playerChoice = NOT_SELECTED;

            }
            break;

        case BATTLE_ENEMY_TURN:
            if (enemyTurnDialogActive) {
                if (enemyTurnDialogTimer > 0) {
                    enemyTurnDialogActive = true;
                    enemyTurnDialogTimer -= dt;
                }
                else {
                    enemyTurnDialogActive = false;
                    answerEditMode = true;
                    question = generateQuestion(player.stage);
                }
            }
            else {
                if (timer > 0) {
                    timer -= dt;
                }
                else {
                    timer = 0;
                    TraceLog(LOG_INFO, "Time's up!");
                    ExecuteEnemyDamage(false); // Time's up, treat as wrong answer
                }
                if (IsKeyPressed(KEY_ENTER)) {
                    animationTimer = 0.0f; // Reset animation timer
                    TraceLog(LOG_INFO, "Player Answered: %s", userAnswer);
                    correct = IsAnswerCorrect();
                    // Log the value of 'correct'
                    TraceLog(LOG_INFO, "Is answer correct? %s", correct ? "true" : "false");
                    ExecuteEnemyDamage(correct);
                    memset(userAnswer, 0, sizeof(userAnswer)); // Clear answer
                    answerEditMode = false;

                }
            }
            break;
        case BATTLE_ENEMY_ANIMATION:
            animationTimer += dt;
            if (IsEnemyAnimationFinished(&currentEnemy)) {
                damageText.active = true;
                if (!correct) {
                    PlayPlayerAnimation(P_HURT, false);
                }
                else {
                    // Back to player's turn
                    currentBattleState = BATTLE_PLAYER_CHOICE;
                    playerChoice = NOT_SELECTED;
                }
                ResetEnemyToIdle(&currentEnemy);
            }


            if (IsPlayerAnimationFinished() && animationTimer > 0.5f) {
                ResetPlayerToIdle();

                // Check if player defeated
                if (player.HP <= 0) {
                    TraceLog(LOG_INFO, "Player Defeated!");
                    nextGameState = GAME_OVER; // Game over jika player kalah
                    break;
                }

                correct = false; // Reset correct for next turn

                // Back to player's turn
                currentBattleState = BATTLE_PLAYER_CHOICE;
                playerChoice = NOT_SELECTED;
            }

            break;

        case BATTLE_VICTORY:
            if (IsEnemyAnimationFinished(&currentEnemy)) {
                nextGameState = GAMEPLAY; // Kembali ke Gameplay setelah Animasi Enemy Death tampilkan
            }
    } 
    // Update Player Annimation
    UpdatePlayerAnimation(dt);
    // UpdateEnemyAnimation(&currentEnemy, deltaTime);
    UpdateEnemyAnimation(&currentEnemy, dt);
    //Update Music Stream
    UpdateMusicStream(bgm);

    // Update floating damage text
    if (damageText.active) {
        damageText.y -= dt;
        damageText.lifetime -= DAMAGE_TEXT_SPEED * dt;
        damageText.alpha = damageText.lifetime / DAMAGE_TEXT_DURATION;

        if (damageText.lifetime <= 0) {
            damageText.active = false;
        }
    }

    return nextGameState;
}

// DRAW BATTLE GUI CORE
void DrawBattleGUI() { 
    // Gui Style
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 3);
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(YELLOW));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(GRAY));


    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    ClearBackground(BLACK);
    // Draw Player Sprite
    DrawPlayerSprite(150, 80, 3.0f);
    char hpLabel[100];
    DrawHealthBar(100, 410, player.HP, player.maxHP, 200, GREEN, TextFormat("%s HP", player.name));

    // Draw Enemy Sprite
    DrawEnemySprite(&currentEnemy, 800, 200, 2.8f);

    // Draw Turn Indicator
    DrawTurnIndicator();

    // Draw floating damage text
    if (damageText.active) {
        Color textColor = damageText.color;
        textColor.a = (unsigned char)(damageText.alpha * 255);

        int fontSize = 30;
        int textWidth = MeasureText(damageText.text, fontSize);

        DrawText(damageText.text, (int)(damageText.x - textWidth / 2), (int)(damageText.y), fontSize, textColor);
    }

    // Draw Current Energy
    DrawText(TextFormat("Energy: %d / %d", player.energy, player.maxEnergy), 20, 20, 20, YELLOW);
    DrawHealthBar(850, 410, currentEnemy.HP, currentEnemy.maxHP, 200, RED, TextFormat("%s HP", currentEnemy.name));

    switch (currentBattleState)
    {
    case BATTLE_PLAYER_CHOICE:
        
        // Dialog Box
        int dialogWidth = 1100;
        int dialogHeight = 125;
        Rectangle dialogRec = { (screenW - dialogWidth) / 2, 450 , dialogWidth, dialogHeight };

        GuiGroupBox(dialogRec, NULL);

        DrawText("*Make Your Choice", dialogRec.x + 90, dialogRec.y + 45, 28, GRAY);

        // Action Buttons
        const char* btnLabels[] = { "Basic Attack", "Skill", "Ultimate", "Skip" };
        const char* energyCost[] = { "+1", "-2", "-3", "+2" };

        int gap = 20;
        int btnWidth = (dialogWidth / 4) - 15;
        int btnHeight = 60;
        

        int startX = dialogRec.x;;
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
                GuiSetState(STATE_NORMAL);
                selectedOption = i;
                switch (i) {
                    case 0: // Basic Attack
                        playerChoice = BASIC_ATTACK;
                        currentBattleState = BATTLE_PLAYER_QUIZ;

                        timer = QUESTION_TIME_LIMIT; // Reset Timer
                        memset(userAnswer, 0, sizeof(userAnswer)); // Clear previous answer

                        // Increase Energy for Basic Attack
                        player.energy += 1;
                        if (player.energy > player.maxEnergy) {
                            player.energy = player.maxEnergy;
                        }

                        break;
                    case 1: // Skill
                        playerChoice = SKILL;
                        currentBattleState = BATTLE_PLAYER_QUIZ;

                        // Decrease Energy for Skill
                        player.energy -= 2;
                        timer = QUESTION_TIME_LIMIT; // Reset Timer

                        memset(userAnswer, 0, sizeof(userAnswer)); // Clear previous answer
                        // Generate Question based on Game Mode
                        if (player.currentMode == MATH) {
                            question = generateQuestion(player.stage);
                        }
                        else if (player.currentMode == LOGIC) {
                            // Logic Question Generation (if implemented)
                        }
                        
                        break;
                    case 2:
                        playerChoice = ULTIMATE;

                        currentBattleState = BATTLE_PLAYER_QUIZ;
                        // Decrease Energy for Ultimate
                        player.energy -= 3;
                        timer = QUESTION_TIME_LIMIT; // Reset Timer
                        memset(userAnswer, 0, sizeof(userAnswer)); // Clear previous answer
                        
                        break;
                    case 3: // SKIP
                        playerChoice = SKIP;
                        currentBattleState = BATTLE_ENEMY_TURN; // Skip to Enemy Turn
                        enemyTurnDialogTimer = ENEMY_TURN_DIALOG_DURATION;
                        // Increase Energy for Skip
                        player.energy += 2;
                        if (player.energy > player.maxEnergy) {
                            player.energy = player.maxEnergy;
                        }
                        timer = QUESTION_TIME_LIMIT; // Reset Timer
                        enemyTurnDialogActive = true;
                        memset(userAnswer, 0, sizeof(userAnswer)); // Clear previous answer

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

                DrawTriangle(v1, v3, v2, WHITE);
            }
            // Draw Energy Cost Label
            Color costColor = isUsable ? BLUE : RED;
            DrawText(TextFormat("Energy %s", energyCost[i]), btnRec.x + (btnWidth / 2) - 50, btnRec.y + btnHeight + 5, 20, costColor);
        }
        break;

        case BATTLE_PLAYER_QUIZ:
            DrawQuizInterface(screenW, screenH);
            break;

        case BATTLE_ANIMATION:

            // Annimation State Dialog Box
            int aniWidth = 1100;
            int aniHeight = 230;
            int aniX = (screenW - aniWidth) / 2;
            int aniY = 450;
            Rectangle animRec = { aniX, aniY , aniWidth, aniHeight };
            DrawRectangleRec(animRec, Fade(BLACK, 0.9f));
            GuiGroupBox(animRec, NULL);

            // Dialog Text Based on Correctness
            if (correct) {
                DrawText("It Was Effective!", animRec.x + 400, animRec.y + 80, 30, GREEN);
            }

            else {
                DrawText("It Missed!", animRec.x + 450, animRec.y + 80, 30, RED);
            }

            break;
        
        case BATTLE_ENEMY_TURN:
            // Enemy Turn Dialog Box
            if (enemyTurnDialogActive) {
                int enemyWidth = 1100;
                int enemyHeight = 125;
                int enemyX = (screenW - enemyWidth) / 2;
                int enemyY = 450;
                Rectangle enemyRec = { enemyX, enemyY , enemyWidth, enemyHeight };
                GuiGroupBox(enemyRec, NULL);

                DrawText("Enemy's Turn!", enemyRec.x + 450, enemyRec.y + 45, 28, RED);
            }
            else {
                DrawQuizInterface(screenW, screenH);
            }

            break;
        
        case BATTLE_ENEMY_ANIMATION:
            // Annimation State Dialog Box
            {
                int aniWidth = 1100;
                int aniHeight = 230;
                int aniX = (screenW - aniWidth) / 2;
                int aniY = 450;
                Rectangle animRec = { aniX, aniY , aniWidth, aniHeight };
                DrawRectangleRec(animRec, Fade(BLACK, 0.9f));
                GuiGroupBox(animRec, NULL);

                // Dialog Text Based on Correctness
                if (!correct) {
                    DrawText("You've got hit!", animRec.x + 250, animRec.y + 80, 30, RED);
                }
                else if (correct) {
                    DrawText("You dodged the attack!", animRec.x + 300, animRec.y + 80, 30, GREEN);
                }
            }
            break;
        

    
    default:
        break;
    }



}

bool IsAnswerCorrect() {
    if (player.currentMode == MATH) {
        int answerInt = atoi(userAnswer);
        bool isCorrect = (answerInt == question.correctAnswer);
        return isCorrect;
    }
    else if (player.currentMode == LOGIC) {
        
    }
}

void ExecutePlayerDamage(bool iscorrect) {
    // Damage Calculation based on player choice
    int damage = 0;
    switch (playerChoice) {
        case BASIC_ATTACK:
            damage = 10; // Basic Attack Damage
            PlayPlayerAnimation(P_BASIC_ATTACK, false);
            break;
        case SKILL:
            damage = 25; // Skill Damage
            PlayPlayerAnimation(P_SKILL, false);
            break;
        case ULTIMATE:
            damage = 50; // Ultimate Damage
            PlayPlayerAnimation(P_ULTIMATE, false);
            break;
        default:
            break;
    }

    if (iscorrect) {
        currentEnemy.HP -= damage;
        if (currentEnemy.HP < 0) currentEnemy.HP = 0;

        // Create floating damage text
        sprintf(damageText.text, "-%d", damage);
        damageText.x = 900;
        damageText.y = 120;
        damageText.alpha = 1.0f;
        damageText.lifetime = DAMAGE_TEXT_DURATION;
        damageText.color = RED;

        TraceLog(LOG_INFO, "Correct Answer! Dealt %d damage to %s", damage, currentEnemy.name);

    }
    else {
        // Wrong Answer, Miss
        sprintf(damageText.text, "MISS");
        damageText.x = 900;
        damageText.y = 100;
        damageText.alpha = 1.0f;
        damageText.lifetime = DAMAGE_TEXT_DURATION;
        damageText.color = GRAY;

        TraceLog(LOG_INFO, "Wrong Answer! No damage dealt.");
    }

    currentBattleState = BATTLE_ANIMATION;
}

void ExecuteEnemyDamage(bool iscorrect) {
    int damage = currentEnemy.damage;

    if (!iscorrect) {
        player.HP -= damage;
        if (player.HP < 0) player.HP = 0;

        // Create floating damage text
        sprintf(damageText.text, "-%d", damage);
        damageText.x = 250;
        damageText.y = 120;
        damageText.alpha = 1.0f;
        damageText.lifetime = DAMAGE_TEXT_DURATION;
        damageText.color = RED;

        TraceLog(LOG_INFO, "Enemy Correct Answer! Dealt %d damage to %s", damage, player.name);

    }
    else {
        // Wrong Answer, Miss
        sprintf(damageText.text, "MISS");
        damageText.x = 250;
        damageText.y = 100;
        damageText.alpha = 1.0f;
        damageText.lifetime = DAMAGE_TEXT_DURATION;
        damageText.color = GRAY;

        TraceLog(LOG_INFO, "Enemy Wrong Answer! No damage dealt.");
    }
    PlayEnemyAnimation(&currentEnemy, E_ATTACK);

    currentBattleState = BATTLE_ENEMY_ANIMATION;
}

// Function to draw quiz interface
void DrawQuizInterface(int screenW, int screenH) {
    // Draw Quiz Interface
    int quizWidth = 1100;
    int quizHeight = 230;
    int quizX = (screenW - quizWidth) / 2;
    int quizY = 450;

    Rectangle quizRec = { quizX, quizY , quizWidth, quizHeight };
    DrawRectangleRec(quizRec, Fade(BLACK, 0.9f));
    GuiGroupBox(quizRec, NULL);

    // Generate Question based on Game Mode
    if (player.currentMode == MATH) {
        question = generateQuestion(player.stage);
    }
    else if (player.currentMode == LOGIC) {
        int successGetQuestion = ambilSoalAcak(daftarSoal, jumlahSoal, daftarJawaban, jumlahJawaban, &CurrentQuestion, &correctAnswer);

        if (successGetQuestion) {
            TraceLog(LOG_INFO, "Logic Question Generated");
            DrawText(CurrentQuestion.pertanyaan, quizX + 25, quizY + 43, 30, WHITE);
            DrawText(TextFormat("A. %s", CurrentQuestion.pilihanA ), quizX + 25, quizY + 90, 25, WHITE);
            DrawText(TextFormat("B. %s", CurrentQuestion.pilihanB ), quizX + 25, quizY + 130, 25, WHITE);
            DrawText(TextFormat("C. %s", CurrentQuestion.pilihanC ), quizX + 25, quizY + 170, 25, WHITE);
        }
        else {
            TraceLog(LOG_WARNING, "Failed to generate logic question.");
        }
    }


    // Draw Question Text
    DrawText(question.questionText, quizX + 25, quizY + 43, 30, WHITE);

    // Input Box
    Rectangle inputRec = { quizX + 20, quizY + 170, 200, 40 };

    if (GuiTextBox(inputRec, userAnswer, sizeof(userAnswer), answerEditMode)) {
        answerEditMode = !answerEditMode; // Toggle edit mode on click
    }

    // Draw Timer
    float maxBarWidth = quizWidth - 10;
    float barHeight = 2;
    float barX = quizX + 10;
    float barY = quizY + 8;
    
    // Hitung persentase sisa waktu
    float progress = timer / QUESTION_TIME_LIMIT;

    // Bar color
    Color timerColor = WHITE;
    if (progress < 0.5f) timerColor = ORANGE;
    if (progress < 0.2f) timerColor = RED;

    // Draw Timer Bar
    DrawRectangle(barX, barY, (int)(maxBarWidth * progress), (int)barHeight, timerColor);


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

// Function to draw turn indicator 
void DrawTurnIndicator() {
    int screenW = GetScreenWidth();
    
    bool isPlayerTurn = (currentBattleState == BATTLE_PLAYER_CHOICE || currentBattleState == BATTLE_PLAYER_QUIZ || currentBattleState == BATTLE_ANIMATION);
    bool isEnemyTurn = (currentBattleState == BATTLE_ENEMY_TURN);


    if (isPlayerTurn) {  
        int textWidth = MeasureText("Your Turn", 20);
        DrawText("Your Turn", screenW - textWidth - 20, 20, 20, WHITE);
        GuiDrawIcon(186, screenW - textWidth - 60, 15, 2, WHITE); 
    }
    else {
        int textWidth = MeasureText("Enemy Turn", 20);
        DrawText("Enemy Turn", screenW - textWidth - 20, 20, 20, WHITE);
        GuiDrawIcon(152, screenW - textWidth - 60, 15, 2, WHITE); 
    }
}

void UnloadBattle() {
    UnloadPlayerAnimations();
    UnloadEnemyAnimations(&currentEnemy);
    StopMusicStream(bgm);
    UnloadMusicStream(bgm);

}