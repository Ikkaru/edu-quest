#include "raylib.h"
#include "mainMenu.h"

// Konstanta Variabel
static Texture2D title;
static Texture2D background;
static float titleScale = 0.75f;

void InitMainMenu() {
    // Load Menu Background
    background = LoadTexture("assets/background/background_MainMenu.png");
    SetTextureFilter(background, TEXTURE_FILTER_POINT);

    // Load Title Logo
    title = LoadTexture("assets/title.png");
    SetTextureFilter(title, TEXTURE_FILTER_POINT);
}

void DrawMainMenu() {
    // load Background
    ClearBackground(BLACK);
    DrawTexture(background, 0, 0, WHITE);

    // Load Title
    float finalWidth = title.width * titleScale;

    // Title Position
    float posX = (GetScreenWidth() - finalWidth) / 2;
    float posY = 50.0f;

    DrawTextureEx(title, (Vector2){posX, posY}, 0.0f, titleScale, WHITE);
}

void UnloadMainMenu() {
    UnloadTexture(background);
    UnloadTexture(title);
}

