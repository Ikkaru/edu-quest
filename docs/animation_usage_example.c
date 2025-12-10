// EXAMPLE: How to use Animation Helper Functions in battleMath.c

#include "global.h"
#include "animation.h"
#include "MathQuest.h"

// ==================== EXAMPLE USAGE ====================

void ExamplePlayerAttack() {
    // 1. TRIGGER ATTACK ANIMATION (langsung main!)
    PlayPlayerAnimation(P_ATTACK_1, false); // false = tidak loop
    
    // Animasi akan berjalan otomatis saat UpdatePlayerAnimation() dipanggil
}

void ExampleSkillAttack() {
    // Skill attack menggunakan animasi berbeda
    PlayPlayerAnimation(P_ATTACK_2, false);
}

void ExampleUltimateAttack() {
    // Ultimate attack
    PlayPlayerAnimation(P_ATTACK_3, false);
}

void ExamplePlayerHurt() {
    // Player kena damage
    PlayPlayerAnimation(P_HURT, false);
}

void ExampleEnemyAttack(Enemy* enemy) {
    // Trigger enemy attack animation
    PlayEnemyAnimation(enemy, E_ATTACK);
}

void ExampleEnemyHurt(Enemy* enemy) {
    // Enemy kena damage
    PlayEnemyAnimation(enemy, E_HURT);
}

// ==================== BATTLE UPDATE LOOP ====================

void BattleUpdateExample(Enemy* currentEnemy) {
    float dt = GetFrameTime();
    
    // UPDATE ANIMATIONS EVERY FRAME
    UpdatePlayerAnimation(dt);
    UpdateEnemyAnimation(currentEnemy, dt);
    
    // Check if attack animation finished
    if (IsPlayerAnimationFinished()) {
        // Attack animation selesai, kembali ke idle
        ResetPlayerToIdle();
    }
    
    if (IsEnemyAnimationFinished(currentEnemy)) {
        // Enemy animation selesai
        ResetEnemyToIdle(currentEnemy);
    }
}

// ==================== COMPLETE BATTLE EXAMPLE ====================

typedef enum {
    STATE_CHOICE,
    STATE_PLAYER_ATTACK,
    STATE_WAIT_ANIM,
    STATE_ENEMY_TURN
} SimpleBattleState;

void SimpleBattleFlow(Enemy* enemy) {
    static SimpleBattleState state = STATE_CHOICE;
    float dt = GetFrameTime();
    
    // ALWAYS update animations
    UpdatePlayerAnimation(dt);
    UpdateEnemyAnimation(enemy, dt);
    
    switch (state) {
        case STATE_CHOICE:
            // Player pilih action
            if (IsKeyPressed(KEY_SPACE)) {
                // Langsung trigger attack animation!
                PlayPlayerAnimation(P_ATTACK_1, false);
                state = STATE_PLAYER_ATTACK;
            }
            break;
            
        case STATE_PLAYER_ATTACK:
            // Tunggu attack animation selesai
            if (IsPlayerAnimationFinished()) {
                // Deal damage ke enemy
                enemy->HP -= 20;
                
                // Trigger enemy hurt animation
                PlayEnemyAnimation(enemy, E_HURT);
                
                // Reset player ke idle
                ResetPlayerToIdle();
                
                state = STATE_WAIT_ANIM;
            }
            break;
            
        case STATE_WAIT_ANIM:
            // Tunggu enemy hurt animation selesai
            if (IsEnemyAnimationFinished(enemy)) {
                ResetEnemyToIdle(enemy);
                state = STATE_ENEMY_TURN;
            }
            break;
            
        case STATE_ENEMY_TURN:
            // Enemy turn
            PlayEnemyAnimation(enemy, E_ATTACK);
            
            // Wait then player takes damage
            if (IsEnemyAnimationFinished(enemy)) {
                PlayPlayerAnimation(P_HURT, false);
                player.HP -= enemy->damage;
                
                ResetEnemyToIdle(enemy);
                state = STATE_CHOICE; // Back to player choice
            }
            break;
    }
}

// ==================== INITIALIZATION ====================

void InitializeBattle(int stage) {
    // Setup player animations (panggil sekali di awal game)
    InitPlayerAnimations();
    
    // Setup enemy animations
    Enemy* enemy = &enemies[stage];
    InitEnemyAnimations(enemy);
    
    // Player mulai dengan idle
    PlayPlayerAnimation(P_IDLE, true);
    
    // Enemy juga idle
    PlayEnemyAnimation(enemy, E_IDLE);
}
