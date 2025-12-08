#pragma once
#include "global.h"
void InitMainMenu();
GameState UpdateMainMenu();
void DrawMainMenu();
bool TextButton(const char* text, int x, int y, int fontSize);
void UnloadMainMenu();
