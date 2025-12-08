#include "raylib.h"
#include "mainMenu.h"
#include "global.h"

int main() {
    // Initilize Window
    InitWindow(1280, 720, "EduQuest (Debug)");
    SetTargetFPS(60);

    // Set Window Icon
    Image icon = LoadImage("assets/icon/icon.png");
    ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    SetWindowIcon(icon);
    UnloadImage(icon);

    InitMainMenu();

    // Application Logic
    while (!WindowShouldClose())
    {   

        // Draw Main Menu
        BeginDrawing();
            DrawMainMenu();
        DrawMainMenu();
        
    }

    UnloadMainMenu();
    CloseWindow();
    return 0;
}