#include "raylib.h"
#include "global.h"
#include "mainMenu.h"
#include "raygui.h"
#include <string.h>

// Konstanta Variabel
static Texture2D title;
static Texture2D background;
static float titleScale = 0.85f;
char tempName[101]; // set max panjang nama hanya 100

// State Internal Menu
static bool showLoginBox = false;
static bool editMode = true;
static bool loginSuccess = false;
GameState nextState = MAIN_MENU;

// Inisialisasi Main Menu
void InitMainMenu() {
    // Load Menu Background
    background = LoadTexture("assets/background/background_MainMenu.png");
    SetTextureFilter(background, TEXTURE_FILTER_POINT);

    // Load Title Logo
    title = LoadTexture("assets/title.png");
    SetTextureFilter(title, TEXTURE_FILTER_POINT);

    nextState = MAIN_MENU;
}

// Update Kondisi State
GameState UpdateMainMenu() {
    // Cek apakah login box aktif
    if (!showLoginBox) {
        // Menunggu player menekan Enter
        if (IsKeyPressed(KEY_ENTER)) {
            showLoginBox = true;
        }
    } 
    else {
        // Cek Jika Player Membatalkan Login
        if (IsKeyPressed(KEY_ESCAPE)) {
            showLoginBox = false;
        }

        // Cek apakah player sudah login
        if (IsKeyPressed(KEY_ENTER)) {
            if (strlen(tempName) > 0 && strlen(tempName) < 101) {
                strcpy(player.name, tempName);
                loginSuccess = true;
                showLoginBox = false;
            }
            else {
                // kosongkan lagi tempName
                memset(tempName, 0, sizeof(tempName));
            }
        }
    }

    return nextState;
}

void DrawMainMenu() {
    // load Background
    ClearBackground(BLACK);
    DrawTexture(background, 0, 0, WHITE);

    // Load Title
    float finalWidth = title.width * titleScale;

    // Title Position
    float tX = (GetScreenWidth() - finalWidth) / 2;
    float tY = 50.0f;

    DrawTextureEx(title, (Vector2){tX, tY}, 0.0f, titleScale, WHITE);
    DrawText("v0.1 EduQuest Build", 10, GetScreenHeight() - 30, 20, GRAY);

    // Tampilkan GUI Login
    if (showLoginBox) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));

        // Login Box Posititon
        int lX = GetScreenWidth() / 2;
        int lY = GetScreenHeight() / 2;

        GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

        // Login Box
        GuiGroupBox((Rectangle){ lX - 150, lY - 80, 300, 180 }, "Login");
        GuiLabel((Rectangle){ lX - 130, lY - 40 , 260, 20 }, "Enter Your Name");

        // Text Box Input
        if (GuiTextBox((Rectangle){ lX - 130, lY - 10, 260, 40 }, tempName, 20, editMode)){
            editMode = !editMode;
        }

        // Tombol Login
        if (GuiButton((Rectangle){ lX - 130, lY + 50, 260, 40 }, "Login")) {
            if (strlen(tempName) > 0 && strlen(tempName) < 101) {
                strcpy(player.name, tempName);
                loginSuccess = true;
                showLoginBox = false;
                return;
            }
            else
            {
                // kosongkan lagi tempName
                memset(tempName, 0, sizeof(tempName));
            }
        }
    }
    // Draw The Text Button
    else if (loginSuccess) {
        int gap = 70; // Jarak antar text
        int startY = 350;
        // Tombol Play Game
        const char textPlay[] = "Play Game";
        if (TextButton(textPlay, (GetScreenWidth() - MeasureText(textPlay, 40)) / 2, startY, 40)) {
            nextState = LOBBY;  // SEMENTARA BUAT TESTING
        }
        // Tombol Exit
        const char textExit[] = "Exit";
        if(TextButton(textExit, (GetScreenWidth() - MeasureText(textExit, 45)) / 2, startY + gap, 45)) {
            nextState = EXIT;
        }
    }
    // Ketika player Belum Menekan Enter dan belum berhasil login
    else {
        if ((int)(GetTime() * 2) % 2 == 0) {
            const char text[] = "Press ENTER to Start";
            int textW = MeasureText(text, 20);
            DrawText(text, (GetScreenWidth() - textW) / 2, 600, 20, WHITE);
        }
    }
        
}


// Callable button Function
bool TextButton(const char* text, int x, int y, int fontSize) {
    // Ukuran Text
    int textWidth = MeasureText(text, fontSize);
    int textHeight = fontSize;

    // Hit Box
    Rectangle hitbox = { x, y, textWidth, textHeight};

    Vector2 mousePos = GetMousePosition();
    bool isHover = CheckCollisionPointRec(mousePos, hitbox);
    bool isClicked = false;

    Color textColor = LIGHTGRAY;
    if (isHover) {
        textColor = WHITE;
    }

    // Logika Button
    if (isHover) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            isClicked = true;
        }
    }
    else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
    // Draw Bayangan Text
    DrawText(text, x + 2, y + 2, fontSize, BLACK);
    // Draw Text
    DrawText(text, x, y, fontSize, textColor);

    return isClicked;
}

void UnloadMainMenu() {
    UnloadTexture(background);
    UnloadTexture(title);
}

