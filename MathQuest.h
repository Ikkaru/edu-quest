#ifndef MATHQUEST_H
#define MATHQUEST_H

// Struktur untuk menyimpan soal
typedef struct {
    char questionText[100];
    int num1;
    int num2;
    int num3;
    int num4;
    char op;
    char op2;
    int correctAnswer;
} Question;

// Deklarasi fungsi-fungsi
Question generateQuestion(int level);
int generateRandomNumber(int min, int max);
char getRandomOperator(const char *operators);

// Fungsi untuk setiap level (10 level setelah hapus level 4 dan 8)
Question generateLevel1();
Question generateLevel2();
Question generateLevel3();
Question generateLevel4(); // Level 4 baru (sebelumnya level 5)
Question generateLevel5(); // Level 5 baru (sebelumnya level 6)
Question generateLevel6(); // Level 6 baru (sebelumnya level 7)
Question generateLevel7(); // Level 7 baru (sebelumnya level 9)
Question generateLevel8(); // Level 8 baru (sebelumnya level 10)
Question generateLevel9(); // Level 9 baru (sebelumnya level 11)
Question generateLevel10(); // Level 10 baru (sebelumnya level 12)

#endif