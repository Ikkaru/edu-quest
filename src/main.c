#include "raylib.h"
#include "mainMenu.h"
#include "global.h"

int main() {

    // Initilize State
    GameState currentState;

    // Initilize Window
    InitWindow(1280, 720, "EduQuest (Debug)");
    SetTargetFPS(60);

    // Set Window Icon
    Image icon = LoadImage("assets/icon/icon.png");
    ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    SetWindowIcon(icon);
    UnloadImage(icon);

    InitMainMenu(); 

    currentState = MAIN_MENU;

    // Application Logic
    while (!WindowShouldClose())
    {   
        // Fase Draw
        switch (currentState)
        {
        case MAIN_MENU:
            BeginDrawing();
                DrawMainMenu();
            EndDrawing();
            UnloadMainMenu();
            currentState = BATTLE;
            break;
        
        default:
            break;
        }
        
    }

    CloseWindow();
    return 0;
}