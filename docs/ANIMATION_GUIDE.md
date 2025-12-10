# 🎬 ANIMATION SYSTEM - QUICK GUIDE

## 📋 Fungsi-Fungsi Penting

### **PLAYER ANIMATION**

#### `PlayPlayerAnimation(PlayerAnimType animType, bool shouldLoop)`
**Fungsi:** Langsung memainkan animasi player yang diinginkan  
**Parameter:**
- `animType`: Jenis animasi (P_IDLE, P_ATTACK_1, P_ATTACK_2, P_ATTACK_3, P_HURT)
- `shouldLoop`: `true` = loop terus, `false` = main sekali lalu stop

**Contoh:**
```c
// Main attack animation (tidak loop)
PlayPlayerAnimation(P_ATTACK_1, false);

// Main idle animation (loop terus)
PlayPlayerAnimation(P_IDLE, true);

// Main hurt animation (tidak loop)
PlayPlayerAnimation(P_HURT, false);
```

---

#### `UpdatePlayerAnimation(float deltaTime)`
**Fungsi:** Update frame animasi (PANGGIL SETIAP FRAME!)  
**Parameter:** `deltaTime` = GetFrameTime()

**Contoh:**
```c
// Di dalam game loop
float dt = GetFrameTime();
UpdatePlayerAnimation(dt);
```

---

#### `IsPlayerAnimationFinished()`
**Fungsi:** Cek apakah animasi sudah selesai  
**Return:** `true` jika animasi non-loop sudah selesai

**Contoh:**
```c
if (IsPlayerAnimationFinished()) {
    // Attack animation selesai, kembali ke idle
    ResetPlayerToIdle();
}
```

---

#### `ResetPlayerToIdle()`
**Fungsi:** Kembalikan player ke animasi IDLE  
**Shortcut dari:** `PlayPlayerAnimation(P_IDLE, true)`

---

### **ENEMY ANIMATION**

#### `PlayEnemyAnimation(Enemy* enemy, EnemyAnimType animType)`
**Fungsi:** Main animasi enemy  
**Parameter:**
- `enemy`: Pointer ke enemy struct
- `animType`: E_IDLE, E_ATTACK, E_HURT

**Contoh:**
```c
Enemy* currentEnemy = &enemies[0];

// Enemy menyerang
PlayEnemyAnimation(currentEnemy, E_ATTACK);

// Enemy kena damage
PlayEnemyAnimation(currentEnemy, E_HURT);
```

---

#### `UpdateEnemyAnimation(Enemy* enemy, float deltaTime)`
**Fungsi:** Update frame animasi enemy (PANGGIL SETIAP FRAME!)

**Contoh:**
```c
float dt = GetFrameTime();
UpdateEnemyAnimation(currentEnemy, dt);
```

---

#### `IsEnemyAnimationFinished(Enemy* enemy)`
**Fungsi:** Cek apakah animasi enemy sudah selesai

**Contoh:**
```c
if (IsEnemyAnimationFinished(currentEnemy)) {
    ResetEnemyToIdle(currentEnemy);
}
```

---

#### `ResetEnemyToIdle(Enemy* enemy)`
**Fungsi:** Kembalikan enemy ke animasi IDLE

---

## 🎮 CONTOH PENGGUNAAN LENGKAP

### **1. Initialization (Panggil sekali di awal)**
```c
void InitBattle(int stage) {
    // Setup animasi player (panggil sekali saja)
    InitPlayerAnimations();
    
    // Setup animasi enemy
    Enemy* enemy = &enemies[stage];
    InitEnemyAnimations(enemy);
}
```

---

### **2. Game Loop (Panggil setiap frame)**
```c
void UpdateBattle(Enemy* enemy) {
    float dt = GetFrameTime();
    
    // WAJIB: Update animasi setiap frame
    UpdatePlayerAnimation(dt);
    UpdateEnemyAnimation(enemy, dt);
    
    // ... game logic lainnya ...
}
```

---

### **3. Trigger Attack Animation**
```c
void PlayerAttack() {
    // Jawaban benar → Main attack animation
    if (answerCorrect) {
        PlayPlayerAnimation(P_ATTACK_1, false);
        
        // Tunggu animasi selesai, lalu damage enemy
    }
}

// Cek di game loop:
if (IsPlayerAnimationFinished()) {
    // Attack selesai, apply damage
    enemy->HP -= 20;
    
    // Trigger enemy hurt
    PlayEnemyAnimation(enemy, E_HURT);
    
    // Reset player ke idle
    ResetPlayerToIdle();
}
```

---

### **4. Enemy Turn**
```c
void EnemyAttack(Enemy* enemy) {
    // Enemy menyerang
    PlayEnemyAnimation(enemy, E_ATTACK);
}

// Cek di game loop:
if (IsEnemyAnimationFinished(enemy)) {
    // Attack selesai, player take damage
    if (answerWrong) {
        player.HP -= enemy->damage;
        PlayPlayerAnimation(P_HURT, false);
    }
    
    // Reset enemy ke idle
    ResetEnemyToIdle(enemy);
}
```

---

### **5. Complete Battle Flow Example**
```c
typedef enum {
    BATTLE_IDLE,
    BATTLE_PLAYER_ATTACK,
    BATTLE_ENEMY_HURT,
    BATTLE_ENEMY_ATTACK,
    BATTLE_PLAYER_HURT
} BattlePhase;

BattlePhase phase = BATTLE_IDLE;

void BattleUpdate(Enemy* enemy) {
    float dt = GetFrameTime();
    
    // ALWAYS update animations
    UpdatePlayerAnimation(dt);
    UpdateEnemyAnimation(enemy, dt);
    
    switch (phase) {
        case BATTLE_IDLE:
            // Player pilih action
            if (GuiButton(..., "Attack")) {
                PlayPlayerAnimation(P_ATTACK_1, false);
                phase = BATTLE_PLAYER_ATTACK;
            }
            break;
            
        case BATTLE_PLAYER_ATTACK:
            // Tunggu attack animation selesai
            if (IsPlayerAnimationFinished()) {
                // Apply damage
                enemy->HP -= 20;
                
                // Trigger enemy hurt
                PlayEnemyAnimation(enemy, E_HURT);
                ResetPlayerToIdle();
                
                phase = BATTLE_ENEMY_HURT;
            }
            break;
            
        case BATTLE_ENEMY_HURT:
            // Tunggu enemy hurt animation selesai
            if (IsEnemyAnimationFinished(enemy)) {
                ResetEnemyToIdle(enemy);
                
                // Enemy turn
                PlayEnemyAnimation(enemy, E_ATTACK);
                phase = BATTLE_ENEMY_ATTACK;
            }
            break;
            
        case BATTLE_ENEMY_ATTACK:
            // Tunggu enemy attack selesai
            if (IsEnemyAnimationFinished(enemy)) {
                // Player take damage
                player.HP -= enemy->damage;
                PlayPlayerAnimation(P_HURT, false);
                
                ResetEnemyToIdle(enemy);
                phase = BATTLE_PLAYER_HURT;
            }
            break;
            
        case BATTLE_PLAYER_HURT:
            // Tunggu player hurt animation selesai
            if (IsPlayerAnimationFinished()) {
                ResetPlayerToIdle();
                phase = BATTLE_IDLE; // Back to player turn
            }
            break;
    }
}
```

---

## 📊 Jenis-Jenis Animasi

### **Player Animations:**
- `WALKING` - Jalan (loop)
- `P_IDLE` - Diam/standby (loop)
- `P_ATTACK_1` - Basic attack (no loop)
- `P_ATTACK_2` - Skill attack (no loop)
- `P_ATTACK_3` - Ultimate attack (no loop)
- `P_HURT` - Kena damage (no loop)

### **Enemy Animations:**
- `E_IDLE` - Diam/standby (loop)
- `E_ATTACK` - Menyerang (no loop)
- `E_HURT` - Kena damage (no loop)

---

## ⚠️ PENTING!

1. **SELALU panggil `UpdatePlayerAnimation()` dan `UpdateEnemyAnimation()` di game loop**
2. **Jangan lupa reset ke IDLE** setelah animasi non-loop selesai
3. **Loop = true** untuk IDLE, **Loop = false** untuk ATTACK/HURT
4. **Cek `IsAnimationFinished()`** sebelum pindah state

---

## 🚀 Quick Start

**Minimal code untuk test animasi:**
```c
// 1. Init (sekali)
InitPlayerAnimations();
InitEnemyAnimations(&enemies[0]);

// 2. Game loop (setiap frame)
float dt = GetFrameTime();
UpdatePlayerAnimation(dt);
UpdateEnemyAnimation(&enemies[0], dt);

// 3. Trigger animasi (kapan saja)
if (IsKeyPressed(KEY_SPACE)) {
    PlayPlayerAnimation(P_ATTACK_1, false);
}

// 4. Auto reset ke idle setelah selesai
if (IsPlayerAnimationFinished()) {
    ResetPlayerToIdle();
}
```

---

## 📝 Notes

- Semua timing dan frame count bisa diubah di `InitPlayerAnimations()` dan `InitEnemyAnimations()`
- Texture loading akan ditambahkan nanti sesuai sprite sheet yang kamu punya
- Functions ini sudah handle semua logic animasi, kamu tinggal panggil!
