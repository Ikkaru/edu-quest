#include "raylib.h"
#include "raygui.h"
#include "global.h"
#include "lobby.h"
#include "SaveSystem.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>  // Tambahkan ini untuk fungsi free()

// State internal lobby
GameState lobbyNextState = LOBBY;
playerHistory playerHistoryData = {0};
static Texture2D background;
static bool isHistoryLoaded = false;
static int selectedMode = 0; // 0 = Math, 1 = Logic

// Inisialisasi lobby
void InitLobby() {
    // Load background (sama dengan main menu)
    background = LoadTexture("assets/background/background_MainMenu.png");
    SetTextureFilter(background, TEXTURE_FILTER_POINT);
    
    // Reset state
    lobbyNextState = LOBBY;
    isHistoryLoaded = false;
    selectedMode = 0;
    
    printf("[Lobby] Initialized for player: %s\n", player.name);
}

// Update kondisi lobby
GameState UpdateLobby() {
    // Tidak ada logika khusus untuk update di sini
    // Semua interaksi ditangani dalam DrawLobby()
    return lobbyNextState;
}

// Fungsi untuk menampilkan riwayat skor
void DrawScoreHistory() {
    if (playerHistoryData.count == 0) {
        DrawText("No scores yet!", 100, 400, 24, GRAY);
        return;
    }
    
    // Gambar panel untuk riwayat skor
    DrawRectangle(800, 300, 400, 300, Fade(BLACK, 0.7f));
    GuiLabel((Rectangle){810, 310, 380, 30}, "Score History");
    
    // Tampilkan beberapa skor terakhir
    int maxToShow = 8; // Maksimal 8 skor terakhir
    int startIdx = (playerHistoryData.count > maxToShow) ? playerHistoryData.count - maxToShow : 0;
    int shownCount = playerHistoryData.count - startIdx;
    
    for (int i = 0; i < shownCount; i++) {
        int idx = startIdx + i;
        char scoreText[50];
        snprintf(scoreText, sizeof(scoreText), "#%d: %d points", i + 1, playerHistoryData.scores[idx]);
        
        DrawText(scoreText, 820, 350 + (i * 30), 20, WHITE);
    }
    
    // Tampilkan statistik
    char statsText[100];
    if (playerHistoryData.count > 0) {
        int total = 0;
        int highest = playerHistoryData.scores[0];
        for (int i = 0; i < playerHistoryData.count; i++) {
            total += playerHistoryData.scores[i];
            if (playerHistoryData.scores[i] > highest) {
                highest = playerHistoryData.scores[i];
            }
        }
        float average = (float)total / playerHistoryData.count;
        
        snprintf(statsText, sizeof(statsText), "Average: %.1f | Highest: %d", average, highest);
        DrawText(statsText, 820, 350 + (shownCount * 30) + 20, 20, YELLOW);
    }
}

// Draw lobby
void DrawLobby() {
    // Load background
    ClearBackground(BLACK);
    DrawTexture(background, 0, 0, WHITE);
    
    // Title
    const char* title = "GAME MODE";
    int titleFontSize = 60;
    int titleWidth = MeasureText(title, titleFontSize);
    int titleX = (GetScreenWidth() - titleWidth) / 2;
    int titleY = 80;
    
    // Draw title dengan efek bayangan untuk keterbacaan
    DrawText(title, titleX + 3, titleY + 3, titleFontSize, BLACK);
    DrawText(title, titleX, titleY, titleFontSize, RED);
    
    // Tombol mode
    int buttonWidth = 300;
    int buttonHeight = 100;
    int buttonY = 250;
    int gap = 50;
    
    // Hitung posisi tombol agar berada di tengah
    int totalWidth = (buttonWidth * 2) + gap;
    int startX = (GetScreenWidth() - totalWidth) / 2;
    
    // Tombol Math
    Rectangle mathBtn = {startX, buttonY, buttonWidth, buttonHeight};
    Rectangle logicBtn = {startX + buttonWidth + gap, buttonY, buttonWidth, buttonHeight};
    
    // Warna tombol berdasarkan seleksi
    Color mathColor = (selectedMode == 0) ? SKYBLUE : DARKBLUE;
    Color logicColor = (selectedMode == 1) ? SKYBLUE : DARKBLUE;
    
    // Gambar tombol Math
    DrawRectangleRec(mathBtn, mathColor);
    DrawRectangleLinesEx(mathBtn, 3, WHITE);
    DrawText("MATH", 
             mathBtn.x + (mathBtn.width - MeasureText("MATH", 40)) / 2,
             mathBtn.y + (mathBtn.height - 40) / 2,
             40, WHITE);
    
    // Gambar tombol Logic
    DrawRectangleRec(logicBtn, logicColor);
    DrawRectangleLinesEx(logicBtn, 3, WHITE);
    DrawText("LOGIC",
             logicBtn.x + (logicBtn.width - MeasureText("LOGIC", 40)) / 2,
             logicBtn.y + (logicBtn.height - 40) / 2,
             40, WHITE);
    
    // Deteksi klik pada tombol
    Vector2 mousePos = GetMousePosition();
    
    // Hover effect dan deteksi klik untuk tombol Math
    if (CheckCollisionPointRec(mousePos, mathBtn)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            selectedMode = 0;
            player.currentMode = MATH;
            
            // Load score history untuk mode Math
            if (playerHistoryData.scores != NULL) {
                free(playerHistoryData.scores);
            }
            playerHistoryData = loadScore(player.name, "math");
            isHistoryLoaded = true;
            
            printf("[Lobby] Loaded Math history for %s. Total scores: %d\n", 
                   player.name, playerHistoryData.count);
        }
    }
    // Hover effect dan deteksi klik untuk tombol Logic
    else if (CheckCollisionPointRec(mousePos, logicBtn)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            selectedMode = 1;
            player.currentMode = LOGIC;
            
            // Load score history untuk mode Logic
            if (playerHistoryData.scores != NULL) {
                free(playerHistoryData.scores);
            }
            playerHistoryData = loadScore(player.name, "logic");
            isHistoryLoaded = true;
            
            printf("[Lobby] Loaded Logic history for %s. Total scores: %d\n", 
                   player.name, playerHistoryData.count);
        }
    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
    
    // Informasi player
    char playerInfo[100];
    snprintf(playerInfo, sizeof(playerInfo), "Player: %s", player.name);
    DrawText(playerInfo, 50, 180, 30, WHITE);
    
    // Mode yang dipilih
    const char* modeText = (selectedMode == 0) ? "Selected Mode: MATH" : "Selected Mode: LOGIC";
    Color modeColor = (selectedMode == 0) ? GREEN : ORANGE;
    DrawText(modeText, 50, 220, 25, modeColor);
    
    // Tampilkan riwayat skor jika sudah memilih mode
    if (isHistoryLoaded) {
        DrawScoreHistory();
    }
    
    // Tombol back ke main menu
    if (GuiButton((Rectangle){50, GetScreenHeight() - 100, 200, 50}, "Back to Menu")) {
        lobbyNextState = MAIN_MENU;
    }
    
    // Tombol mulai game
    if (isHistoryLoaded) {
        const char* startText = (selectedMode == 0) ? "Start Math Game" : "Start Logic Game";
        if (GuiButton((Rectangle){GetScreenWidth() - 250, GetScreenHeight() - 100, 200, 50}, startText)) {
            // Di sini Anda bisa mengatur state berikutnya ke GAMEPLAY
            // Untuk sekarang, kita kembali ke main menu dulu
            lobbyNextState = MAIN_MENU;
            printf("[Lobby] Starting %s game for %s\n", 
                   (selectedMode == 0) ? "Math" : "Logic", player.name);
        }
    }
}

// Unload resources lobby
void UnloadLobby() {
    UnloadTexture(background);
    
    // Free allocated memory untuk score history
    if (playerHistoryData.scores != NULL) {
        free(playerHistoryData.scores);
        playerHistoryData.scores = NULL;
        playerHistoryData.count = 0;
    }
    
    printf("[Lobby] Resources unloaded\n");
}