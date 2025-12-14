#include "animation.h"
#include "global.h"
#include <string.h>
#include <stdio.h>

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
    // Initialize with default empty values
    // Use LoadPlayerAnimationTexture() to load actual sprites
    LoadPlayerAnimationTexture(P_IDLE, "assets/Player/Player_Idle.png", 10, 0.15f, true);
    LoadPlayerAnimationTexture(P_BASIC_ATTACK, "assets/Player/Player_BasicAttack.png", 6, 0.0856f, false);
    LoadPlayerAnimationTexture(P_SKILL, "assets/Player/Player_Skill.png", 4, 0.1f, false);
    LoadPlayerAnimationTexture(P_ULTIMATE, "assets/Player/Player_Ultimate.png", 10, 0.1f, false);
    LoadPlayerAnimationTexture(P_HURT, "assets/Player/Player_Hurt.png", 1, 0.5f, false);
    LoadPlayerAnimationTexture(P_DEATH, "assets/Player/Player_Death.png", 10, 0.15f, false);
    LoadPlayerAnimationTexture(P_RUN, "assets/Player/Player_Run.png", 10, 0.15f, true);
    // Start with IDLE animation
    PlayPlayerAnimation(P_IDLE, true);
}

void InitEnemyAnimations(Enemy* enemy, int stage) {
    if (!enemy) return;
    
    char filepathIdle[100];
    char filepathAttack[100];
    char filepathHurt[100];
    char filepathDeath[100];

    // Enemy Texture Annimation file path
    sprintf(filepathIdle, "assets/Enemy%d/Enemy%d_Idle.png", stage, stage);
    sprintf(filepathAttack, "assets/Enemy%d/Enemy%d_Attack.png", stage, stage);
    sprintf(filepathHurt, "assets/Enemy%d/Enemy%d_Hurt.png", stage, stage);
    sprintf(filepathDeath, "assets/Enemy%d/Enemy%d_Death.png", stage, stage); 
    
    // Initialize with default empty values
    // Use LoadEnemyAnimationTexture() to load actual sprites
    switch (stage)
    {
    case 1:
        LoadEnemyAnimationTexture(enemy, E_IDLE, filepathIdle, 6, 0.15f, true);
        LoadEnemyAnimationTexture(enemy, E_ATTACK, filepathAttack, 10, 0.12f, false);
        LoadEnemyAnimationTexture(enemy, E_HURT, filepathHurt, 5, 0.056f, false);
        LoadEnemyAnimationTexture(enemy, E_DEATH, filepathDeath, 10, 0.11f, false);
        break;
    case 2:
        LoadEnemyAnimationTexture(enemy, E_IDLE, filepathIdle, 4, 0.15f, true);
        LoadEnemyAnimationTexture(enemy, E_ATTACK, filepathAttack, 8, 0.12f, false);
        LoadEnemyAnimationTexture(enemy, E_HURT, filepathHurt, 6, 0.056f, false);
        LoadEnemyAnimationTexture(enemy, E_DEATH, filepathDeath, 8, 0.11f, false);
        break;
    case 3:
        LoadEnemyAnimationTexture(enemy, E_IDLE, filepathIdle, 4, 0.1f, true);
        LoadEnemyAnimationTexture(enemy, E_ATTACK, filepathAttack, 7, 0.12f, false);
        LoadEnemyAnimationTexture(enemy, E_HURT, filepathHurt, 5, 0.056f, false);
        LoadEnemyAnimationTexture(enemy, E_DEATH, filepathDeath, 10, 0.11f, false);
        break;
    case 4:
        LoadEnemyAnimationTexture(enemy, E_IDLE, filepathIdle, 6, 0.15f, true);
        LoadEnemyAnimationTexture(enemy, E_ATTACK, filepathAttack, 10, 0.12f, false);
        LoadEnemyAnimationTexture(enemy, E_HURT, filepathHurt, 5, 0.056f, false);
        LoadEnemyAnimationTexture(enemy, E_DEATH, filepathDeath, 10, 0.11f, false);
        break;
    case 5:
        LoadEnemyAnimationTexture(enemy, E_IDLE, filepathIdle, 4, 0.1f, true);
        LoadEnemyAnimationTexture(enemy, E_ATTACK, filepathAttack, 8, 0.12f, false);
        LoadEnemyAnimationTexture(enemy, E_HURT, filepathHurt, 6, 0.056f, false);
        LoadEnemyAnimationTexture(enemy, E_DEATH, filepathDeath, 8, 0.11f, false);
        break;
    case 6:
        LoadEnemyAnimationTexture(enemy, E_IDLE, filepathIdle, 4, 0.1f, true);
        LoadEnemyAnimationTexture(enemy, E_ATTACK, filepathAttack, 12, 0.12f, false);
        LoadEnemyAnimationTexture(enemy, E_HURT, filepathHurt, 4, 0.056f, false);
        LoadEnemyAnimationTexture(enemy, E_DEATH, filepathDeath, 11, 0.11f, false);
        break;
    case 7:
        LoadEnemyAnimationTexture(enemy, E_IDLE, filepathIdle, 4, 0.1f, true);
        LoadEnemyAnimationTexture(enemy, E_ATTACK, filepathAttack, 7, 0.12f, false);
        LoadEnemyAnimationTexture(enemy, E_HURT, filepathHurt, 5, 0.056f, false);
        LoadEnemyAnimationTexture(enemy, E_DEATH, filepathDeath, 10, 0.11f, false);
        break;
    case 8:
        LoadEnemyAnimationTexture(enemy, E_IDLE, filepathIdle, 6, 0.15f, true);
        LoadEnemyAnimationTexture(enemy, E_ATTACK, filepathAttack, 10, 0.12f, false);
        LoadEnemyAnimationTexture(enemy, E_HURT, filepathHurt, 5, 0.056f, false);
        LoadEnemyAnimationTexture(enemy, E_DEATH, filepathDeath, 10, 0.11f, false);
        break;
    case 9:
        LoadEnemyAnimationTexture(enemy, E_IDLE, filepathIdle, 4, 0.1f, true);
        LoadEnemyAnimationTexture(enemy, E_ATTACK, filepathAttack, 8, 0.12f, false);
        LoadEnemyAnimationTexture(enemy, E_HURT, filepathHurt, 6, 0.056f, false);
        LoadEnemyAnimationTexture(enemy, E_DEATH, filepathDeath, 8, 0.15f, false);
        break;
    case 10:
        LoadEnemyAnimationTexture(enemy, E_IDLE, filepathIdle, 4, 0.1f, true);
        LoadEnemyAnimationTexture(enemy, E_ATTACK, filepathAttack, 12, 0.12f, false);
        LoadEnemyAnimationTexture(enemy, E_HURT, filepathHurt, 4, 0.056f, false);
        LoadEnemyAnimationTexture(enemy, E_DEATH, filepathDeath, 10, 0.11f, false);
        break;
    
    default:
        break;
    }



    
    // Start with IDLE
    PlayEnemyAnimation(enemy, E_IDLE);
}

// ==================== TEXTURE LOADING ====================

void LoadPlayerAnimationTexture(PlayerAnimType animType, const char* texturePath, int frameCount, float frameSpeed, bool loop) {
    // Load texture from file
    player.anims[animType].texture = LoadTexture(texturePath);
    player.anims[animType].frameCount = frameCount;
    player.anims[animType].frameSpeed = frameSpeed;
    player.anims[animType].loop = loop;
    
    // Optional: Set texture filter for pixel art
    SetTextureFilter(player.anims[animType].texture, TEXTURE_FILTER_POINT);
}

void LoadEnemyAnimationTexture(Enemy* enemy, EnemyAnimType animType, const char* texturePath, int frameCount, float frameSpeed, bool loop) {
    if (!enemy) return;
    
    // Load texture from file
    enemy->anims[animType].texture = LoadTexture(texturePath);
    enemy->anims[animType].frameCount = frameCount;
    enemy->anims[animType].frameSpeed = frameSpeed;
    enemy->anims[animType].loop = loop;
    
    // Optional: Set texture filter for pixel art
    SetTextureFilter(enemy->anims[animType].texture, TEXTURE_FILTER_POINT);
}

void UnloadPlayerAnimations() {
    // Unload all loaded textures
    for (int i = 0; i < P_ANIM_COUNT; i++) {
        if (player.anims[i].texture.id != 0) {
            UnloadTexture(player.anims[i].texture);
            player.anims[i].texture.id = 0;
        }
    }
}

void UnloadEnemyAnimations(Enemy* enemy) {
    if (!enemy) return;
    
    // Unload all loaded textures
    for (int i = 0; i < E_ANIM_COUNT; i++) {
        if (enemy->anims[i].texture.id != 0) {
            UnloadTexture(enemy->anims[i].texture);
            enemy->anims[i].texture.id = 0;
        }
    }
}

// ==================== DRAWING FUNCTIONS ====================

void DrawPlayerSprite(int x, int y, float scale) {
    AnimationData* anim = &player.anims[player.currentAnim];
    
    // Check if texture loaded and has frames
    if (anim->texture.id == 0 || anim->frameCount == 0) return;
    
    // Calculate frame dimensions
    int frameWidth = anim->texture.width / anim->frameCount;
    int frameHeight = anim->texture.height;
    
    // Source rectangle (which frame to draw from spritesheet)
    Rectangle source = {
        player.currentFrame * frameWidth,  // X offset
        0,                                  // Y offset
        frameWidth,                         // Width of 1 frame
        frameHeight                         // Height
    };
    
    // Destination rectangle (where to draw on screen)
    Rectangle dest = {
        x,                          // X position
        y,                          // Y position
        frameWidth * scale,         // Scaled width
        frameHeight * scale         // Scaled height
    };
    
    // Draw the sprite
    DrawTexturePro(anim->texture, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
}

void DrawEnemySprite(Enemy* enemy, int x, int y, float scale) {
    if (!enemy) return;
    
    AnimationData* anim = &enemy->anims[enemy->currentAnim];
    
    // Check if texture loaded and has frames
    if (anim->texture.id == 0 || anim->frameCount == 0) return;
    
    // Calculate frame dimensions
    int frameWidth = anim->texture.width / anim->frameCount;
    int frameHeight = anim->texture.height;
    
    // Source rectangle (which frame to draw from spritesheet)
    Rectangle source = {
        enemy->currentFrame * frameWidth,  // X offset
        0,                                  // Y offset
        frameWidth,                         // Width of 1 frame
        frameHeight                         // Height
    };
    
    // Destination rectangle (where to draw on screen)
    Rectangle dest = {
        x,                          // X position
        y,                          // Y position
        frameWidth * scale,         // Scaled width
        frameHeight * scale         // Scaled height
    };
    
    // Draw the sprite
    DrawTexturePro(anim->texture, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
}
