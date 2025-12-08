#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "mainMenu.h"
#include "global.h"
#include "raygui.h"


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
        // Logic
        switch (currentState)
        {
        case MAIN_MENU:
            UpdateMainMenu();
            break;
        
        default:
            break;
        }


        // Draw
        switch (currentState)
        {
        case MAIN_MENU:
            BeginDrawing();
                DrawMainMenu();
            EndDrawing();
            break;
        
        default:
            break;
        }
        
    }

    UnloadMainMenu();

    CloseWindow();
    return 0;
}