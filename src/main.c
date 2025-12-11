#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "mainMenu.h"
#include "battle.h"
#include "animation.h"
#include "global.h"
#include "raygui.h"


int main() {

    // Initilize State
    GameState currentState;
    GameState nextState;

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
            nextState = UpdateMainMenu();
            break;

        case BATTLE:
            Battle(player.stage);
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

        case BATTLE:
            BeginDrawing();
                DrawBattleGUI();
            EndDrawing();
            break;
        }

        // Cek apakah ada permintaan perubahan state
        if (nextState != currentState) {

            // Unload memori State Sebelumnya
            switch (currentState)
            {
            case MAIN_MENU: UnloadMainMenu(); break;
            
            }

            // Load memori state berikutnya
            switch (nextState)
            {
            case MAIN_MENU: InitMainMenu(); break;
            case BATTLE: currentState = BATTLE; InitPlayerAnimations(); InitEnemyAnimations(&enemies[player.stage-1], player.stage); break;
            case EXIT:
                CloseWindow();
                return 0;
                break;
            }
        }
        
    }

    CloseWindow();
    return 0;
}