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
void SetPlayerWalkDuration(float duration);    // Mengubah durasi perjalanan masuk
void SetPlayerExitDuration(float duration);    // Mengubah durasi perjalanan keluar
void SetPlayerStartX(int startX);             // Mengubah posisi X awal
void SetPlayerStopX(int stopX);               // Mengubah posisi X berhenti
void SetPlayerY(int y);                       // Mengubah posisi Y
void SetPlayerScale(float scale);             // Mengubah ukuran player
// =====================================================================