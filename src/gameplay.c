#include "raylib.h"
#include "raygui.h"
#include "global.h"
#include "animation.h"
#include "SaveSystem.h"
#include "gameplay.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

// State internal gameplay
GameState gameplayNextState = GAMEPLAY;
static Texture2D background;
static Texture2D frameOverlay;
static float fadeAlpha = 1.0f;
static bool fadeIn = true;
static bool playerReachedCenter = false;
static bool showDialog = false;
static int dialogChoice = 0;
static float playerWalkProgress = 0.0f;
static bool playerExiting = false;
static float exitProgress = 0.0f;
static bool fadeOutComplete = false;
static float dialogSlideProgress = 0.0f;
static float dialogSlideDuration = 0.5f;
static float dialogSlideTimer = 0.0f;
static float buttonSlideProgress = 0.0f;      // Progress animasi slide tombol terpisah
static float buttonSlideDuration = 0.7f;      // Durasi animasi slide tombol
static float buttonSlideTimer = 0.0f;
static bool fadeOutUI = false;                // Flag untuk animasi fade out UI
static float fadeOutUIProgress = 0.0f;        // Progress fade out UI
static float fadeOutUIDuration = 0.3f;        // Durasi fade out UI

// ============ VARIABEL YANG BISA DIKUSTOMISASI ============
static float walkDuration = 1.5f;
static int playerStartX = -500;
static int playerStopX = 250;
static int playerY = 100;
static float playerScale = 7.0f;
static float exitDuration = 2.5f;
// ==========================================================

static float walkTimer = 0.0f;
static float exitTimer = 0.0f;

// Inisialisasi gameplay
void InitGameplay() {
    // Load background gameplay
    background = LoadTexture("assets/bg_1/BG_Gameplay.png");
    SetTextureFilter(background, TEXTURE_FILTER_POINT);
    
    // Load frame overlay (layer paling atas)
    frameOverlay = LoadTexture("assets/bg_1/BG_GameplayOverlay.png");
    SetTextureFilter(frameOverlay, TEXTURE_FILTER_POINT);
    
    // Inisialisasi animasi player
    InitPlayerAnimations();
    
    // Mulai dengan animasi P_RUN (berjalan)
    PlayPlayerAnimation(P_RUN, true);
    
    // Reset semua state
    gameplayNextState = GAMEPLAY;
    fadeAlpha = 1.0f;
    fadeIn = true;
    playerReachedCenter = false;
    showDialog = false;
    dialogChoice = 0;
    playerWalkProgress = 0.0f;
    playerExiting = false;
    exitProgress = 0.0f;
    fadeOutComplete = false;
    dialogSlideProgress = 0.0f;
    dialogSlideTimer = 0.0f;
    buttonSlideProgress = 0.0f;
    buttonSlideTimer = 0.0f;
    fadeOutUI = false;
    fadeOutUIProgress = 0.0f;
    walkTimer = 0.0f;
    exitTimer = 0.0f;
    
    printf("[Gameplay] Initialized for player: %s, Stage: %d\n", player.name, player.stage);
}

// Update gameplay
GameState UpdateGameplay() {
    float deltaTime = GetFrameTime();
    
    // Update animasi player
    UpdatePlayerAnimation(deltaTime);
    
    // Update transisi fade in saat awal
    if (fadeIn) {
        fadeAlpha -= deltaTime * 2.0f;
        if (fadeAlpha <= 0.0f) {
            fadeAlpha = 0.0f;
            fadeIn = false;
        }
    }
    
    // Update animasi fade out UI setelah pilihan
    if (fadeOutUI) {
        fadeOutUIProgress += deltaTime / fadeOutUIDuration;
        if (fadeOutUIProgress > 1.0f) {
            fadeOutUIProgress = 1.0f;
        }
    }
    
    // Update animasi slide dialog
    if (showDialog && dialogSlideProgress < 1.0f) {
        dialogSlideTimer += deltaTime;
        dialogSlideProgress = dialogSlideTimer / dialogSlideDuration;
        if (dialogSlideProgress > 1.0f) dialogSlideProgress = 1.0f;
    }
    
    // Update animasi slide tombol (dimulai sedikit setelah dialog)
    if (showDialog && buttonSlideProgress < 1.0f && dialogSlideProgress >= 0.3f) {
        buttonSlideTimer += deltaTime;
        buttonSlideProgress = buttonSlideTimer / buttonSlideDuration;
        if (buttonSlideProgress > 1.0f) buttonSlideProgress = 1.0f;
    }
    
    // Jika player sedang keluar (setelah klik YES)
    if (playerExiting) {
        exitTimer += deltaTime;
        exitProgress = exitTimer / exitDuration;
        
        if (exitProgress > 1.0f) exitProgress = 1.0f;
        
        if (exitProgress >= 0.7f && !fadeOutComplete) {
            float fadeStart = 0.7f;
            float fadeEnd = 1.0f;
            float fadeProgress = (exitProgress - fadeStart) / (fadeEnd - fadeStart);
            
            fadeAlpha = fadeProgress;
            
            if (fadeAlpha >= 1.0f) {
                fadeAlpha = 1.0f;
                fadeOutComplete = true;
                
                player.stage += 1;
                printf("[Gameplay] Stage increased to: %d\n", player.stage);
                
                if (player.stage >= 1 && player.stage <= 10) {
                    currentEnemy = enemies[player.stage - 1];
                    printf("[Gameplay] Loading enemy: %s\n", currentEnemy.name);
                }
                
                gameplayNextState = BATTLE;
            }
        }
        
        return gameplayNextState;
    }
    
    // Update pergerakan player masuk
    if (!playerReachedCenter && !playerExiting) {
        walkTimer += deltaTime;
        playerWalkProgress = walkTimer / walkDuration;
        
        if (playerWalkProgress > 1.0f) playerWalkProgress = 1.0f;
        
        if (playerWalkProgress >= 1.0f) {
            playerReachedCenter = true;
            PlayPlayerAnimation(P_IDLE, true);
            
            // Mulai animasi slide UI
            showDialog = true;
            dialogSlideTimer = 0.0f;
            dialogSlideProgress = 0.0f;
            buttonSlideTimer = 0.0f;
            buttonSlideProgress = 0.0f;
        }
    }
    
    // Handle input dialog jika UI sudah sepenuhnya muncul dan belum fade out
    if (showDialog && !fadeOutUI && dialogSlideProgress >= 1.0f && 
        buttonSlideProgress >= 1.0f && dialogChoice == 0 && !playerExiting) {
        
        // Deteksi input untuk dialog
        if (IsKeyPressed(KEY_Y) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_RIGHT)) {
            dialogChoice = 1; // Yes
            printf("[Gameplay] Player chose: YES\n");
            
            // Mulai animasi fade out UI
            fadeOutUI = true;
            fadeOutUIProgress = 0.0f;
            
            // Tunggu sebentar lalu mulai animasi keluar player
            // (kita akan handle ini di bawah)
        } else if (IsKeyPressed(KEY_N) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_LEFT)) {
            dialogChoice = 2; // No
            printf("[Gameplay] Player chose: NO\n");
            
            // Mulai animasi fade out UI
            fadeOutUI = true;
            fadeOutUIProgress = 0.0f;
            
            // Simpan score setelah UI fade out
            // (kita akan handle ini di bawah)
        }
    }
    
    // Jika UI sudah fade out sepenuhnya
    if (fadeOutUI && fadeOutUIProgress >= 1.0f) {
        if (dialogChoice == 1) {
            // Mulai animasi keluar player
            playerExiting = true;
            PlayPlayerAnimation(P_RUN, true);
            exitTimer = 0.0f;
            exitProgress = 0.0f;
            fadeOutComplete = false;
        } else if (dialogChoice == 2) {
            // Simpan score dan pindah ke lobby
            const char* mode = (player.currentMode == MATH) ? "math" : "logic";
            saveScore(player.name, mode, player.score);
            printf("[Gameplay] Score saved: %d for %s in mode %s\n", 
                   player.score, player.name, mode);
            
            gameplayNextState = LOBBY;
        }
    }
    
    // Jika pilihan NO dan belum fade out UI, mulai fade out game
    if (dialogChoice == 2 && fadeOutUI && fadeOutUIProgress >= 1.0f && !playerExiting) {
        fadeAlpha += deltaTime * 2.0f;
        
        if (fadeAlpha >= 1.0f) {
            fadeAlpha = 1.0f;
            gameplayNextState = LOBBY;
        }
    }
    
    return gameplayNextState;
}

// Fungsi untuk menggambar UI dengan animasi
void DrawGameplayUI() {
    if (!showDialog || playerExiting) return;
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Hitung alpha berdasarkan fade out progress
    float uiAlpha = 1.0f - fadeOutUIProgress; // Dari 1.0 ke 0.0
    
    // ============ DIALOG BOX (ATAS) ============
    int dialogHeight = 130;
    int targetDialogY = 30;
    
    // Animasi slide dialog dari atas
    int dialogStartY = -dialogHeight;
    int currentDialogY = dialogStartY + (int)((targetDialogY - dialogStartY) * dialogSlideProgress);
    
    int dialogWidth = screenWidth - 100;
    int dialogX = (screenWidth - dialogWidth) / 2;
    int dialogY = currentDialogY;
    
    // Gambar dialog dengan efek fade
    float dialogBoxAlpha = dialogSlideProgress * 0.9f * uiAlpha;
    DrawRectangle(dialogX, dialogY, dialogWidth, dialogHeight, 
                 Fade((Color){20, 20, 30, 230}, dialogBoxAlpha));
    
    // Border dialog
    if (dialogSlideProgress >= 1.0f) {
        DrawLine(dialogX, dialogY, dialogX + dialogWidth, dialogY, 
                Fade(GOLD, dialogBoxAlpha));
        DrawLine(dialogX, dialogY + dialogHeight, dialogX + dialogWidth, dialogY + dialogHeight, 
                Fade(GOLD, dialogBoxAlpha));
    }
    
    // Teks pertanyaan
    const char* questionText = "";
    if (player.stage == 0) {
        questionText = "Are You Ready To Start The EduQuest?";
    } else if (player.stage == 9) {
        questionText = "Are You Ready For The Boss Battle?";
    } else {
        questionText = "Are You Ready For The Next Stage?";
    }
    
    // Gambar teks dengan efek fade
    int questionFontSize = 26;
    int questionWidth = MeasureText(questionText, questionFontSize);
    int questionX = dialogX + (dialogWidth - questionWidth) / 2;
    int questionY = dialogY + 25;
    
    Color questionColor = Fade((Color){255, 255, 200, 255}, dialogBoxAlpha);
    DrawText(questionText, questionX, questionY, questionFontSize, questionColor);
    
    // Info stage
    char stageInfo[50];
    snprintf(stageInfo, sizeof(stageInfo), "Stage %d/10", player.stage+1);
    int stageWidth = MeasureText(stageInfo, 20);
    DrawText(stageInfo, dialogX + (dialogWidth - stageWidth)/2, 
             questionY + 35, 20, Fade((Color){200, 200, 255, 255}, dialogBoxAlpha));
    
    // Petunjuk keyboard dalam dialog box
    const char* hintText = "Press Left / N for NO, Right / Y / Enter for YES";
    int hintWidth = MeasureText(hintText, 16);
    int hintX = dialogX + (dialogWidth - hintWidth) / 2;
    int hintY = dialogY + 85;
    DrawText(hintText, hintX, hintY, 16, Fade((Color){200, 200, 150, 200}, dialogBoxAlpha));
    
    // ============ TOMBOL (POJOK BAWAH) ============
    int buttonWidth = 180;
    int buttonHeight = 60;
    
    // Posisi akhir tombol (pojok bawah)
    int targetYesX = screenWidth - buttonWidth - 50;  // Pojok kanan bawah
    int targetNoX = 50;                               // Pojok kiri bawah
    int targetButtonY = screenHeight - buttonHeight - 50; // 50px dari bawah
    
    // Animasi slide tombol dari bawah luar layar
    int buttonStartY = screenHeight + 100;
    int currentButtonY = buttonStartY + (int)((targetButtonY - buttonStartY) * buttonSlideProgress);
    int buttonY = currentButtonY;
    
    // YES button di pojok kanan bawah
    Rectangle yesButton = {
        (float)targetYesX, 
        (float)buttonY, 
        (float)buttonWidth, 
        (float)buttonHeight
    };
    
    // NO button di pojok kiri bawah
    Rectangle noButton = {
        (float)targetNoX, 
        (float)buttonY, 
        (float)buttonWidth, 
        (float)buttonHeight
    };
    
    // Warna tombol dengan efek fade
    float buttonAlpha = buttonSlideProgress * uiAlpha;
    Color yesColor = Fade((Color){40, 180, 40, 220}, buttonAlpha);
    Color noColor = Fade((Color){180, 40, 40, 220}, buttonAlpha);
    
    // Handle input jika UI sepenuhnya muncul dan belum fade out
    if (!fadeOutUI && dialogSlideProgress >= 1.0f && buttonSlideProgress >= 1.0f) {
        Vector2 mousePos = GetMousePosition();
        
        // Tombol Yes (kanan bawah)
        if (CheckCollisionPointRec(mousePos, yesButton)) {
            yesColor = Fade((Color){60, 220, 60, 255}, buttonAlpha);
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && dialogChoice == 0) {
                dialogChoice = 1;
                fadeOutUI = true;
                fadeOutUIProgress = 0.0f;
            }
        }
        // Tombol No (kiri bawah)
        else if (CheckCollisionPointRec(mousePos, noButton)) {
            noColor = Fade((Color){220, 60, 60, 255}, buttonAlpha);
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && dialogChoice == 0) {
                dialogChoice = 2;
                fadeOutUI = true;
                fadeOutUIProgress = 0.0f;
            }
        }
    }
    
    // Gambar tombol Yes (pojok kanan bawah)
    DrawRectangleRec(yesButton, yesColor);
    if (buttonSlideProgress >= 1.0f) {
        DrawRectangleLinesEx(yesButton, 2, Fade((Color){100, 255, 100, 255}, buttonAlpha));
    }
    
    const char* yesText = "YES";
    int yesTextWidth = MeasureText(yesText, 26);
    DrawText(yesText, 
             targetYesX + (buttonWidth - yesTextWidth)/2, 
             buttonY + (buttonHeight - 26)/2, 
             26, Fade(WHITE, buttonAlpha));
    
    // Gambar tombol No (pojok kiri bawah)
    DrawRectangleRec(noButton, noColor);
    if (buttonSlideProgress >= 1.0f) {
        DrawRectangleLinesEx(noButton, 2, Fade((Color){255, 100, 100, 255}, buttonAlpha));
    }
    
    const char* noText = "NO";
    int noTextWidth = MeasureText(noText, 26);
    DrawText(noText, 
             targetNoX + (buttonWidth - noTextWidth)/2, 
             buttonY + (buttonHeight - 26)/2, 
             26, Fade(WHITE, buttonAlpha));
    
}

// Draw gameplay
void DrawGameplay() {
    // Clear background
    ClearBackground(BLACK);
    
    // Gambar background gameplay
    DrawTexture(background, 0, 0, WHITE);
    
    // Hitung posisi player
    int currentPlayerX;
    int screenWidth = GetScreenWidth();
    
    if (playerExiting) {
        // Animasi keluar
        int exitStartX = playerStopX;
        int exitEndX = screenWidth + 500;
        currentPlayerX = exitStartX + (int)((exitEndX - exitStartX) * exitProgress);
    } else {
        // Animasi masuk
        currentPlayerX = playerStartX + (int)((playerStopX - playerStartX) * playerWalkProgress);
    }
    
    // Gambar player dengan animasi
    DrawPlayerSprite(currentPlayerX, playerY, playerScale);
    
    // Gambar frame overlay
    DrawTexture(frameOverlay, 0, 0, WHITE);
    
    // Gambar UI (dialog + tombol)
    if (showDialog && !playerExiting) {
        DrawGameplayUI();
    }
    
    // Gambar transisi fade
    if (fadeAlpha > 0.0f) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), 
                     Fade(BLACK, fadeAlpha));
    }
}

// Unload resources gameplay
void UnloadGameplay() {
    UnloadTexture(background);
    UnloadTexture(frameOverlay);
    UnloadPlayerAnimations();
    
    printf("[Gameplay] Resources unloaded\n");
}

// ============ FUNGSI UNTUK MENGUBAH VARIABEL KUSTOMISASI ============

void SetPlayerWalkDuration(float duration) {
    walkDuration = duration;
    printf("[Gameplay] Walk duration set to: %.1f seconds\n", walkDuration);
}

void SetPlayerExitDuration(float duration) {
    exitDuration = duration;
    printf("[Gameplay] Exit duration set to: %.1f seconds\n", exitDuration);
}

void SetPlayerStartX(int startX) {
    playerStartX = startX;
    printf("[Gameplay] Player start X set to: %d\n", playerStartX);
}

void SetPlayerStopX(int stopX) {
    playerStopX = stopX;
    printf("[Gameplay] Player stop X set to: %d\n", playerStopX);
}

void SetPlayerY(int y) {
    playerY = y;
    printf("[Gameplay] Player Y position set to: %d\n", playerY);
}

void SetPlayerScale(float scale) {
    playerScale = scale;
    printf("[Gameplay] Player scale set to: %.1f\n", playerScale);
}

void SetDialogSlideDuration(float duration) {
    dialogSlideDuration = duration;
    printf("[Gameplay] Dialog slide duration set to: %.1f seconds\n", dialogSlideDuration);
}

void SetButtonSlideDuration(float duration) {
    buttonSlideDuration = duration;
    printf("[Gameplay] Button slide duration set to: %.1f seconds\n", buttonSlideDuration);
}

void SetFadeOutUIDuration(float duration) {
    fadeOutUIDuration = duration;
    printf("[Gameplay] UI fade out duration set to: %.1f seconds\n", fadeOutUIDuration);
}