#include "raylib.h"
#include "global.h"

int main() {
    // Initilize Window
    InitWindow(1280, 720, "Edu Quest (Debug)");
    SetTargetFPS(60);

    // Set Window Icon
    Image icon = LoadImage("assets/icon/icon.png");
    ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    SetWindowIcon(icon);
    UnloadImage(icon);

    // Application Logic
    while(!WindowShouldClose())
    {   
        MainMenu();
    }

    CloseWindow();
    return 0;
}