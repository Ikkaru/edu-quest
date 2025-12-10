// Battle Logic
// #include "global.h"
// #include "MathQuest.h"
// #include <stdbool.h>

// // Variabel Soal 
// Question question;

// // Konstanta Timer
// const float QUESTION_TIME_LIMIT = 20.0f;
// float timer = 0.0f;

// void Battle(int stage) {
//     // Initialize Enemy
//     Enemy currentEnemy = enemies[stage];
//     BattleState currentBattleState = BATTLE_PLAYER_CHOICE;

//     PlayerChoice playerChoice;

//     // State Check
//     switch (currentBattleState) {
//         case BATTLE_PLAYER_CHOICE:
//             switch (playerChoice)
//             {
//             case BASIC_ATTACK:
//                 currentBattleState = BATTLE_PLAYER_QUIZ;
//                 BasicAttack();
//                 break;
            
//             case SKILL:
//                 break;
//             }
//     } 

// }

// bool BasicAttack() {
//     question = generateQuestion(player.stage);
//     playerChoice = BASIC_ATTACK;
//     // Logic for asking question and processing answer
// }


// void drawBattleUI() {
//     // Draw Player and Enemy HP Bars
//     return true;
// }