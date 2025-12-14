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

// ============ VARIABEL YANG BISA DIKUSTOMISASI ============
static float walkDuration = 2.5f;          // Waktu yang dibutuhkan untuk sampai ke tengah
static int playerStartX = -500;            // Posisi X awal (di luar layar kiri)
static int playerStopX = 250;              // Posisi X berhenti (tengah layar)
static int playerY = 100;                  // Posisi Y player
static float playerScale = 7.0f;           // Scale ukuran player sprite
static float exitDuration = 2.5f;          // Waktu untuk keluar layar kanan
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
    
    // Reset state
    gameplayNextState = GAMEPLAY;
    fadeAlpha = 1.0f;
    fadeIn = true;
    playerReachedCenter = false;
    showDialog = false;
    dialogChoice = 0;
    playerWalkProgress = 0.0f;
    playerExiting = false;
    exitProgress = 0.0f;
    walkTimer = 0.0f;
    exitTimer = 0.0f;
    
    printf("[Gameplay] Initialized for player: %s, Stage: %d\n", player.name, player.stage);
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
    
    // Jika player sedang keluar (setelah klik YES)
    if (playerExiting) {
        exitTimer += deltaTime;
        exitProgress = exitTimer / exitDuration;
        
        if (exitProgress >= 1.0f) {
            exitProgress = 1.0f;
            
            // Pindah ke BATTLE state
            player.stage += 1;
            printf("[Gameplay] Stage increased to: %d\n", player.stage);
            
            // Load enemy untuk stage baru
            if (player.stage >= 1 && player.stage <= 10) {
                currentEnemy = enemies[player.stage - 1];
                printf("[Gameplay] Loading enemy: %s\n", currentEnemy.name);
            }
            
            gameplayNextState = BATTLE;
        }
        
        return gameplayNextState;
    }
    
    // Update pergerakan player masuk jika belum sampai posisi berhenti
    if (!playerReachedCenter && !playerExiting) {
        walkTimer += deltaTime;
        playerWalkProgress = walkTimer / walkDuration;
        
        if (playerWalkProgress > 1.0f) {
            playerWalkProgress = 1.0f;
        }
        
        if (playerWalkProgress >= 1.0f) {
            playerReachedCenter = true;
            // Ganti animasi menjadi P_IDLE (diam) setelah sampai
            PlayPlayerAnimation(P_IDLE, true);
        }
    } else if (!showDialog && playerReachedCenter && !playerExiting) {
        // Tampilkan dialog setelah player sampai di posisi berhenti
        static float dialogDelay = 0.5f;
        dialogDelay -= deltaTime;
        
        if (dialogDelay <= 0.0f) {
            showDialog = true;
            dialogDelay = 0.5f;
        }
    }
    
    // Handle dialog jika sudah muncul
    if (showDialog && dialogChoice == 0 && !playerExiting) {
        // Deteksi input untuk dialog
        if (IsKeyPressed(KEY_Y) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_RIGHT)) {
            dialogChoice = 1; // Yes
            printf("[Gameplay] Player chose: YES - Starting exit animation\n");
            
            // Mulai animasi keluar
            playerExiting = true;
            PlayPlayerAnimation(P_RUN, true);
            exitTimer = 0.0f;
            exitProgress = 0.0f;
        } else if (IsKeyPressed(KEY_N) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_LEFT)) {
            dialogChoice = 2; // No
            printf("[Gameplay] Player chose: NO - Returning to lobby\n");
            
            // Simpan score dan langsung fade out
            const char* mode = (player.currentMode == MATH) ? "math" : "logic";
            saveScore(player.name, mode, player.score);
            printf("[Gameplay] Score saved: %d for %s in mode %s\n", 
                   player.score, player.name, mode);
            
            gameplayNextState = LOBBY;
        }
    }
    
    // Jika pilihan NO (fade out ke lobby)
    if (dialogChoice == 2 && !playerExiting) {
        fadeAlpha += deltaTime * 2.0f;
        
        if (fadeAlpha >= 1.0f) {
            fadeAlpha = 1.0f;
            gameplayNextState = LOBBY;
        }
    }
    
    return gameplayNextState;
}

// Fungsi untuk menggambar text box dialog di atas layar
void DrawDialogBox() {
    if (!showDialog || playerExiting) return;
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Posisi dialog di atas layar mepet dengan garis tepi atas
    int dialogWidth = screenWidth - 100;  // Lebar hampir full layar
    int dialogHeight = 120;               // Tinggi lebih pendek
    int dialogX = (screenWidth - dialogWidth) / 2;
    int dialogY = 20;                     // Mepet atas
    
    // Background dialog semi-transparan
    DrawRectangle(dialogX, dialogY, dialogWidth, dialogHeight, Fade((Color){20, 20, 30, 230}, 0.9f));
    
    // Border atas dan bawah saja
    DrawLine(dialogX, dialogY, dialogX + dialogWidth, dialogY, GOLD);
    DrawLine(dialogX, dialogY + dialogHeight, dialogX + dialogWidth, dialogY + dialogHeight, GOLD);
    
    // Teks pertanyaan berdasarkan stage
    const char* questionText = "";
    
    if (player.stage == 1) {
        questionText = "Are You Ready To Start The EduQuest?";
    } else if (player.stage == 10) {
        questionText = "Are You Ready For The Boss Battle?";
    } else {
        questionText = "Are You Ready For The Next Stage?";
    }
    
    // Gambar teks pertanyaan di tengah dialog
    int questionFontSize = 26;
    int questionWidth = MeasureText(questionText, questionFontSize);
    int questionX = dialogX + (dialogWidth - questionWidth) / 2;
    int questionY = dialogY + 25;
    
    DrawText(questionText, questionX, questionY, questionFontSize, (Color){255, 255, 200, 255});
    
    // Gambar info stage kecil di bawah pertanyaan
    char stageInfo[50];
    snprintf(stageInfo, sizeof(stageInfo), "Stage %d/10", player.stage);
    int stageWidth = MeasureText(stageInfo, 20);
    DrawText(stageInfo, dialogX + (dialogWidth - stageWidth)/2, 
             questionY + 35, 20, (Color){200, 200, 255, 255});
    
    // Tombol Yes di KANAN layar
    int buttonWidth = 150;
    int buttonHeight = 50;
    int buttonY = dialogY + dialogHeight + 20; // Di bawah dialog
    
    // YES button di kanan
    int yesButtonX = screenWidth - buttonWidth - 50;
    Rectangle yesButton = {(float)yesButtonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};
    
    // NO button di kiri
    int noButtonX = 50;
    Rectangle noButton = {(float)noButtonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};
    
    // Warna tombol
    Color yesColor = (Color){40, 180, 40, 220};
    Color noColor = (Color){180, 40, 40, 220};
    
    Vector2 mousePos = GetMousePosition();
    
    // Tombol Yes (kanan)
    if (CheckCollisionPointRec(mousePos, yesButton)) {
        yesColor = (Color){60, 220, 60, 255};
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && dialogChoice == 0) {
            dialogChoice = 1;
            playerExiting = true;
            PlayPlayerAnimation(P_RUN, true);
        }
    }
    // Tombol No (kiri)
    else if (CheckCollisionPointRec(mousePos, noButton)) {
        noColor = (Color){220, 60, 60, 255};
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && dialogChoice == 0) {
            dialogChoice = 2;
        }
    }
    
    // Gambar tombol Yes (kanan) dengan panah →
    DrawRectangleRec(yesButton, yesColor);
    DrawRectangleLinesEx(yesButton, 2, (Color){100, 255, 100, 255});
    
    const char* yesText = "YES →";
    int yesTextWidth = MeasureText(yesText, 22);
    DrawText(yesText, yesButtonX + (buttonWidth - yesTextWidth)/2, 
             buttonY + (buttonHeight - 22)/2, 22, WHITE);
    
    // Gambar tombol No (kiri) dengan panah ←
    DrawRectangleRec(noButton, noColor);
    DrawRectangleLinesEx(noButton, 2, (Color){255, 100, 100, 255});
    
    const char* noText = "← NO";
    int noTextWidth = MeasureText(noText, 22);
    DrawText(noText, noButtonX + (buttonWidth - noTextWidth)/2, 
             buttonY + (buttonHeight - 22)/2, 22, WHITE);
    
    // Petunjuk keyboard di tengah bawah
    const char* hintText = "Press LEFT/← for No, RIGHT/→ for Yes";
    int hintWidth = MeasureText(hintText, 18);
    DrawText(hintText, (screenWidth - hintWidth)/2, 
             buttonY + buttonHeight + 10, 18, (Color){200, 200, 150, 200});
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
        // Animasi keluar: dari posisi stop ke luar layar kanan
        int exitStartX = playerStopX;
        int exitEndX = screenWidth + 200; // Di luar layar kanan
        currentPlayerX = exitStartX + (int)((exitEndX - exitStartX) * exitProgress);
    } else {
        // Animasi masuk: dari kiri ke posisi stop
        currentPlayerX = playerStartX + (int)((playerStopX - playerStartX) * playerWalkProgress);
    }
    
    // Gambar player dengan animasi
    DrawPlayerSprite(currentPlayerX, playerY, playerScale);
    
    // Gambar frame overlay (layer paling atas)
    DrawTexture(frameOverlay, 0, 0, WHITE);
    
    // Gambar dialog jika sudah sampai di tengah dan belum keluar
    if (showDialog && !playerExiting) {
        DrawDialogBox();
    }
    
    // HAPUS INFORMASI DI POJOK KIRI ATAS (sesuai permintaan)
    // Tidak ada DrawText untuk stage, player info, mode, atau score di sini
    
    // Gambar transisi fade
    if (fadeAlpha > 0.0f) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), 
                     Fade(BLACK, fadeAlpha));
    }
    
    // Jika sedang keluar, tambahkan fade in bertahap
    if (playerExiting) {
        float exitFade = exitProgress * 0.7f; // 70% opacity maksimal
        if (exitFade > 0.0f) {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), 
                         Fade(BLACK, exitFade));
        }
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

// Fungsi untuk mengubah durasi keluar
void SetPlayerExitDuration(float duration) {
    exitDuration = duration;
    printf("[Gameplay] Exit duration set to: %.1f seconds\n", exitDuration);
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