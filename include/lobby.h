#pragma once
#include "global.h"
#include "SaveSystem.h"

// Fungsi-fungsi untuk lobby
void InitLobby();
GameState UpdateLobby();
void DrawLobby();
void UnloadLobby();

// State internal lobby
extern GameState lobbyNextState;
extern playerHistory playerHistoryData;