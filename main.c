#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "MathQuest.h"

// Fungsi untuk menampilkan banner level
void displayLevelBanner(int level) {
    printf("\n========================================\n");
    printf("           LEVEL %d\n", level);
    printf("========================================\n");
}

// Fungsi untuk menampilkan hasil
void displayResult(int isCorrect, int score, int level) {
    printf("\n");
    if (isCorrect) {
        printf(">> BENAR! Jawaban kamu tepat.\n");
        printf(">> Skor: %d\n", score);
    } else {
        printf(">> SALAH. Coba lagi!\n");
    }
    printf("----------------------------------------\n");
}

// Fungsi untuk mengecek apakah string berisi karakter tertentu
int containsChar(const char *str, const char *search) {
    return strstr(str, search) != NULL;
}

int main() {
    int currentLevel = 1;
    int totalScore = 0;
    int maxLevel = 12;
    char playAgain;
    
    printf("========================================\n");
    printf("       SELAMAT DATANG DI MATH QUEST     \n");
    printf("           Game Matematika Seru         \n");
    printf("========================================\n\n");
    
    do {
        // Reset level jika ingin bermain lagi
        currentLevel = 1;
        totalScore = 0;
        
        while (currentLevel <= maxLevel) {
            displayLevelBanner(currentLevel);
            
            // Generate soal berdasarkan level
            Question q = generateQuestion(currentLevel);
            
            // Tampilkan soal dengan batas waktu
            printf("\nSoal: %s\n", q.questionText);
            printf("Batas waktu: %d detik\n", q.timeLimit);
            printf("----------------------------------------\n");
            
            int userAnswer;
            int attempts = 0;
            int maxAttempts = 3;
            
            while (attempts < maxAttempts) {
                printf("Jawaban kamu: ");
                
                // Validasi input
                if (scanf("%d", &userAnswer) != 1) {
                    printf("Input tidak valid! Masukkan angka.\n");
                    while (getchar() != '\n'); // Clear input buffer
                    continue;
                }
                
                // Cek jawaban
                if (userAnswer == q.correctAnswer) {
                    totalScore += (10 * currentLevel); // Skor berdasarkan level
                    displayResult(1, totalScore, currentLevel);
                    
                    // Tampilkan pesan khusus berdasarkan level
                    if (currentLevel < maxLevel) {
                        printf("\n>> SELAMAT! Lanjut ke LEVEL %d <<\n", currentLevel + 1);
                        printf("Tekan Enter untuk melanjutkan...");
                        getchar(); // Consume newline
                        getchar(); // Wait for Enter
                    }
                    break;
                } else {
                    attempts++;
                    printf("\n>> Jawaban salah!");
                    
                    if (attempts < maxAttempts) {
                        printf(" Kesempatan: %d/%d\n", maxAttempts - attempts, maxAttempts);
                        printf("Coba lagi: ");
                    } else {
                        printf("\n\n>> Kesempatan habis! Jawaban yang benar: %d\n", q.correctAnswer);
                        printf("Tekan Enter untuk melanjutkan...");
                        getchar(); // Consume newline
                        getchar(); // Wait for Enter
                        
                        // Tampilkan penjelasan jika ada
                        // Gunakan strstr() untuk menghindari masalah karakter multi-byte
                        if (containsChar(q.questionText, "²") || containsChar(q.questionText, "^2")) {
                            printf("\n>> Tips: %d pangkat 2 = %d × %d = %d\n", 
                                   q.num1, q.num1, q.num1, q.correctAnswer);
                        } else if (containsChar(q.questionText, "√") || containsChar(q.questionText, "sqrt")) {
                            printf("\n>> Tips: Akar kuadrat dari %d = %d karena %d × %d = %d\n", 
                                   q.num1, q.correctAnswer, q.correctAnswer, q.correctAnswer, q.num1);
                        } else if (containsChar(q.questionText, "³") || containsChar(q.questionText, "^3")) {
                            printf("\n>> Tips: %d pangkat 3 = %d × %d × %d = %d\n", 
                                   q.num1, q.num1, q.num1, q.num1, q.correctAnswer);
                        }
                    }
                }
            }
            
            currentLevel++;
            printf("\n");
        }
        
        // Tampilkan hasil akhir
        printf("\n========================================\n");
        printf("           PERMAINAN SELESAI!          \n");
        printf("========================================\n");
        printf(">> TOTAL SKOR AKHIR: %d <<\n", totalScore);
        printf(">> Level tertinggi: %d\n", maxLevel);
        
        if (totalScore >= 1000) {
            printf("\n>> RANK: MATH GENIUS! <<\n");
        } else if (totalScore >= 700) {
            printf("\n>> RANK: MATH MASTER! <<\n");
        } else if (totalScore >= 400) {
            printf("\n>> RANK: MATH EXPERT! <<\n");
        } else {
            printf("\n>> RANK: MATH BEGINNER! <<\n");
        }
        
        printf("\n========================================\n");
        printf("Ingin bermain lagi? (y/n): ");
        scanf(" %c", &playAgain);
        while (getchar() != '\n'); // Clear input buffer
        
    } while (playAgain == 'y' || playAgain == 'Y');
    
    printf("\n========================================\n");
    printf("      Terima kasih telah bermain!      \n");
    printf("========================================\n");
    
    return 0;
}