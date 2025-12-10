#include "raylib.h"
#include "raygui.h"
#include "global.h"
#include "lobby.h"
#include "SaveSystem.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// State internal lobby
GameState lobbyNextState = LOBBY;
playerHistory playerHistoryData = {0};
static Texture2D background;
static bool isHistoryLoaded = false;
static int selectedMode = 0; // 0 = Math, 1 = Logic

// Variabel untuk animasi
static float waveTime = 0.0f;
static float buttonBobTime = 0.0f;
static float colorPulseTime = 0.0f;

// Warna custom
#ifndef SILVER
#define SILVER CLITERAL(Color){ 192, 192, 192, 255 }
#endif

// Cek apakah GOLD sudah didefinisikan di raylib.h
#ifdef GOLD
#undef GOLD
#endif
#define GOLD CLITERAL(Color){ 255, 215, 0, 255 }

// Fungsi untuk mengurutkan skor secara descending (terbesar ke terkecil)
static void SortScoresDescending(int* scores, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (scores[j] < scores[j + 1]) {
                // Tukar posisi
                int temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }
}

// Fungsi helper untuk membersihkan history
static void ClearHistory() {
    if (playerHistoryData.scores != NULL) {
        free(playerHistoryData.scores);
        playerHistoryData.scores = NULL;
        playerHistoryData.count = 0;
    }
}

// Fungsi untuk menggambar busur lingkaran
static void DrawArc(int centerX, int centerY, float radius, float startAngle, float endAngle, Color color) {
    int segments = 36;
    float step = (endAngle - startAngle) / (float)segments;
    float angle = startAngle;
    
    for (int i = 0; i < segments; i++) {
        float nextAngle = angle + step;
        
        Vector2 startPos = {
            centerX + cosf(angle) * radius,
            centerY + sinf(angle) * radius
        };
        
        Vector2 endPos = {
            centerX + cosf(nextAngle) * radius,
            centerY + sinf(nextAngle) * radius
        };
        
        DrawLineV(startPos, endPos, color);
        angle = nextAngle;
    }
}

// Inisialisasi lobby
void InitLobby() {
    // Load background (sama dengan main menu)
    background = LoadTexture("assets/background/background_MainMenu.png");
    SetTextureFilter(background, TEXTURE_FILTER_POINT);
    
    // Reset state
    lobbyNextState = LOBBY;
    isHistoryLoaded = false;
    selectedMode = 0;
    
    // Reset animasi
    waveTime = 0.0f;
    buttonBobTime = 0.0f;
    colorPulseTime = 0.0f;
    
    // Bersihkan history sebelumnya
    ClearHistory();
    
    printf("[Lobby] Initialized for player: %s\n", player.name);
}

// Update kondisi lobby
GameState UpdateLobby() {
    // Update waktu untuk animasi
    waveTime += GetFrameTime() * 2.0f;
    buttonBobTime += GetFrameTime() * 1.5f;
    colorPulseTime += GetFrameTime();
    
    return lobbyNextState;
}

// Fungsi untuk menggambar panel Select Game Mode di pojok kiri atas
void DrawGameModePanel() {
    // Panel untuk game mode di pojok kiri atas
    int panelX = 50;
    int panelY = 50;
    int panelWidth = 500;
    int panelHeight = 500;
    
    // Background panel abu-abu
    DrawRectangle(panelX, panelY, panelWidth, panelHeight, (Color){60, 60, 70, 240});
    DrawRectangleLinesEx((Rectangle){(float)panelX, (float)panelY, (float)panelWidth, (float)panelHeight}, 
                        3, (Color){100, 100, 100, 255});
    
    // Title untuk panel game mode
    const char* panelTitle = "SELECT GAME MODE";
    int titleFontSize = 32;
    int titleWidth = MeasureText(panelTitle, titleFontSize);
    int titleX = panelX + (panelWidth - titleWidth) / 2;
    int titleY = panelY + 30;
    
    // Efek gelombang untuk title
    float titleWaveOffset = sinf(waveTime) * 3.0f;
    DrawText(panelTitle, titleX + 2, titleY + titleWaveOffset + 2, titleFontSize, BLACK);
    DrawText(panelTitle, titleX, titleY + titleWaveOffset, titleFontSize, GOLD);
    
    // Garis dekoratif di bawah title
    DrawLine(panelX + 50, titleY + 40, panelX + panelWidth - 50, titleY + 40, (Color){150, 150, 150, 150});
    
    // Posisi tombol di tengah panel
    int buttonWidth = 400;
    int buttonHeight = 120;
    int buttonY = panelY + 100;
    int gap = 30;
    
    // Hitung posisi tombol agar berada di tengah panel
    int startX = panelX + (panelWidth - buttonWidth) / 2;
    
    // Tombol Math (atas) - Warna merah
    Rectangle mathBtn = {(float)startX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};
    
    // Tombol Logic (bawah) - Warna biru
    Rectangle logicBtn = {(float)startX, (float)(buttonY + buttonHeight + gap), (float)buttonWidth, (float)buttonHeight};
    
    // Efek animasi naik turun bergantian
    float mathBobOffset = sinf(buttonBobTime) * 5.0f;
    float logicBobOffset = sinf(buttonBobTime + 1.5f) * 5.0f;
    
    // Warna tombol berdasarkan seleksi
    Color mathColor = (selectedMode == 0) ? (Color){220, 60, 60, 240} : (Color){180, 40, 40, 200};
    Color logicColor = (selectedMode == 1) ? (Color){60, 120, 220, 240} : (Color){40, 80, 180, 200};
    
    // Gambar tombol Math dengan efek
    DrawRectangleRec((Rectangle){mathBtn.x, mathBtn.y + mathBobOffset, mathBtn.width, mathBtn.height}, mathColor);
    DrawRectangleLinesEx((Rectangle){mathBtn.x, mathBtn.y + mathBobOffset, mathBtn.width, mathBtn.height}, 
                        3, (Color){255, 100, 100, 255});
    
    // Teks untuk tombol Math
    DrawText("MATH", (int)(mathBtn.x + (mathBtn.width - MeasureText("MATH", 50)) / 2),
             (int)(mathBtn.y + mathBobOffset + 30), 50, WHITE);
    
    // Gambar tombol Logic dengan efek
    DrawRectangleRec((Rectangle){logicBtn.x, logicBtn.y + logicBobOffset, logicBtn.width, logicBtn.height}, logicColor);
    DrawRectangleLinesEx((Rectangle){logicBtn.x, logicBtn.y + logicBobOffset, logicBtn.width, logicBtn.height}, 
                        3, (Color){100, 150, 255, 255});
    
    // Teks untuk tombol Logic
    DrawText("LOGIC", (int)(logicBtn.x + (logicBtn.width - MeasureText("LOGIC", 50)) / 2),
             (int)(logicBtn.y + logicBobOffset + 30), 50, WHITE);
    
    // Deskripsi di bawah tombol
    const char* description = "Click a mode to view score history";
    DrawText(description, panelX + (panelWidth - MeasureText(description, 18)) / 2,
             panelY + panelHeight - 40, 18, (Color){200, 200, 200, 200});
    
    // Deteksi klik pada tombol
    Vector2 mousePos = GetMousePosition();
    
    // Hover effect dan deteksi klik untuk tombol Math
    if (CheckCollisionPointRec(mousePos, mathBtn)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            selectedMode = 0;
            player.currentMode = MATH;
            
            // Load score history untuk mode Math
            ClearHistory();
            playerHistoryData = loadScore(player.name, "math");
            
            // Urutkan skor dari terbesar ke terkecil
            if (playerHistoryData.scores != NULL && playerHistoryData.count > 0) {
                SortScoresDescending(playerHistoryData.scores, playerHistoryData.count);
            }
            
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
            ClearHistory();
            playerHistoryData = loadScore(player.name, "logic");
            
            // Urutkan skor dari terbesar ke terkecil
            if (playerHistoryData.scores != NULL && playerHistoryData.count > 0) {
                SortScoresDescending(playerHistoryData.scores, playerHistoryData.count);
            }
            
            isHistoryLoaded = true;
            
            printf("[Lobby] Loaded Logic history for %s. Total scores: %d\n", 
                   player.name, playerHistoryData.count);
        }
    }
}

// Fungsi untuk menggambar panel Score History di sebelah kanan
void DrawScoreHistoryPanel() {
    // Panel untuk history player di sebelah kanan
    int panelX = GetScreenWidth() - 550;
    int panelY = 50;
    int panelWidth = 500;
    int panelHeight = 500;
    
    // Background panel abu-abu
    DrawRectangle(panelX, panelY, panelWidth, panelHeight, (Color){60, 60, 70, 240});
    DrawRectangleLinesEx((Rectangle){(float)panelX, (float)panelY, (float)panelWidth, (float)panelHeight}, 
                        3, (Color){100, 100, 100, 255});
    
    // Title untuk panel history
    const char* panelTitle = "SCORE HISTORY";
    int titleFontSize = 32;
    int titleWidth = MeasureText(panelTitle, titleFontSize);
    int titleX = panelX + (panelWidth - titleWidth) / 2;
    int titleY = panelY + 30;
    
    // Efek gelombang untuk title history
    float historyWaveOffset = sinf(waveTime + 1.0f) * 3.0f;
    DrawText(panelTitle, titleX + 2, titleY + historyWaveOffset + 2, titleFontSize, BLACK);
    DrawText(panelTitle, titleX, titleY + historyWaveOffset, titleFontSize, GOLD);
    
    // Garis dekoratif di bawah title
    DrawLine(panelX + 50, titleY + 40, panelX + panelWidth - 50, titleY + 40, (Color){150, 150, 150, 150});
    
    // Konten history
    int contentX = panelX + 20;
    int contentY = panelY + 80;
    int contentWidth = panelWidth - 40;
    int contentHeight = panelHeight - 120;
    
    if (!isHistoryLoaded) {
        // Pesan jika belum memilih mode
        const char* noSelectionText = " Select a game mode\nto view score history";
        int textWidth = MeasureText("Select a game mode", 24);
        DrawText(noSelectionText, panelX + (panelWidth - textWidth) / 2,
                 panelY + panelHeight / 2 - 30, 24, (Color){150, 150, 150, 200});
        
        // Ikon panah arah kiri
        DrawTriangle((Vector2){(float)(panelX + panelWidth/2 - 30), (float)(panelY + panelHeight/2 + 30)},
                     (Vector2){(float)(panelX + panelWidth/2 + 10), (float)(panelY + panelHeight/2 + 10)},
                     (Vector2){(float)(panelX + panelWidth/2 + 10), (float)(panelY + panelHeight/2 + 50)},
                     (Color){200, 200, 100, 150});
        return;
    }
    
    if (playerHistoryData.count == 0) {
        // Pesan jika tidak ada skor
        const char* noScoreText = "No scores yet !\n Play games to \n  earn scores";
        int textWidth = MeasureText("No scores yet!", 28);
        DrawText(noScoreText, panelX + (panelWidth - textWidth) / 2,
                 panelY + panelHeight / 2 - 30, 28, (Color){200, 200, 200, 200});
        
        return;
    }
    
    // Area untuk daftar skor
    int listX = contentX;
    int listY = contentY;
    int listWidth = contentWidth;
    int listHeight = contentHeight - 80; // Sisakan ruang untuk statistik
    
    // Header tabel - ditengah
    DrawRectangle(listX, listY, listWidth, 40, (Color){50, 50, 60, 220});
    DrawRectangleLinesEx((Rectangle){(float)listX, (float)listY, (float)listWidth, 40.0f}, 
                        2, (Color){100, 100, 120, 150});
    
    // Hitung posisi teks agar ditengah
    int col1Center = listX + 60;
    int col2Center = listX + listWidth/2;
    int col3Center = listX + listWidth - 60;
    
    DrawText("Rank", col1Center - MeasureText("Rank", 22)/2, listY + 10, 22, (Color){220, 220, 255, 255});
    DrawText("Score", col2Center - MeasureText("Score", 22)/2, listY + 10, 22, (Color){220, 220, 255, 255});
    DrawText("Date", col3Center - MeasureText("Date", 22)/2, listY + 10, 22, (Color){220, 220, 255, 255});
    
    // Tampilkan beberapa skor teratas
    int maxToShow = 7;
    int shownCount = (playerHistoryData.count > maxToShow) ? maxToShow : playerHistoryData.count;
    
    int listStartY = listY + 50;
    int rowHeight = 35;
    
    for (int i = 0; i < shownCount; i++) {
        // Warna bergantian untuk baris
        Color rowColor = (i % 2 == 0) ? (Color){70, 70, 80, 180} : (Color){80, 80, 90, 180};
        
        // Warna khusus untuk peringkat 1, 2, 3
        if (i == 0) rowColor = (Color){80, 60, 40, 200};    // Emas
        else if (i == 1) rowColor = (Color){70, 70, 70, 200}; // Perak
        else if (i == 2) rowColor = (Color){100, 60, 40, 200}; // Perunggu
        
        DrawRectangle(listX, listStartY + (i * rowHeight), listWidth, rowHeight, rowColor);
        
        // Border untuk baris
        DrawRectangleLinesEx((Rectangle){(float)listX, (float)(listStartY + (i * rowHeight)), 
                                        (float)listWidth, (float)rowHeight}, 1, (Color){100, 100, 120, 100});
        
        // Peringkat - ditengah
        char rankText[10];
        snprintf(rankText, sizeof(rankText), "#%d", i + 1);
        int rankWidth = MeasureText(rankText, 20);
        DrawText(rankText, col1Center - rankWidth/2, listStartY + (i * rowHeight) + 8, 20, WHITE);
        
        // Skor - ditengah (sudah terurut dari terbesar)
        char scoreText[20];
        snprintf(scoreText, sizeof(scoreText), "%d", playerHistoryData.scores[i]);
        int scoreWidth = MeasureText(scoreText, 20);
        
        // Warna skor berdasarkan peringkat
        Color scoreColor = WHITE;
        if (i == 0) scoreColor = GOLD;
        else if (i == 1) scoreColor = SILVER;
        else if (i == 2) scoreColor = (Color){205, 127, 50, 255}; // Perunggu
        
        DrawText(scoreText, col2Center - scoreWidth/2, listStartY + (i * rowHeight) + 8, 20, scoreColor);
        
        // Tanggal dummy - ditengah
        const char* dateText = "Today";
        int dateWidth = MeasureText(dateText, 20);
        DrawText(dateText, col3Center - dateWidth/2, listStartY + (i * rowHeight) + 8, 20, (Color){180, 180, 180, 255});
        
        // Emoji berdasarkan peringkat
        if (i == 0) {
            const char* trophy = "🥇";
            DrawText(trophy, listX + listWidth - 30, listStartY + (i * rowHeight) + 5, 25, GOLD);
        } else if (i == 1) {
            const char* silverMedal = "🥈";
            DrawText(silverMedal, listX + listWidth - 30, listStartY + (i * rowHeight) + 5, 25, SILVER);
        } else if (i == 2) {
            const char* bronzeMedal = "🥉";
            DrawText(bronzeMedal, listX + listWidth - 30, listStartY + (i * rowHeight) + 5, 25, (Color){205, 127, 50, 255});
        }
    }
    
    // Statistik di bagian bawah
    if (playerHistoryData.count > 0) {
        int statsY = listStartY + (shownCount * rowHeight) + 20;
        
        // Hitung statistik dari data yang sudah diurutkan
        int total = 0;
        int highest = playerHistoryData.scores[0]; // Peringkat 1
        int lowest = playerHistoryData.scores[playerHistoryData.count - 1]; // Peringkat terakhir
        
        for (int i = 0; i < playerHistoryData.count; i++) {
            total += playerHistoryData.scores[i];
        }
        
        float average = (float)total / playerHistoryData.count;
        
        // Panel statistik
        DrawRectangle(listX, statsY, listWidth, 80, (Color){50, 50, 60, 220});
        DrawRectangleLinesEx((Rectangle){(float)listX, (float)statsY, (float)listWidth, 80.0f}, 
                            2, (Color){100, 100, 120, 150});
        
        // Gambar statistik - ditengah
        char statsText[4][50];
        snprintf(statsText[0], sizeof(statsText[0]), "Total: %d", playerHistoryData.count);
        snprintf(statsText[1], sizeof(statsText[1]), "Avg: %.1f", average);
        snprintf(statsText[2], sizeof(statsText[2]), "High: %d", highest);
        snprintf(statsText[3], sizeof(statsText[3]), "Low: %d", lowest);
        
        for (int i = 0; i < 4; i++) {
            int textWidth = MeasureText(statsText[i], 18);
            DrawText(statsText[i], listX + (listWidth/4 * i) + (listWidth/4 - textWidth)/2, 
                     statsY + 15, 18, (Color){180, 220, 255, 255});
        }
    }
}

// Fungsi untuk menggambar informasi player di tengah bawah
void DrawPlayerInfoCenter() {
    // Panel informasi player di tengah bawah
    int panelWidth = 400;
    int panelHeight = 80;
    int panelX = (GetScreenWidth() - panelWidth) / 2;
    int panelY = GetScreenHeight() - 150;
    
    // Background panel
    DrawRectangle(panelX, panelY, panelWidth, panelHeight, (Color){40, 40, 50, 200});
    DrawRectangleLinesEx((Rectangle){(float)panelX, (float)panelY, (float)panelWidth, (float)panelHeight}, 
                        2, (Color){100, 100, 150, 200});
    
    // Nama player - ditengah
    char playerInfo[100];
    snprintf(playerInfo, sizeof(playerInfo), "Player: %s", player.name);
    int nameWidth = MeasureText(playerInfo, 24);
    DrawText(playerInfo, panelX + (panelWidth - nameWidth) / 2, panelY + 15, 24, (Color){220, 240, 255, 255});
    
    // Mode yang dipilih - ditengah
    const char* modeText = (selectedMode == 0) ? "Selected Mode: MATH" : "Selected Mode: LOGIC";
    Color modeColor = (selectedMode == 0) ? (Color){255, 100, 100, 255} : (Color){100, 150, 255, 255};
    int modeWidth = MeasureText(modeText, 20);
    DrawText(modeText, panelX + (panelWidth - modeWidth) / 2, panelY + 45, 20, modeColor);
}

// Fungsi untuk menggambar tombol dengan teks yang jelas
void DrawButtonWithText(const char* text, Rectangle bounds, Color bgColor, Color borderColor, Color textColor, int fontSize) {
    // Gambar tombol
    DrawRectangleRec(bounds, bgColor);
    DrawRectangleLinesEx(bounds, 2, borderColor);
    
    // Hitung posisi teks agar ditengah
    int textWidth = MeasureText(text, fontSize);
    int textX = (int)(bounds.x + (bounds.width - textWidth) / 2);
    int textY = (int)(bounds.y + (bounds.height - fontSize) / 2);
    
    // Gambar teks dengan bayangan untuk keterbacaan
    DrawText(text, textX + 1, textY + 1, fontSize, Fade(BLACK, 0.5f));
    DrawText(text, textX, textY, fontSize, textColor);
}

// Draw lobby
void DrawLobby() {
    // Load background
    ClearBackground(BLACK);
    DrawTexture(background, 0, 0, WHITE);
    
    // Gambar efek overlay gelap ringan
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 50});
    
    // Gambar garis pemisah vertikal di tengah layar
    int centerX = GetScreenWidth() / 2;
    for (int y = 50; y < GetScreenHeight() - 200; y += 4) {
        float wave = sinf(waveTime * 2 + y * 0.02f) * 3.0f;
        DrawPixel(centerX + (int)wave, y, (Color){100, 100, 150, 100});
    }
    
    // Gambar panel Select Game Mode di pojok kiri atas
    DrawGameModePanel();
    
    // Gambar panel Score History di sebelah kanan
    DrawScoreHistoryPanel();
    
    // Gambar informasi player di tengah bawah
    DrawPlayerInfoCenter();
    
    // Tombol Back to Main Menu di pojok kiri bawah - WARNA MERAH
    int backBtnX = 110;
    int backBtnY = GetScreenHeight() - 140;
    int backBtnWidth = 240;
    int backBtnHeight = 70;
    
    Rectangle backBtnRect = {(float)backBtnX, (float)backBtnY, (float)backBtnWidth, (float)backBtnHeight};
    
    // Warna tombol back (MERAH)
    Color backBtnColor = (Color){180, 40, 40, 220}; // Merah gelap
    Color backBorderColor = (Color){255, 100, 100, 255}; // Merah terang
    Color backTextColor = WHITE;
    
    Vector2 mousePos = GetMousePosition();
    
    // Efek hover
    if (CheckCollisionPointRec(mousePos, backBtnRect)) {
        backBtnColor = (Color){220, 60, 60, 255}; // Merah lebih terang saat hover
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        
        // Deteksi klik
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            lobbyNextState = MAIN_MENU;
        }
    }
    
    // Gambar tombol back dengan fungsi baru
    DrawButtonWithText("Back to Main Menu", backBtnRect, backBtnColor, backBorderColor, backTextColor, 22);
    
    // Tombol Start Game di pojok kanan bawah - WARNA BIRU (hanya muncul jika sudah memilih mode)
    if (isHistoryLoaded) {
        int startBtnX = GetScreenWidth() - 350;
        int startBtnY = GetScreenHeight() - 140;
        int startBtnWidth = 240;
        int startBtnHeight = 70;
        
        Rectangle startBtnRect = {(float)startBtnX, (float)startBtnY, (float)startBtnWidth, (float)startBtnHeight};
        
        // Warna tombol start (BIRU)
        Color startBtnColor = (selectedMode == 0) ? 
            (Color){40, 80, 180, 220} : // Biru untuk Math
            (Color){60, 120, 220, 220}; // Biru lebih terang untuk Logic
        
        Color startBorderColor = (selectedMode == 0) ? 
            (Color){100, 150, 255, 255} : // Biru terang untuk Math
            (Color){120, 180, 255, 255};  // Biru lebih terang untuk Logic
        
        Color startTextColor = WHITE;
        
        // Efek hover
        if (CheckCollisionPointRec(mousePos, startBtnRect)) {
            startBtnColor = (selectedMode == 0) ? 
                (Color){60, 100, 220, 255} : // Biru lebih terang saat hover (Math)
                (Color){80, 140, 240, 255};  // Biru lebih terang saat hover (Logic)
            
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            
            // Deteksi klik
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                lobbyNextState = MAIN_MENU;
                printf("[Lobby] Starting %s game for %s\n", 
                       (selectedMode == 0) ? "Math" : "Logic", player.name);
            }
        }
        
        // Teks tombol berdasarkan mode
        const char* startText = (selectedMode == 0) ? "Start Math Game" : "Start Logic Game";
        
        // Gambar tombol start dengan fungsi baru
        DrawButtonWithText(startText, startBtnRect, startBtnColor, startBorderColor, startTextColor, 22);
    }
    
    // Footer
    DrawRectangle(0, GetScreenHeight() - 30, GetScreenWidth(), 30, Fade(BLACK, 0.7f));
    const char* footerText = "EduQuest - Game Lobby";
    int footerWidth = MeasureText(footerText, 18);
    DrawText(footerText, (GetScreenWidth() - footerWidth) / 2, 
             GetScreenHeight() - 25, 18, (Color){200, 200, 100, 200});
}

// Unload resources lobby
void UnloadLobby() {
    UnloadTexture(background);
    
    // Free allocated memory untuk score history
    ClearHistory();
    
    printf("[Lobby] Resources unloaded\n");
}