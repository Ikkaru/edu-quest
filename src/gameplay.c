#include "raylib.h"
#include "raygui.h"
#include "global.h"
#include "animation.h"
#include "SaveSystem.h"
#include "gameplay.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

// State internal gameplay - semua jadikan static kecuali nextState
GameState gameplayNextState = GAMEPLAY;
static Texture2D background;
static Texture2D frameOverlay;
static float fadeAlpha = 1.0f;
static bool fadeIn = true;
static bool playerReachedCenter = false;
static bool showDialog = false;
static int dialogChoice = 0;
static float playerWalkProgress = 0.0f;

// ============ VARIABEL YANG BISA DIKUSTOMISASI ============
static float walkDuration = 2.5f;          // Waktu yang dibutuhkan untuk sampai ke tengah (dalam detik)
static int playerStartX = -400;            // Posisi X awal (di luar layar kiri)
static int playerStopX = 300;              // Posisi X berhenti (tengah layar = 1280/2 = 640)
static int playerY = 200;                  // Posisi Y player (tengah vertikal = 720/2 = 360)
static float playerScale = 5.0f;           // Scale ukuran player sprite
// ==========================================================

static float walkTimer = 0.0f;

// Inisialisasi gameplay
void InitGameplay() {
    // Load background gameplay (beda dengan lobby)
    background = LoadTexture("assets/bg_1/BG_Gameplay.png");
    SetTextureFilter(background, TEXTURE_FILTER_POINT);
    
    // Load frame overlay (layer paling atas)
    frameOverlay = LoadTexture("assets/bg_1/BG_GameplayOverlay.png");
    SetTextureFilter(frameOverlay, TEXTURE_FILTER_POINT);
    
    // Inisialisasi animasi player
    InitPlayerAnimations();
    
    // Mulai dengan animasi P_RUN (berjalan)
    PlayPlayerAnimation(P_RUN, true);
    
    // Reset state
    gameplayNextState = GAMEPLAY;
    fadeAlpha = 1.0f;
    fadeIn = true;
    playerReachedCenter = false;
    showDialog = false;
    dialogChoice = 0;
    playerWalkProgress = 0.0f;
    walkTimer = 0.0f;
    
    // Debug: Tampilkan nilai variabel kustomisasi
    printf("[Gameplay] Initialized for player: %s, Stage: %d\n", player.name, player.stage);
    printf("[Gameplay] Walk Duration: %.1f seconds\n", walkDuration);
    printf("[Gameplay] Start X: %d, Stop X: %d\n", playerStartX, playerStopX);
    printf("[Gameplay] Player Y: %d, Scale: %.1f\n", playerY, playerScale);
}

// Update gameplay
GameState UpdateGameplay() {
    float deltaTime = GetFrameTime();
    
    // Update animasi player
    UpdatePlayerAnimation(deltaTime);
    
    // Update transisi fade in/out
    if (fadeIn) {
        fadeAlpha -= deltaTime * 2.0f;
        if (fadeAlpha <= 0.0f) {
            fadeAlpha = 0.0f;
            fadeIn = false;
        }
    }
    
    // Update pergerakan player jika belum sampai posisi berhenti
    if (!playerReachedCenter) {
        walkTimer += deltaTime;
        playerWalkProgress = walkTimer / walkDuration;
        
        // Clamp progress antara 0.0 - 1.0
        if (playerWalkProgress > 1.0f) {
            playerWalkProgress = 1.0f;
        }
        
        // Cek jika sudah mencapai progress 1.0 (100%)
        if (playerWalkProgress >= 1.0f) {
            playerReachedCenter = true;
            // Ganti animasi menjadi P_IDLE (diam) setelah sampai
            PlayPlayerAnimation(P_IDLE, true);
            printf("[Gameplay] Player reached stopping position at X: %d\n", playerStopX);
        }
    } else if (!showDialog) {
        // Tampilkan dialog setelah player sampai di posisi berhenti
        // Beri delay 0.5 detik sebelum muncul dialog
        static float dialogDelay = 0.5f;
        dialogDelay -= deltaTime;
        
        if (dialogDelay <= 0.0f) {
            showDialog = true;
            printf("[Gameplay] Showing dialog box\n");
            dialogDelay = 0.5f; // Reset untuk next time
        }
    }
    
    // Handle dialog jika sudah muncul
    if (showDialog && dialogChoice == 0) {
        // Deteksi input untuk dialog
        if (IsKeyPressed(KEY_Y) || IsKeyPressed(KEY_ENTER)) {
            dialogChoice = 1; // Yes
            printf("[Gameplay] Player chose: YES\n");
        } else if (IsKeyPressed(KEY_N) || IsKeyPressed(KEY_ESCAPE)) {
            dialogChoice = 2; // No
            printf("[Gameplay] Player chose: NO\n");
        }
    }
    
    // Jika pilihan sudah dibuat, mulai fade out
    if (dialogChoice > 0) {
        fadeAlpha += deltaTime * 2.0f;
        
        if (fadeAlpha >= 1.0f) {
            fadeAlpha = 1.0f;
            
            if (dialogChoice == 1) {
                // Yes - Naik stage dan pindah ke BATTLE
                player.stage += 1;
                printf("[Gameplay] Stage increased to: %d\n", player.stage);
                
                // Load enemy untuk stage baru
                if (player.stage >= 1 && player.stage <= 10) {
                    currentEnemy = enemies[player.stage - 1];
                    printf("[Gameplay] Loading enemy: %s (HP: %d, Damage: %d)\n", 
                           currentEnemy.name, currentEnemy.HP, currentEnemy.damage);
                }
                
                gameplayNextState = BATTLE;
            } else if (dialogChoice == 2) {
                // No - Simpan score dan kembali ke LOBBY
                // Tentukan mode berdasarkan currentMode player
                const char* mode = (player.currentMode == MATH) ? "math" : "logic";
                saveScore(player.name, mode, player.score);
                printf("[Gameplay] Score saved: %d for %s in mode %s\n", 
                       player.score, player.name, mode);
                
                gameplayNextState = LOBBY;
            }
        }
    }
    
    return gameplayNextState;
}

// Fungsi untuk menggambar text box dialog
void DrawDialogBox() {
    if (!showDialog) return;
    
    // Posisi dialog di atas player (tengah atas layar)
    int dialogWidth = 600;
    int dialogHeight = 300;
    int dialogX = (GetScreenWidth() - dialogWidth) / 2;
    int dialogY = 150;
    
    // Background dialog dengan efek transparan
    DrawRectangle(dialogX, dialogY, dialogWidth, dialogHeight, Fade((Color){30, 30, 40, 240}, 0.95f));
    
    // Border dialog
    DrawRectangleLinesEx((Rectangle){(float)dialogX, (float)dialogY, (float)dialogWidth, (float)dialogHeight}, 
                        4, GOLD);
    
    // Teks pertanyaan berdasarkan stage
    const char* questionText = "";
    
    // Periksa stage dan atur teks yang sesuai
    if (player.stage == 1) {
        questionText = "Are You Ready To Start The EduQuest?";
    } else if (player.stage == 10) {
        questionText = "Are You Ready For The Boss Battle?";
    } else {
        questionText = "Are You Ready For The Next Stage?";
    }
    
    // Gambar teks pertanyaan
    int questionFontSize = 28;
    int questionWidth = MeasureText(questionText, questionFontSize);
    int questionX = dialogX + (dialogWidth - questionWidth) / 2;
    int questionY = dialogY + 60;
    
    DrawText(questionText, questionX + 2, questionY + 2, questionFontSize, BLACK);
    DrawText(questionText, questionX, questionY, questionFontSize, (Color){255, 255, 200, 255});
    
    // Gambar info stage di dalam dialog
    char stageInfo[50];
    snprintf(stageInfo, sizeof(stageInfo), "Stage: %d/10", player.stage);
    int stageWidth = MeasureText(stageInfo, 22);
    DrawText(stageInfo, dialogX + (dialogWidth - stageWidth)/2, 
             questionY + 40, 22, (Color){200, 200, 255, 255});
    
    // Tombol Yes
    int buttonWidth = 180;
    int buttonHeight = 60;
    int buttonY = dialogY + 150;
    int gap = 40;
    
    int yesButtonX = dialogX + (dialogWidth/2 - buttonWidth - gap/2);
    int noButtonX = dialogX + (dialogWidth/2 + gap/2);
    
    Rectangle yesButton = {(float)yesButtonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};
    Rectangle noButton = {(float)noButtonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};
    
    // Warna tombol berdasarkan hover
    Color yesColor = (Color){40, 180, 40, 220};
    Color noColor = (Color){180, 40, 40, 220};
    
    Vector2 mousePos = GetMousePosition();
    
    // Tombol Yes
    if (CheckCollisionPointRec(mousePos, yesButton)) {
        yesColor = (Color){60, 220, 60, 255};
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && dialogChoice == 0) {
            dialogChoice = 1;
        }
    }
    // Tombol No
    else if (CheckCollisionPointRec(mousePos, noButton)) {
        noColor = (Color){220, 60, 60, 255};
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && dialogChoice == 0) {
            dialogChoice = 2;
        }
    }
    
    // Gambar tombol Yes
    DrawRectangleRec(yesButton, yesColor);
    DrawRectangleLinesEx(yesButton, 3, (Color){100, 255, 100, 255});
    
    const char* yesText = "YES";
    int yesTextWidth = MeasureText(yesText, 26);
    DrawText(yesText, yesButtonX + (buttonWidth - yesTextWidth)/2, 
             buttonY + (buttonHeight - 26)/2, 26, WHITE);
    
    // Gambar tombol No
    DrawRectangleRec(noButton, noColor);
    DrawRectangleLinesEx(noButton, 3, (Color){255, 100, 100, 255});
    
    const char* noText = "NO";
    int noTextWidth = MeasureText(noText, 26);
    DrawText(noText, noButtonX + (buttonWidth - noTextWidth)/2, 
             buttonY + (buttonHeight - 26)/2, 26, WHITE);
    
    // Petunjuk keyboard
    const char* hintText = "Press Y/ENTER for Yes, N/ESC for No";
    int hintWidth = MeasureText(hintText, 18);
    DrawText(hintText, dialogX + (dialogWidth - hintWidth)/2, 
             dialogY + dialogHeight - 30, 18, (Color){200, 200, 150, 200});
}

// Draw gameplay
void DrawGameplay() {
    // Clear background
    ClearBackground(BLACK);
    
    // Gambar background gameplay
    DrawTexture(background, 0, 0, WHITE);
    
    // Hitung posisi player berdasarkan progress berjalan menggunakan interpolasi linear
    int currentPlayerX = playerStartX + (int)((playerStopX - playerStartX) * playerWalkProgress);
    
    // Gambar player dengan animasi di posisi yang sudah dihitung
    DrawPlayerSprite(currentPlayerX, playerY, playerScale);
    
    // Gambar frame overlay (layer paling atas)
    DrawTexture(frameOverlay, 0, 0, WHITE);
    
    // Gambar dialog jika sudah sampai di tengah
    if (showDialog) {
        DrawDialogBox();
    }
    
    // Gambar informasi stage di pojok kiri atas
    char stageText[50];
    snprintf(stageText, sizeof(stageText), "Stage: %d/10", player.stage);
    DrawText(stageText, 20, 20, 24, WHITE);
    
    // Gambar informasi player
    char playerInfo[100];
    snprintf(playerInfo, sizeof(playerInfo), "Player: %s", player.name);
    DrawText(playerInfo, 20, 50, 20, (Color){200, 200, 255, 255});
    
    // Gambar mode yang dipilih
    const char* modeText = (player.currentMode == MATH) ? "Mode: MATH" : "Mode: LOGIC";
    Color modeColor = (player.currentMode == MATH) ? (Color){255, 100, 100, 255} : (Color){100, 150, 255, 255};
    DrawText(modeText, 20, 80, 20, modeColor);
    
    // Gambar score
    char scoreText[50];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", player.score);
    DrawText(scoreText, 20, 110, 20, (Color){255, 255, 100, 255});
    
    // DEBUG: Gambar informasi posisi player (opsional, bisa dihapus)
    if (!playerReachedCenter) {
        char debugText[100];
        snprintf(debugText, sizeof(debugText), "Pos: (%d, %d) Progress: %.1f%%", 
                 currentPlayerX, playerY, playerWalkProgress * 100);
        DrawText(debugText, GetScreenWidth() - 300, 20, 18, GREEN);
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

// Fungsi untuk mengubah durasi perjalanan
void SetPlayerWalkDuration(float duration) {
    walkDuration = duration;
    printf("[Gameplay] Walk duration set to: %.1f seconds\n", walkDuration);
}

// Fungsi untuk mengubah posisi awal X
void SetPlayerStartX(int startX) {
    playerStartX = startX;
    printf("[Gameplay] Player start X set to: %d\n", playerStartX);
}

// Fungsi untuk mengubah posisi berhenti X
void SetPlayerStopX(int stopX) {
    playerStopX = stopX;
    printf("[Gameplay] Player stop X set to: %d\n", playerStopX);
}

// Fungsi untuk mengubah posisi Y
void SetPlayerY(int y) {
    playerY = y;
    printf("[Gameplay] Player Y position set to: %d\n", playerY);
}

// Fungsi untuk mengubah scale player
void SetPlayerScale(float scale) {
    playerScale = scale;
    printf("[Gameplay] Player scale set to: %.1f\n", playerScale);
}