# EduQuest - Game Documentation

## 📖 Table of Contents
1. [Project Overview](#project-overview)
2. [Architecture](#architecture)
3. [Game Flow](#game-flow)
4. [Module Documentation](#module-documentation)
5. [Data Structures](#data-structures)
6. [API Reference](#api-reference)
7. [Build & Deployment](#build--deployment)
8. [Asset Structure](#asset-structure)

---

## 📋 Project Overview

**EduQuest** adalah game edukasi berbasis quiz yang menggabungkan pembelajaran matematika dan logika dengan sistem battle turn-based. Game ini dibangun menggunakan Raylib framework dalam bahasa C.

### Key Features
- **Dual Mode**: Matematika dan Logika
- **Turn-Based Battle System**: Player vs Enemy dengan sistem energy dan skill
- **Progressive Difficulty**: 10 stage dengan tingkat kesulitan meningkat
- **Animation System**: Sprite-based animation untuk player dan enemy
- **Save System**: Menyimpan score dan history player
- **Responsive UI**: GUI interaktif menggunakan raygui

### Tech Stack
- **Language**: C (C99)
- **Framework**: Raylib 5.0
- **GUI**: raygui
- **Build System**: GCC
- **Platform**: Windows

---

## 🏗 Architecture

### State Machine Design

Game menggunakan **finite state machine** untuk mengatur alur aplikasi:

```
┌─────────────┐
│  MAIN_MENU  │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│    LOBBY    │◄─────────┐
└──────┬──────┘          │
       │                 │
       ▼                 │
┌─────────────┐          │
│  GAMEPLAY   │          │
└──────┬──────┘          │
       │                 │
       ▼                 │
┌─────────────┐          │
│   BATTLE    │──────────┤
└──────┬──────┘          │
       │                 │
       ▼                 │
┌─────────────┐          │
│ GAME_OVER   │──────────┘
└─────────────┘
```

### Directory Structure

```
edu-quest/
├── src/                    # Source code files
│   ├── main.c              # Entry point & state machine
│   ├── global.c            # Global data initialization
│   ├── mainMenu.c          # Main menu logic
│   ├── lobby.c             # Mode selection & history
│   ├── gameplay.c          # Stage transition scene
│   ├── battle.c            # Battle system core
│   ├── animation.c         # Animation engine
│   ├── MathQuest.c         # Math question generator
│   ├── logika.c            # Logic question loader
│   └── SaveSystem.c        # Score persistence
├── include/                # Header files
│   ├── global.h            # Global types & declarations
│   ├── battle.h
│   ├── animation.h
│   ├── MathQuest.h
│   ├── logika.h
│   └── SaveSystem.h
├── assets/                 # Game assets
│   ├── audio/
│   ├── background/
│   ├── Player/
│   ├── Enemy1-10/
│   └── icon/
├── history/                # Save data
│   ├── math/
│   └── logic/
├── bin/                    # Compiled executables
├── lib/                    # Raylib libraries
├── quest.txt               # Logic questions database
└── answ.txt                # Logic answers database
```

---

## 🎮 Game Flow

### 1. Startup Sequence

```c
main() {
    InitWindow(1280, 720, "EduQuest");
    SetTargetFPS(60);
    InitAudioDevice();
    
    // Set window icon
    // Initialize Main Menu
    // Start game loop
}
```

**Flow:**
1. Window initialization (1280x720 @ 60 FPS)
2. Audio device setup
3. Icon loading
4. Initial state: `MAIN_MENU`
5. Enter game loop

### 2. Main Menu Flow

```
MAIN_MENU State:
├── Display title animation
├── Input player name (max 100 chars)
├── Show "START GAME" button
└── On click → Transition to LOBBY
```

**Key Functions:**
- `InitMainMenu()`: Load background, reset state
- `UpdateMainMenu()`: Handle input, button detection
- `DrawMainMenu()`: Render UI with animations
- `UnloadMainMenu()`: Cleanup resources

### 3. Lobby Flow

```
LOBBY State:
├── Select Game Mode
│   ├── MATH Mode (Red button)
│   └── LOGIC Mode (Blue button)
├── Display Score History
│   ├── Load from history/<mode>/<name>.txt
│   └── Show top 5 scores (sorted descending)
├── Show "START QUEST" button
└── On click → Initialize player → GAMEPLAY
```

**Player Initialization:**
```c
// Math Mode
player.currentMode = MATH;
player.HP = 100;
player.maxHP = 100;
player.energy = 3;
player.maxEnergy = 10;
player.score = 0;
player.stage = 1;
player.maxStage = 10;

// Logic Mode (same stats)
```

### 4. Gameplay Flow

```
GAMEPLAY State:
├── Player walks to center (P_RUN animation)
├── Stop at center → P_IDLE animation
├── Show dialog: "Ready for battle?"
│   ├── YES → Player exits → BATTLE
│   └── NO → Save score → LOBBY
└── Fade transitions for smooth visuals
```

**Animation Timeline:**
```
0.0s - 1.5s:  Player walks in (P_RUN)
1.5s:         Reach center, switch to P_IDLE
1.5s - 2.0s:  Dialog slides in
2.0s:         Buttons appear
User choice:  Fade out + exit animation (2.5s)
```

### 5. Battle Flow

The battle system is the core gameplay loop:

```
BATTLE State Machine:
┌──────────────────────┐
│ BATTLE_PLAYER_CHOICE │
└──────┬───────────────┘
       │ Select action
       ▼
┌──────────────────────┐
│ BATTLE_PLAYER_QUIZ   │ ← 10 second timer
└──────┬───────────────┘
       │ Answer question
       ▼
┌──────────────────────┐
│ BATTLE_ANIMATION     │ ← Player attacks
└──────┬───────────────┘
       │ Animation done
       ▼
┌──────────────────────┐
│ BATTLE_ENEMY_TURN    │ ← 3s dialog + quiz
└──────┬───────────────┘
       │ Answer question
       ▼
┌──────────────────────┐
│ BATTLE_ENEMY_ANIMATION│ ← Enemy attacks
└──────┬───────────────┘
       │ Check win/lose
       ├─► BATTLE_VICTORY → GAMEPLAY
       ├─► BATTLE_GAMEOVER_VICTORY → MAIN_MENU
       └─► BATTLE_GAMEOVER_DEFEAT → MAIN_MENU
```

#### Battle State Details

**BATTLE_PLAYER_CHOICE:**
- Display 4 action buttons:
  - **Basic Attack**: +1 energy, 10 damage
  - **Skill**: -2 energy, 25 damage
  - **Ultimate**: -3 energy, 50 damage
  - **Skip**: +2 energy, no damage
- Keyboard navigation: LEFT/RIGHT to select, ENTER to confirm
- Energy constraints enforced (buttons disabled if insufficient energy)

**BATTLE_PLAYER_QUIZ:**
- 10-second countdown timer
- Question generation based on mode:
  - **MATH**: Dynamic generation from `MathQuest.c`
  - **LOGIC**: Random selection from `quest.txt`
- Text input for answer (GuiTextBox)
- Submit with ENTER key
- Timer expires → Wrong answer (MISS)

**BATTLE_ANIMATION:**
- Play player attack animation (P_BASIC_ATTACK/P_SKILL/P_ULTIMATE)
- Wait for animation finish
- If correct: Play enemy hurt (E_HURT), deal damage
- If wrong: Skip to enemy turn
- Display floating damage text (fades upward)
- Check enemy HP:
  - HP <= 0 → E_DEATH animation → BATTLE_VICTORY
  - HP > 0 → Continue to enemy turn

**BATTLE_ENEMY_TURN:**
- Show "Enemy's Turn!" dialog for 3 seconds
- Generate question for player to defend
- 10-second timer (same as player quiz)
- Answer determines if player takes damage

**BATTLE_ENEMY_ANIMATION:**
- Play enemy attack animation (E_ATTACK)
- Wait for animation finish
- If wrong answer: Play player hurt (P_HURT), deal damage
- If correct: Player dodges (no damage)
- Display floating damage text
- Check player HP:
  - HP <= 0 → P_DEATH animation → BATTLE_GAMEOVER_DEFEAT
  - HP > 0 → Back to BATTLE_PLAYER_CHOICE

**BATTLE_VICTORY:**
- Play E_DEATH animation
- Award 100 score points
- If stage == maxStage → BATTLE_GAMEOVER_VICTORY
- Else → Return to GAMEPLAY (next stage)

**BATTLE_GAMEOVER (Victory/Defeat):**
- Stop battle music → Play gameover music
- Fade in effect (alpha 0.0 → 1.0)
- Display final score and message
- Save score to history
- Press ENTER → MAIN_MENU

---

## 📦 Module Documentation

### main.c - Application Core

**Responsibility:** Entry point, state management, main game loop

**Key Functions:**

```c
int main()
```
- Initialize window, audio, and resources
- Manage state machine transitions
- Handle Init/Update/Draw/Unload lifecycle for each state
- Main game loop (60 FPS)

**State Transition Logic:**
```c
if (nextState != currentState) {
    // Unload old state
    switch (currentState) { ... }
    
    // Load new state
    switch (nextState) { ... }
    
    currentState = nextState;
}
```

---

### global.c & global.h - Global Data

**Responsibility:** Centralized data structures and enemy definitions

**Key Structures:**

```c
typedef struct {
    GameMode currentMode;  // MATH or LOGIC
    int score;             // Current score
    int energy;            // Current energy (0-10)
    int maxEnergy;         // Maximum energy
    int HP;                // Current health
    int maxHP;             // Maximum health
    int stage;             // Current stage (1-10)
    int maxStage;          // Total stages
    char name[101];        // Player name
    
    // Animation data
    AnimationData anims[P_ANIM_COUNT];
    int currentAnim;
    int currentFrame;
    float frameTimer;
    bool isPlaying;
} PlayerData;
```

```c
typedef struct {
    char name[100];        // Enemy name
    int maxHP;             // Maximum health
    int HP;                // Current health
    int damage;            // Damage per attack
    
    // Animation data
    AnimationData anims[E_ANIM_COUNT];
    int currentAnim;
    int currentFrame;
    float frameTimer;
} Enemy;
```

**Global Variables:**
```c
PlayerData player;              // Global player instance
Enemy enemies[10];              // 10 enemies (stages)
Enemy currentEnemy;             // Active battle enemy
GameState currentState;         // Current game state
BattleState currentBattleState; // Current battle sub-state
PlayerChoice playerChoice;      // Selected action
```

**Enemy Definitions:**
```c
// Stage 1-10 enemies with progressive difficulty
enemies[0] = {"Slime", 50, 50, 5};
enemies[1] = {"Goblin", 80, 80, 8};
enemies[2] = {"Skeleton", 120, 120, 12};
// ... up to Stage 10 boss
enemies[9] = {"Dark Lord", 500, 500, 40};
```

---

### battle.c - Battle System

**Responsibility:** Core battle logic, quiz system, damage calculation

**Global Variables:**
```c
Music bgm, over;                    // Background music
Question mathQuestion;              // Current math question
char userAnswer[64];                // User input buffer
bool answerEditMode;                // TextBox edit state
bool correct;                       // Answer correctness
float timer;                        // Quiz timer (10s)
DamageText damageText;              // Floating damage text
float animationTimer;               // Animation delay timer
bool enemyTurnDialogActive;         // Enemy turn dialog flag
float enemyTurnDialogTimer;         // 3-second dialog timer
BattleState currentBattleState;     // Battle sub-state
PlayerChoice playerChoice;          // Selected action
GameState nextGameState;            // Next state transition

// Logic mode variables
Soal daftarSoal[MAKS_SOAL];         // Question bank
KunciJawaban daftarJawaban[MAKS_SOAL]; // Answer key
int jumlahSoalLogika;               // Question count
int jumlahJawabanLo;                // Answer count
Soal CurrentQuestion;               // Current logic question
char correctAnswer;                 // Correct answer (a/b/c)

// Game Over fade
float fadeAlpha;                    // Fade transparency (0-1)
float fadeSpeed;                    // Fade speed
bool fadeComplete;                  // Fade finished flag
```

**Key Functions:**

```c
void InitBattle(int stage)
```
- Initialize battle state and variables
- Load current enemy from global array
- Load BGM and gameover music
- Load logic questions from files
- Setup GUI styles

```c
GameState UpdateBattle()
```
- Main battle state machine
- Handle all 8 battle states
- Update timers and animations
- Process user input
- Update music streams
- Update floating damage text
- Return next game state

```c
void DrawBattleGUI()
```
- Render player and enemy sprites
- Draw health bars and energy display
- Draw turn indicator (top right)
- Draw floating damage text
- Render state-specific UI:
  - Player choice buttons
  - Quiz interface
  - Animation dialogs
  - Game over screens

```c
bool IsAnswerCorrect()
```
- Validate user answer
- **MATH mode**: Convert string to int, compare with `mathQuestion.correctAnswer`
- **LOGIC mode**: Compare first char (case-insensitive) with `correctAnswer`

```c
void ExecutePlayerDamage(bool iscorrect)
```
- Calculate damage based on playerChoice (10/25/50)
- Play attack animation (P_BASIC_ATTACK/P_SKILL/P_ULTIMATE)
- Apply damage to enemy if correct
- Create floating damage text ("−X" or "MISS")
- Transition to BATTLE_ANIMATION state

```c
void ExecuteEnemyDamage(bool iscorrect)
```
- Get enemy damage value
- Apply damage to player if incorrect
- Create floating damage text
- Play enemy attack animation (E_ATTACK)
- Transition to BATTLE_ENEMY_ANIMATION state

```c
void QuestionGeneration()
```
- Generate question based on currentMode
- **MATH**: Call `generateQuestion(player.stage)`
- **LOGIC**: Call `ambilSoalAcak()` with question bank

```c
void DrawQuizInterface(int screenW, int screenH)
```
- Draw quiz dialog box
- Display question text (math or logic with options)
- Render input text box (GuiTextBox)
- Draw countdown timer bar

```c
void DrawHealthBar(int x, int y, int currentHP, int maxHP, ...)
```
- Draw HP bar with background
- Change color based on HP percentage (<30% = red)
- Display HP value (current / max)

```c
void DrawTurnIndicator()
```
- Show "Your Turn" or "Enemy Turn" text
- Display corresponding icon (GuiDrawIcon)

```c
void UnloadBattle()
```
- Unload player and enemy animations
- Stop and unload music streams

---

### animation.c - Animation Engine

**Responsibility:** Sprite-based animation system for player and enemy

**Core Concepts:**
- Each entity has multiple animation types (idle, attack, hurt, etc.)
- Each animation has: texture, frame count, frame speed, loop flag
- Frame-based animation with delta time interpolation

**Key Functions:**

```c
void InitPlayerAnimations()
```
- Initialize all player animation slots
- Currently uses placeholder (not loading actual textures)
- Sets up frame counts and speeds for each animation type

```c
void PlayPlayerAnimation(PlayerAnimType type, bool loop)
```
- Start playing specific animation
- Reset frame to 0
- Set loop flag
- Mark as playing

```c
void UpdatePlayerAnimation(float deltaTime)
```
- Advance frame timer based on deltaTime
- When timer exceeds frameSpeed, advance to next frame
- Handle looping vs one-shot animations
- Stop animation when finished (if not looping)

```c
bool IsPlayerAnimationFinished()
```
- Check if current animation has completed
- Returns true only for non-looping animations that reached the end

```c
void ResetPlayerToIdle()
```
- Switch to P_IDLE animation
- Set looping to true
- Reset playing flag

```c
void DrawPlayerSprite(int x, int y, float scale)
```
- Calculate source rectangle from sprite sheet
- Calculate destination rectangle with scale
- Draw current frame using DrawTexturePro

**Enemy Functions:**
- Same structure as player functions
- `InitEnemyAnimations(Enemy* enemy, int stage)`
- `PlayEnemyAnimation(Enemy* enemy, EnemyAnimType type)`
- `UpdateEnemyAnimation(Enemy* enemy, float dt)`
- `IsEnemyAnimationFinished(Enemy* enemy)`
- `ResetEnemyToIdle(Enemy* enemy)`
- `DrawEnemySprite(Enemy* enemy, int x, int y, float scale)`

---

### MathQuest.c - Math Question Generator

**Responsibility:** Dynamic math question generation based on difficulty

**Question Difficulty Progression:**

| Level | Type | Example | Range |
|-------|------|---------|-------|
| 1 | Addition/Subtraction | `5 + 3 = ?` | 1-10 |
| 2 | Add/Sub/Multiply | `7 * 3 = ?` | 1-15 |
| 3 | Four operations | `15 / 3 = ?` | 1-20 |
| 4 | Square root | `sqrt(25) = ?` | Perfect squares 10-50 |
| 5 | Parentheses | `(5 + 3) * 2 = ?` | 1-40, 3 numbers |
| 6 | Power of 2 | `7^2 = ?` | 1-60 |
| 7 | Division + Power | `100 / 5 * 3^2 = ?` | 50-100 |
| 8 | Sqrt + Cube | `sqrt(144) + 3^3 = ?` | 100-200 |
| 9 | Cube * N - Sqrt | `3^3 * 4 - sqrt(49) = ?` | Complex |
| 10 | Boss formula | `(200/10 + 3^2) * sqrt(64) = ?` | Extreme |

**Key Functions:**

```c
int generateRandomNumber(int min, int max)
```
- Generate random integer in range [min, max]

```c
char getRandomOperator(const char *operators)
```
- Select random operator from string ("+−*/")

```c
Question generateLevel1() ... generateLevel10()
```
- Generate question for specific level
- Return Question struct with:
  - `questionText`: Formatted string
  - `correctAnswer`: Integer result
  - `num1`, `num2`, `num3`, `num4`: Operands
  - `op`, `op2`: Operators

```c
Question generateQuestion(int level)
```
- Main entry point
- Call appropriate level generator (1-10)
- **NOTE**: Contains bug - calls `srand(time(NULL))` every time
  - Should be called once in InitBattle() instead

---

### logika.c - Logic Question Loader

**Responsibility:** Load and manage logic questions from text files

**File Format:**

**quest.txt:**
```
QUESTION: Jika semua kucing adalah hewan...
A: Semua kucing bisa bergerak
B: Semua hewan adalah kucing
C: Tidak ada kucing yang bisa bergerak
---
```

**answ.txt:**
```
ID:1 ANSWER:a
ID:2 ANSWER:b
ID:3 ANSWER:c
```

**Key Functions:**

```c
int muatSoal(const char *namaFile, Soal *daftarSoal, int maksSoal)
```
- Parse quest.txt line by line
- Extract QUESTION, A, B, C sections
- Store in array of Soal structs
- Return count of loaded questions

```c
int muatJawaban(const char *namaFile, KunciJawaban *daftarJawaban, int maksJawaban)
```
- Parse answ.txt with sscanf
- Extract ID and ANSWER (a/b/c)
- Store in KunciJawaban array
- Return count of loaded answers

```c
int ambilSoalAcak(...)
```
- Select random question from bank
- Find matching answer by ID
- Return question struct and answer char by reference
- Uses `rand() % jumlahSoal` for randomization

```c
const bool cekJawaban(char jawabanUser, char jawabanBenar)
```
- Normalize both chars to lowercase
- Compare equality
- Return true/false

---

### SaveSystem.c - Score Persistence

**Responsibility:** Save and load player scores from disk

**Directory Structure:**
```
history/
├── math/
│   ├── Alice.txt
│   └── Bob.txt
└── logic/
    ├── Alice.txt
    └── Bob.txt
```

**File Format:**
```
100
150
200
```
Each line is a score from one game session.

**Key Functions:**

```c
void saveScore(const char* player_name, char* mode, int score)
```
- Build file path: `history/<mode>/<player_name>.txt`
- Open file in append mode ("a")
- Write score as single line
- Close file

```c
playerHistory loadScore(const char* player_name, char* mode)
```
- Build file path: `history/<mode>/<player_name>.txt`
- Open file for reading
- Count lines (= score count)
- Allocate dynamic array
- Read all scores into array
- Return playerHistory struct with count and scores pointer
- If file doesn't exist, create empty file

**playerHistory Struct:**
```c
typedef struct {
    int count;      // Number of scores
    int* scores;    // Dynamic array of scores
} playerHistory;
```

**Usage Pattern:**
```c
// Save
saveScore(player.name, "math", player.score);

// Load
playerHistory history = loadScore(player.name, "math");
// ... use history.scores[i]
free(history.scores);  // Remember to free!
```

---

### lobby.c - Mode Selection & History

**Responsibility:** Game mode selection and score history display

**Features:**
- Animated UI with wave effects
- Mode selection (Math/Logic) with colored buttons
- Score history display (top 5, sorted descending)
- Smooth transitions

**Key Functions:**

```c
void InitLobby()
```
- Load background texture
- Reset animation timers
- Clear previous history data
- Initialize state to LOBBY

```c
GameState UpdateLobby()
```
- Update animation timers (waveTime, buttonBobTime, etc.)
- Return next state (LOBBY or GAMEPLAY)

```c
void DrawGameModePanel()
```
- Render mode selection panel (left side)
- Draw "SELECT GAME MODE" title with wave effect
- Render Math button (red, top)
- Render Logic button (blue, bottom)
- Handle button hover and click animations
- On click: Set player.currentMode, load history

```c
void DrawHistoryPanel()
```
- Render score history panel (right side)
- Load scores for selected mode
- Sort scores descending
- Display top 5 scores with medal icons:
  - 🥇 Gold (1st)
  - 🥈 Silver (2nd)
  - 🥉 Bronze (3rd)
  - Gray circles (4th-5th)

```c
void DrawStartButton()
```
- Render "START QUEST" button (bottom center)
- Bobbing animation effect
- On click: Transition to GAMEPLAY

**Helper Functions:**
```c
static void SortScoresDescending(int* scores, int count)
```
- Bubble sort algorithm
- Sorts in-place from highest to lowest

```c
static void ClearHistory()
```
- Free allocated memory for scores
- Reset count to 0

```c
static void DrawArc(...)
```
- Draw circular arc for medal decorations

---

### gameplay.c - Stage Transition Scene

**Responsibility:** Animated transition between stages

**Animation Sequence:**
1. **Fade In** (0.0-0.5s): Screen fades from black
2. **Player Walk** (0.0-1.5s): Player runs from left to center
3. **Stop & Idle** (1.5s): Player stops, switches to idle animation
4. **Dialog Slide In** (1.5-2.0s): Dialog box slides up from bottom
5. **Button Appear** (2.0s+): Yes/No buttons fade in
6. **User Choice**:
   - **YES**: Player runs right, fade out, transition to BATTLE
   - **NO**: Save score, transition to LOBBY

**Key Variables:**
```c
static float walkDuration = 1.5f;      // Walk animation time
static int playerStartX = -500;        // Off-screen left
static int playerStopX = 250;          // Center position
static int playerY = 100;              // Y position
static float playerScale = 7.0f;       // Sprite scale
static float exitDuration = 2.5f;      // Exit animation time
```

**Key Functions:**

```c
void InitGameplay()
```
- Load background and overlay textures
- Initialize player animations
- Start P_RUN animation
- Reset all animation timers and flags

```c
GameState UpdateGameplay()
```
- Update delta time
- Progress through animation states:
  - **Fade in**: Decrease fadeAlpha
  - **Walk**: Lerp player position, check if reached center
  - **Dialog slide**: Ease-in animation
  - **Button slide**: Staggered animation after dialog
  - **Exit**: Player runs off-screen, fade out
- Handle button clicks (YES/NO)
- Return next state when complete

```c
void DrawGameplay()
```
- Draw background layer
- Calculate and draw player sprite at current position
- Draw frame overlay (parallax effect)
- Draw dialog box with easing
- Draw buttons with slide-in animation
- Draw fade overlay (black rectangle)

**Easing Functions:**
```c
float easeOutCubic = 1.0f - powf(1.0f - t, 3.0f);  // Dialog
float easeOutQuad = 1.0f - (1.0f - t) * (1.0f - t); // Button
```

---

### mainMenu.c - Title Screen

**Responsibility:** Game entry point, player name input

**Features:**
- Animated title with wave effect
- Player name input (TextBox)
- "START GAME" button
- Smooth transitions

**Key Functions:**

```c
void InitMainMenu()
```
- Load background texture
- Reset animation timers
- Clear player name input

```c
GameState UpdateMainMenu()
```
- Update animation timers
- Return next state (LOBBY on button click)

```c
void DrawMainMenu()
```
- Render background
- Draw title with wave animation
- Draw player name input box
- Draw start button (disabled if name empty)
- Handle button click → Set player.name → LOBBY

---

## 📊 Data Structures

### Question (MathQuest.h)
```c
typedef struct {
    char questionText[256];  // Formatted question
    int correctAnswer;       // Integer result
    int num1, num2, num3, num4; // Operands
    char op, op2;            // Operators
} Question;
```

### Soal (logika.h)
```c
typedef struct {
    int nomor;                  // Question ID
    char pertanyaan[MAKS_TEKST]; // Question text
    char pilihanA[MAKS_TEKST];   // Option A
    char pilihanB[MAKS_TEKST];   // Option B
    char pilihanC[MAKS_TEKST];   // Option C
} Soal;
```

### KunciJawaban (logika.h)
```c
typedef struct {
    int nomor;      // Question ID
    char jawaban;   // Answer (a/b/c)
} KunciJawaban;
```

### DamageText (battle.h)
```c
typedef struct {
    char text[20];      // Damage text ("−10", "MISS")
    float x, y;         // Position
    float alpha;        // Transparency
    float lifetime;     // Remaining time
    Color color;        // Text color
    bool active;        // Is visible?
} DamageText;
```

### AnimationData (global.h)
```c
typedef struct {
    Texture2D texture;   // Sprite sheet
    int frameCount;      // Total frames
    float frameSpeed;    // Seconds per frame
    bool loop;           // Loop animation?
} AnimationData;
```

### playerHistory (SaveSystem.h)
```c
typedef struct {
    int count;      // Number of scores
    int* scores;    // Dynamic array
} playerHistory;
```

---

## 🔧 API Reference

### Global Functions

#### Player Management
```c
void InitPlayerAnimations()
void PlayPlayerAnimation(PlayerAnimType type, bool loop)
void UpdatePlayerAnimation(float deltaTime)
bool IsPlayerAnimationFinished()
void ResetPlayerToIdle()
void DrawPlayerSprite(int x, int y, float scale)
void UnloadPlayerAnimations()
```

#### Enemy Management
```c
void InitEnemyAnimations(Enemy* enemy, int stage)
void PlayEnemyAnimation(Enemy* enemy, EnemyAnimType type)
void UpdateEnemyAnimation(Enemy* enemy, float dt)
bool IsEnemyAnimationFinished(Enemy* enemy)
void ResetEnemyToIdle(Enemy* enemy)
void DrawEnemySprite(Enemy* enemy, int x, int y, float scale)
void UnloadEnemyAnimations(Enemy* enemy)
```

#### Question Generation
```c
Question generateQuestion(int level)        // Math questions
int ambilSoalAcak(...)                     // Logic questions
bool cekJawaban(char user, char correct)   // Validate logic answer
```

#### Save System
```c
void saveScore(const char* name, char* mode, int score)
playerHistory loadScore(const char* name, char* mode)
```

#### Battle System
```c
void InitBattle(int stage)
GameState UpdateBattle()
void DrawBattleGUI()
void UnloadBattle()
bool IsAnswerCorrect()
void ExecutePlayerDamage(bool correct)
void ExecuteEnemyDamage(bool correct)
void QuestionGeneration()
void DrawQuizInterface(int screenW, int screenH)
void DrawHealthBar(int x, int y, int hp, int maxHP, ...)
void DrawTurnIndicator()
```

#### State Management
```c
// Main Menu
void InitMainMenu()
GameState UpdateMainMenu()
void DrawMainMenu()
void UnloadMainMenu()

// Lobby
void InitLobby()
GameState UpdateLobby()
void DrawLobby()
void UnloadLobby()

// Gameplay
void InitGameplay()
GameState UpdateGameplay()
void DrawGameplay()
void UnloadGameplay()
```

---

## 🛠 Build & Deployment

### Debug Build

**Command:**
```bash
gcc -g -O0 -Wall src/*.c -o bin/game_debug.exe \
    -Iinclude -Llib -lraylib -lopengl32 -lgdi32 -lwinmm
```

**Flags:**
- `-g`: Include debug symbols
- `-O0`: No optimization (easier debugging)
- `-Wall`: Show all warnings

**VS Code Task:** Press `Ctrl+Shift+B` → Select "Build Game (Debug)"

### Release Build

**Command:**
```bash
gcc -O3 -Wall -s -mwindows src/*.c -o bin/EduQuest.exe \
    -Iinclude -Llib -lraylib -lopengl32 -lgdi32 -lwinmm
```

**Flags:**
- `-O3`: Maximum optimization
- `-s`: Strip debug symbols (smaller file)
- `-mwindows`: Hide console (GUI only)

**VS Code Task:** Press `Ctrl+Shift+B` → Select "Build Game (Release)"

### Distribution Package

Include these files for distribution:
```
EduQuest/
├── EduQuest.exe
├── quest.txt
├── answ.txt
├── assets/
│   ├── audio/
│   ├── background/
│   ├── bg_1/
│   ├── Player/
│   ├── Enemy1-10/
│   └── icon/
└── history/
    ├── math/
    └── logic/
```

### System Requirements

**Minimum:**
- OS: Windows 7 or higher
- CPU: Dual-core 1.5 GHz
- RAM: 512 MB
- GPU: OpenGL 3.3 compatible
- Storage: 100 MB

**Recommended:**
- OS: Windows 10/11
- CPU: Quad-core 2.0 GHz
- RAM: 2 GB
- GPU: Dedicated GPU with OpenGL 4.0+

---

## 📁 Asset Structure

### Audio Assets
```
assets/audio/
└── bgm/
    ├── battle.mp3         # Battle background music
    └── gameover.mp3       # Game over music
```

### Visual Assets
```
assets/
├── background/
│   └── background_MainMenu.png    # Main menu background
├── bg_1/
│   ├── BG_Gameplay.png            # Gameplay background layer
│   └── BG_GameplayOverlay.png     # Gameplay foreground layer
├── icon/
│   └── icon.png                   # Window icon
├── Player/
│   ├── idle/                      # Idle animation frames
│   ├── basic_attack/              # Attack animation
│   ├── skill/                     # Skill animation
│   ├── ultimate/                  # Ultimate animation
│   ├── hurt/                      # Hurt animation
│   └── death/                     # Death animation
└── Enemy1-10/
    ├── idle/
    ├── attack/
    ├── hurt/
    └── death/
```

### Data Files
```
quest.txt           # Logic questions (128 questions)
answ.txt            # Logic answers (128 answers)
```

---

---



## 🎯 Future Enhancements

### Planned Features
1. **More Game Modes**: Geography, History, Science
2. **Multiplayer**: Local or online battles
3. **Achievement System**: Unlock rewards for milestones
4. **Power-ups**: Temporary boosts during battle
5. **Character Customization**: Choose player sprite
6. **Sound Effects**: Attack sounds, UI clicks
7. **Difficulty Settings**: Easy/Normal/Hard modes
8. **Leaderboard**: Global score ranking

### Technical Improvements
1. **Asset Preloading**: Load all textures at startup
2. **Config File**: JSON/XML for game settings
3. **Localization**: Multi-language support
4. **Error Handling**: Better error messages
5. **Unit Tests**: Automated testing framework
6. **Profiling**: Performance measurement tools

---

## 📚 References

### Documentation
- [Raylib Official Docs](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [raygui Controls](https://github.com/raysan5/raygui)
- [C Programming Guide](https://en.cppreference.com/w/c)

### Assets
- Background images: Free assets on internet
- Sprite sheets: Free assets on internet
- Music: Royalty-free tracks

### Libraries
- **Raylib 5.0**: Graphics, audio, input
- **raygui**: Immediate-mode GUI
- **Standard C Library**: stdio, stdlib, string, math, time

---

## 👥 Credits

**Developer:** Ikhsan, Ayu, Wildan (Team Edu-Quest)

**Framework:** Raylib by Ramon Santamaria

**Version:** 1.0.0

**License:** Educational Project

---

**Last Updated:** December 15, 2025

