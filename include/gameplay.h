#pragma once
#include "global.h"

// Gameplay functions
void InitGameplay();
GameState UpdateGameplay();
void DrawGameplay();
void UnloadGameplay();

// State internal gameplay
extern GameState gameplayNextState;

// ============ FUNGSI UNTUK MENGUBAH VARIABEL KUSTOMISASI ============
void SetPlayerWalkDuration(float duration);
void SetPlayerExitDuration(float duration);
void SetPlayerStartX(int startX);
void SetPlayerStopX(int stopX);
void SetPlayerY(int y);
void SetPlayerScale(float scale);
void SetDialogSlideDuration(float duration);
void SetButtonSlideDuration(float duration);      // Mengubah durasi slide tombol
void SetFadeOutUIDuration(float duration);        // Mengubah durasi fade out UI
// =====================================================================