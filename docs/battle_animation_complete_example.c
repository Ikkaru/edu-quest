// // ==================== COMPLETE BATTLE + ANIMATION EXAMPLE ====================
// // File: battleMath.c (CONTOH IMPLEMENTASI)

// #include "global.h"
// #include "animation.h"
// #include "MathQuest.h"
// #include "raygui.h"

// Enemy currentEnemy;

// // ==================== INITIALIZATION ====================
// void InitBattle(int stage) {
//     // 1. Setup player animations
//     InitPlayerAnimations();
    
//     // 2. Load player sprite sheets
//     LoadPlayerAnimationTexture(P_IDLE, "assets/player/idle.png", 4, 0.15f, true);
//     LoadPlayerAnimationTexture(P_ATTACK_1, "assets/player/attack1.png", 5, 0.08f, false);
//     LoadPlayerAnimationTexture(P_ATTACK_2, "assets/player/attack2.png", 6, 0.1f, false);
//     LoadPlayerAnimationTexture(P_ATTACK_3, "assets/player/attack3.png", 8, 0.12f, false);
//     LoadPlayerAnimationTexture(P_HURT, "assets/player/hurt.png", 3, 0.1f, false);
    
//     // 3. Setup enemy
//     currentEnemy = enemies[stage];
//     InitEnemyAnimations(&currentEnemy);
    
//     // 4. Load enemy sprite sheets
//     LoadEnemyAnimationTexture(&currentEnemy, E_IDLE, "assets/enemy/slime_idle.png", 4, 0.2f, true);
//     LoadEnemyAnimationTexture(&currentEnemy, E_ATTACK, "assets/enemy/slime_attack.png", 5, 0.1f, false);
//     LoadEnemyAnimationTexture(&currentEnemy, E_HURT, "assets/enemy/slime_hurt.png", 2, 0.1f, false);
    
//     // 5. Start with idle animations
//     PlayPlayerAnimation(P_IDLE, true);
//     PlayEnemyAnimation(&currentEnemy, E_IDLE);
// }

// // ==================== UPDATE ====================
// void UpdateBattle() {
//     float dt = GetFrameTime();
    
//     // WAJIB: Update animasi setiap frame
//     UpdatePlayerAnimation(dt);
//     UpdateEnemyAnimation(&currentEnemy, dt);
    
//     // ... battle logic lainnya ...
// }

// // ==================== DRAW ====================
// void DrawBattle() {
//     BeginDrawing();
//     ClearBackground(RAYWHITE);
    
//     // Draw player sprite
//     DrawPlayerSprite(200, 400);
    
//     // Draw enemy sprite
//     DrawEnemySprite(&currentEnemy, 800, 300);
    
//     EndDrawing();
// }

// // ==================== HELPER: DRAW SPRITE ====================
// void DrawPlayerSprite(int x, int y) {
//     AnimationData* anim = &player.anims[player.currentAnim];
    
//     // Check if texture loaded
//     if (anim->texture.id == 0 || anim->frameCount == 0) return;
    
//     // Calculate frame dimensions
//     int frameWidth = anim->texture.width / anim->frameCount;
//     int frameHeight = anim->texture.height;
    
//     // Source rectangle (which frame to draw)
//     Rectangle source = {
//         player.currentFrame * frameWidth,  // X offset
//         0,                                  // Y
//         frameWidth,                         // Width
//         frameHeight                         // Height
//     };
    
//     // Destination rectangle (where to draw, with scaling)
//     Rectangle dest = {
//         x,                      // X position
//         y,                      // Y position
//         frameWidth * 3.0f,      // Scale 3x
//         frameHeight * 3.0f      // Scale 3x
//     };
    
//     // Draw the sprite
//     DrawTexturePro(anim->texture, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
// }

// void DrawEnemySprite(Enemy* enemy, int x, int y) {
//     if (!enemy) return;
    
//     AnimationData* anim = &enemy->anims[enemy->currentAnim];
    
//     // Check if texture loaded
//     if (anim->texture.id == 0 || anim->frameCount == 0) return;
    
//     // Calculate frame dimensions
//     int frameWidth = anim->texture.width / anim->frameCount;
//     int frameHeight = anim->texture.height;
    
//     // Source rectangle
//     Rectangle source = {
//         enemy->currentFrame * frameWidth,
//         0,
//         frameWidth,
//         frameHeight
//     };
    
//     // Destination rectangle
//     Rectangle dest = {
//         x,
//         y,
//         frameWidth * 3.0f,
//         frameHeight * 3.0f
//     };
    
//     DrawTexturePro(anim->texture, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
// }

// // ==================== CLEANUP ====================
// void UnloadBattle() {
//     UnloadPlayerAnimations();
//     UnloadEnemyAnimations(&currentEnemy);
// }

// // ==================== BATTLE FLOW EXAMPLE ====================

// typedef enum {
//     PHASE_CHOICE,
//     PHASE_PLAYER_ATTACK,
//     PHASE_WAIT_PLAYER_ANIM,
//     PHASE_ENEMY_HURT,
//     PHASE_ENEMY_ATTACK,
//     PHASE_PLAYER_HURT
// } BattlePhase;

// BattlePhase currentPhase = PHASE_CHOICE;

// void BattleFlow() {
//     float dt = GetFrameTime();
    
//     // Update animations
//     UpdatePlayerAnimation(dt);
//     UpdateEnemyAnimation(&currentEnemy, dt);
    
//     switch (currentPhase) {
//         case PHASE_CHOICE:
//             // Player chooses action
//             if (GuiButton((Rectangle){100, 600, 150, 50}, "Attack")) {
//                 // Start attack animation
//                 PlayPlayerAnimation(P_ATTACK_1, false);
//                 currentPhase = PHASE_PLAYER_ATTACK;
//             }
//             break;
            
//         case PHASE_PLAYER_ATTACK:
//             // Wait for attack animation to finish
//             if (IsPlayerAnimationFinished()) {
//                 // Apply damage
//                 currentEnemy.HP -= 20;
                
//                 // Trigger enemy hurt
//                 PlayEnemyAnimation(&currentEnemy, E_HURT);
//                 ResetPlayerToIdle();
                
//                 currentPhase = PHASE_ENEMY_HURT;
//             }
//             break;
            
//         case PHASE_ENEMY_HURT:
//             // Wait for enemy hurt animation to finish
//             if (IsEnemyAnimationFinished(&currentEnemy)) {
//                 ResetEnemyToIdle(&currentEnemy);
                
//                 // Check if enemy dead
//                 if (currentEnemy.HP <= 0) {
//                     // Victory!
//                     return;
//                 }
                
//                 // Enemy's turn
//                 PlayEnemyAnimation(&currentEnemy, E_ATTACK);
//                 currentPhase = PHASE_ENEMY_ATTACK;
//             }
//             break;
            
//         case PHASE_ENEMY_ATTACK:
//             // Wait for enemy attack animation
//             if (IsEnemyAnimationFinished(&currentEnemy)) {
//                 // Player takes damage
//                 player.HP -= currentEnemy.damage;
                
//                 // Trigger player hurt
//                 PlayPlayerAnimation(P_HURT, false);
//                 ResetEnemyToIdle(&currentEnemy);
                
//                 currentPhase = PHASE_PLAYER_HURT;
//             }
//             break;
            
//         case PHASE_PLAYER_HURT:
//             // Wait for player hurt animation
//             if (IsPlayerAnimationFinished()) {
//                 ResetPlayerToIdle();
                
//                 // Back to player turn
//                 currentPhase = PHASE_CHOICE;
//             }
//             break;
//     }
// }

// // ==================== MAIN GAME LOOP ====================
// int main() {
//     InitWindow(1280, 720, "Battle Test");
//     SetTargetFPS(60);
    
//     InitBattle(0);  // Start stage 0
    
//     while (!WindowShouldClose()) {
//         BattleFlow();   // Update battle
//         DrawBattle();   // Draw everything
//     }
    
//     UnloadBattle();
//     CloseWindow();
//     return 0;
// }
