#include "animation.h"
#include "global.h"
#include <string.h>

// ==================== PLAYER ANIMATION FUNCTIONS ====================

void PlayPlayerAnimation(PlayerAnimType animType, bool shouldLoop) {
    player.currentAnim = animType;
    player.currentFrame = 0;
    player.frameTimer = 0.0f;
    player.isPlaying = true;
    
    // Set loop property (safe even if texture not loaded yet)
    player.anims[animType].loop = shouldLoop;
}

void UpdatePlayerAnimation(float deltaTime) {
    if (!player.isPlaying) return;
    
    AnimationData* currentAnimData = &player.anims[player.currentAnim];
    
    // No animation data loaded yet
    if (currentAnimData->frameCount == 0) return;
    
    player.frameTimer += deltaTime;
    
    // Time to advance to next frame
    if (player.frameTimer >= currentAnimData->frameSpeed) {
        player.currentFrame++;
        player.frameTimer = 0.0f;
        
        // Check if animation finished
        if (player.currentFrame >= currentAnimData->frameCount) {
            if (currentAnimData->loop) {
                // Loop animation (for IDLE)
                player.currentFrame = 0;
            } else {
                // Stop at last frame (for ATTACK, HURT)
                player.currentFrame = currentAnimData->frameCount - 1;
                player.isPlaying = false;
            }
        }
    }
}

bool IsPlayerAnimationFinished() {
    AnimationData* currentAnimData = &player.anims[player.currentAnim];
    
    // If looping, never "finishes"
    if (currentAnimData->loop) return false;
    
    // Check if we reached the last frame and stopped playing
    return !player.isPlaying && 
           player.currentFrame >= currentAnimData->frameCount - 1;
}

void ResetPlayerToIdle() {
    PlayPlayerAnimation(P_IDLE, true);
}

// ==================== ENEMY ANIMATION FUNCTIONS ====================

void PlayEnemyAnimation(Enemy* enemy, EnemyAnimType animType) {
    if (!enemy) return;
    
    enemy->currentAnim = animType;
    enemy->currentFrame = 0;
    enemy->frameTimer = 0.0f;
}

void UpdateEnemyAnimation(Enemy* enemy, float deltaTime) {
    if (!enemy) return;
    
    AnimationData* currentAnimData = &enemy->anims[enemy->currentAnim];
    
    // No animation data loaded yet
    if (currentAnimData->frameCount == 0) return;
    
    enemy->frameTimer += deltaTime;
    
    // Time to advance to next frame
    if (enemy->frameTimer >= currentAnimData->frameSpeed) {
        enemy->currentFrame++;
        enemy->frameTimer = 0.0f;
        
        // Check if animation finished
        if (enemy->currentFrame >= currentAnimData->frameCount) {
            if (currentAnimData->loop) {
                // Loop animation (for IDLE)
                enemy->currentFrame = 0;
            } else {
                // Stop at last frame or reset to idle
                enemy->currentFrame = currentAnimData->frameCount - 1;
            }
        }
    }
}

bool IsEnemyAnimationFinished(Enemy* enemy) {
    if (!enemy) return true;
    
    AnimationData* currentAnimData = &enemy->anims[enemy->currentAnim];
    
    // If looping, never "finishes"
    if (currentAnimData->loop) return false;
    
    // Check if we reached the last frame
    return enemy->currentFrame >= currentAnimData->frameCount - 1;
}

void ResetEnemyToIdle(Enemy* enemy) {
    if (!enemy) return;
    PlayEnemyAnimation(enemy, E_IDLE);
}

// ==================== ANIMATION INITIALIZATION ====================

void InitPlayerAnimations() {
    // Setup default values (you'll replace with actual texture loading)
    
    // WALKING animation (example: 6 frames, 0.1s per frame, loop)
    player.anims[WALKING].frameCount = 6;
    player.anims[WALKING].frameSpeed = 0.1f;
    player.anims[WALKING].loop = true;
    
    // IDLE animation (example: 4 frames, 0.15s per frame, loop)
    player.anims[P_IDLE].frameCount = 4;
    player.anims[P_IDLE].frameSpeed = 0.15f;
    player.anims[P_IDLE].loop = true;
    
    // ATTACK_1 animation (example: 5 frames, 0.08s per frame, no loop)
    player.anims[P_ATTACK_1].frameCount = 5;
    player.anims[P_ATTACK_1].frameSpeed = 0.08f;
    player.anims[P_ATTACK_1].loop = false;
    
    // ATTACK_2 animation (Skill)
    player.anims[P_ATTACK_2].frameCount = 6;
    player.anims[P_ATTACK_2].frameSpeed = 0.1f;
    player.anims[P_ATTACK_2].loop = false;
    
    // ATTACK_3 animation (Ultimate)
    player.anims[P_ATTACK_3].frameCount = 8;
    player.anims[P_ATTACK_3].frameSpeed = 0.12f;
    player.anims[P_ATTACK_3].loop = false;
    
    // HURT animation
    player.anims[P_HURT].frameCount = 3;
    player.anims[P_HURT].frameSpeed = 0.1f;
    player.anims[P_HURT].loop = false;
    
    // Start with IDLE animation
    PlayPlayerAnimation(P_IDLE, true);
}

void InitEnemyAnimations(Enemy* enemy) {
    if (!enemy) return;
    
    // Setup default values for enemy animations
    
    // IDLE animation
    enemy->anims[E_IDLE].frameCount = 4;
    enemy->anims[E_IDLE].frameSpeed = 0.2f;
    enemy->anims[E_IDLE].loop = true;
    
    // ATTACK animation
    enemy->anims[E_ATTACK].frameCount = 5;
    enemy->anims[E_ATTACK].frameSpeed = 0.1f;
    enemy->anims[E_ATTACK].loop = false;
    
    // HURT animation
    enemy->anims[E_HURT].frameCount = 2;
    enemy->anims[E_HURT].frameSpeed = 0.1f;
    enemy->anims[E_HURT].loop = false;
    
    // Start with IDLE
    PlayEnemyAnimation(enemy, E_IDLE);
}
