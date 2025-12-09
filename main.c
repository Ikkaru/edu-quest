#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MathQuest.h"

// Fungsi untuk menampilkan banner level
void displayLevelBanner(int level) {
    printf("\n========================================\n");
    printf("           LEVEL %d\n", level);
    printf("========================================\n");
}

int main() {
    int currentLevel = 1;
    int totalScore = 0;
    int maxLevel = 10; // Diubah dari 12 menjadi 10
    char playAgain;
    
    printf("========================================\n");
    printf("       SELAMAT DATANG DI MATH QUEST     \n");
    printf("           Game Matematika Seru         \n");
    printf("========================================\n\n");
    
    do {
        currentLevel = 1;
        totalScore = 0;
        
        while (currentLevel <= maxLevel) {
            displayLevelBanner(currentLevel);
            
            // Generate soal berdasarkan level
            Question q = generateQuestion(currentLevel);
            
            // Tampilkan soal
            printf("\nSoal: %s\n", q.questionText);
            printf("----------------------------------------\n");
            
            int userAnswer;
            int answeredCorrectly = 0;
            
            while (!answeredCorrectly) {
                printf("Jawaban kamu: ");
                
                // Validasi input
                if (scanf("%d", &userAnswer) != 1) {
                    printf("Input tidak valid! Masukkan angka.\n");
                    while (getchar() != '\n');
                    continue;
                }
                
                // Cek jawaban
                if (userAnswer == q.correctAnswer) {
                    totalScore += (10 * currentLevel);
                    printf("\n>> BENAR! Skor kamu sekarang: %d\n", totalScore);
                    printf("----------------------------------------\n");
                    
                    if (currentLevel < maxLevel) {
                        printf("\n>> Lanjut ke LEVEL %d\n", currentLevel + 1);
                        printf("Tekan Enter untuk melanjutkan...");
                        getchar();
                        getchar();
                    }
                    answeredCorrectly = 1;
                } else {
                    printf("\n>> SALAH! Coba lagi.\n");
                }
            }
            
            currentLevel++;
        }
        
        // Tampilkan hasil akhir
        printf("\n========================================\n");
        printf("           PERMAINAN SELESAI!          \n");
        printf("========================================\n");
        printf(">> TOTAL SKOR AKHIR: %d <<\n", totalScore);
        printf(">> Level tertinggi yang dicapai: %d\n", maxLevel);
        
        // Sesuaikan threshold ranking untuk 10 level
        if (totalScore >= 550) { // Maksimal skor: 10*(1+2+3+4+5+6+7+8+9+10) = 550
            printf("\n>> RANK: MATH GENIUS! <<\n");
        } else if (totalScore >= 400) {
            printf("\n>> RANK: MATH MASTER! <<\n");
        } else if (totalScore >= 250) {
            printf("\n>> RANK: MATH EXPERT! <<\n");
        } else {
            printf("\n>> RANK: MATH BEGINNER! <<\n");
        }
        
        printf("\n========================================\n");
        printf("Ingin bermain lagi? (y/n): ");
        scanf(" %c", &playAgain);
        while (getchar() != '\n');
        
    } while (playAgain == 'y' || playAgain == 'Y');
    
    printf("\n========================================\n");
    printf("      Terima kasih telah bermain!      \n");
    printf("========================================\n");
    
    return 0;
}