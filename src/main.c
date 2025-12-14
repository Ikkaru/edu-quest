#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "mainMenu.h"
#include "battle.h"
#include "animation.h"
#include "global.h"
#include "raygui.h"
#include "lobby.h"
#include "gameplay.h"  // Tambah include gameplay

int main() {
    // Initialize State
    GameState currentState;
    GameState nextState;

    // Initialize Window
    InitWindow(1280, 720, "EduQuest (Debug)");
    SetTargetFPS(60);

    // Intialize Audio Device
    InitAudioDevice();

    // Set Window Icon
    Image icon = LoadImage("assets/icon/icon.png");
    ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    SetWindowIcon(icon);
    UnloadImage(icon);

    InitMainMenu(); 
    currentState = MAIN_MENU; // FOR TESTING INGETIN GUE BUAT UBAH
    nextState = MAIN_MENU;

    // Application Logic
    while (!WindowShouldClose())
    {   
        // Logic
        switch (currentState)
        {
        case MAIN_MENU:
            nextState = UpdateMainMenu();
            break;
        case LOBBY:
            nextState = UpdateLobby();
            break;
        case GAMEPLAY:  // Tambah case untuk gameplay
            nextState = UpdateGameplay();
            break;
        case BATTLE:
            nextState = UpdateBattle();
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
        case LOBBY:
            BeginDrawing();
                DrawLobby();
            EndDrawing();
            break;
        case GAMEPLAY:  // Tambah case untuk gameplay
            BeginDrawing();
                DrawGameplay();
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
            case LOBBY: UnloadLobby(); break;
            case GAMEPLAY: UnloadGameplay(); break;  // Tambah untuk gameplay
            case BATTLE: UnloadBattle(); break;
            }

            // Load memori state berikutnya
            switch (nextState)
            {
            case MAIN_MENU: InitMainMenu(); break;
            case LOBBY: InitLobby(); break;
            case GAMEPLAY: InitGameplay(); break;  // Tambah untuk gameplay
            case BATTLE: InitBattle(player.stage); break;
            case EXIT:
                CloseWindow();
                return 0;
                break;
            }
            
            currentState = nextState;
        }
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}