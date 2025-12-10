#pragma once
#include "raylib.h"
// Game State
typedef enum {
    MAIN_MENU,
    LOBBY,
    GAMEPLAY,
    BATTLE,
    SPECIAL_EVENT,
    GAME_OVER,
    EXIT
} GameState;

// Game Mode
typedef enum {
    MATH,
    LOGIC,
} GameMode;

// Gamedata
typedef struct
{   
    GameMode currentMode;
    int score; 
    int energy;
    int maxEnergy;
    int HP;
    int maxHP;
    int stage;
    int maxStage;
    char name[101];

    // --- ANIMATION SYSTEM ---
    AnimationData anims[P_ANIM_COUNT]; // Array menyimpan 5 animasi
    int currentAnim;       // Sedang putar animasi yang mana?
    int currentFrame;      // Frame ke berapa?
    float frameTimer;
    bool isPlaying;        // Apakah animasi sedang berjalan?
} PlayerData;

typedef struct
{
    char name[100];
    int maxHP;
    int HP;
    int damage;
    char texture[100];

    AnimationData anims[E_ANIM_COUNT]; // Array menyimpan 3 animasi
    int currentAnim;
    int currentFrame;
    float frameTimer;
} Enemy;

typedef enum
{
    BATTLE_PLAYER_CHOICE,
    BATTLE_PLAYER_QUIZ,
    BATTLE_ANIMATION,
    BATTLE_ENEMY_TURN,
    BATTLE_DEFENSE_QUIZ,
} BattleState;

typedef enum
{
    BASIC_ATTACK,
    SKILL,
    ULTIMATE,
    SKIP,
} PlayerChoice;

typedef enum {
    WALKING,
    P_IDLE,
    P_ATTACK_1,
    P_ATTACK_2,
    P_ATTACK_3,
    P_HURT,
    P_ANIM_COUNT // Total animasi player (5)
} PlayerAnimType;

typedef enum {
    E_IDLE,
    E_ATTACK,
    E_HURT,
    E_ANIM_COUNT // Total animasi enemy (3)
} EnemyAnimType;

typedef struct {
    Texture2D texture;
    int frameCount;
    float frameSpeed;
    bool loop; // True untuk Idle, False untuk Attack/Hurt
} AnimationData;


extern PlayerData player;
extern GameState currentState;
extern Enemy enemies[10];
extern BattleState currentBattleState;
extern PlayerChoice playerChoice; 
