#ifndef MATHQUEST_H
#define MATHQUEST_H

// Struktur untuk menyimpan soal
typedef struct {
    char questionText[100];
    int num1;
    int num2;
    int num3;
    int num4;
    char op;           // GANTI: operator -> op
    char op2;          // GANTI: operator2 -> op2
    int correctAnswer;
    int timeLimit;
} Question;

// Deklarasi fungsi-fungsi
Question generateQuestion(int level);
int generateRandomNumber(int min, int max);
char getRandomOperator(const char *operators);

// Fungsi untuk setiap level
Question generateLevel1();
Question generateLevel2();
Question generateLevel3();
Question generateLevel4();
Question generateLevel5();
Question generateLevel6();
Question generateLevel7();
Question generateLevel8();
Question generateLevel9();
Question generateLevel10();
Question generateLevel11();
Question generateLevel12();

#endif