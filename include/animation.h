#pragma once
#include "global.h"

// ==================== ANIMATION HELPER FUNCTIONS ====================

// --- PLAYER ANIMATION ---
// Play specific player animation (auto-resets frame and timer)
void PlayPlayerAnimation(PlayerAnimType animType, bool shouldLoop);

// Update player animation frame (call every frame)
void UpdatePlayerAnimation(float deltaTime);

// Check if current player animation is finished (for non-looping anims)
bool IsPlayerAnimationFinished();

// Reset player to idle
void ResetPlayerToIdle();

// --- ENEMY ANIMATION ---
// Play specific enemy animation (auto-resets frame and timer)
void PlayEnemyAnimation(Enemy* enemy, EnemyAnimType animType);

// Update enemy animation frame (call every frame)
void UpdateEnemyAnimation(Enemy* enemy, float deltaTime);

// Check if current enemy animation is finished
bool IsEnemyAnimationFinished(Enemy* enemy);

// Reset enemy to idle
void ResetEnemyToIdle(Enemy* enemy);

// --- ANIMATION SETUP ---
// Initialize animation data for player (call once at game start)
void InitPlayerAnimations();

// Initialize animation data for enemy (call when spawning enemy)
void InitEnemyAnimations(Enemy* enemy, int stage);

// --- TEXTURE LOADING ---
// Load player animation texture (spritesheet)
void LoadPlayerAnimationTexture(PlayerAnimType animType, const char* texturePath, int frameCount, float frameSpeed, bool loop);

// Load enemy animation texture (spritesheet)
void LoadEnemyAnimationTexture(Enemy* enemy, EnemyAnimType animType, const char* texturePath, int frameCount, float frameSpeed, bool loop);

// Unload all player animation textures
void UnloadPlayerAnimations();

// Unload all enemy animation textures
void UnloadEnemyAnimations(Enemy* enemy);

// --- DRAWING ---
// Draw player sprite with current animation
void DrawPlayerSprite(int x, int y, float scale);

// Draw enemy sprite with current animation
void DrawEnemySprite(Enemy* enemy, int x, int y, float scale);
