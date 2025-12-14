#pragma once
#include "raylib.h"
// Game State
typedef enum {
    MAIN_MENU,
    LOBBY,
    GAMEPLAY,
    BATTLE,
    GAME_OVER,
    EXIT
} GameState;

// Game Mode
typedef enum {
    MATH,
    LOGIC,
} GameMode;

// Annimation Data
typedef struct {
    Texture2D texture;
    int frameCount;
    float frameSpeed;
    bool loop; // True untuk Idle, False untuk Attack/Hurt
} AnimationData;


// Enemy Annimation
typedef enum {
    E_IDLE,
    E_ATTACK,
    E_HURT,
    E_DEATH,
    E_ANIM_COUNT // Total animasi enemy (4)
} EnemyAnimType;

// Player Annimation
typedef enum {
    WALKING,
    P_RUN,
    P_IDLE,
    P_BASIC_ATTACK,
    P_SKILL,
    P_ULTIMATE,
    P_HURT,
    P_DEATH,
    P_ANIM_COUNT // Total animasi player (6)
} PlayerAnimType;

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
    BATTLE_ENEMY_ANIMATION,
    BATTLE_DEFENSE_QUIZ,
} BattleState;

typedef enum
{   
    NOT_SELECTED,
    BASIC_ATTACK,
    SKILL,
    ULTIMATE,
    SKIP,
} PlayerChoice;

extern PlayerData player;
extern GameState currentState;
extern Enemy enemies[10];
extern Enemy currentEnemy;
extern BattleState currentBattleState;
extern PlayerChoice playerChoice; 
