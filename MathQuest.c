#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "MathQuest.h"

// Fungsi untuk menghasilkan angka acak dalam range
int generateRandomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Fungsi untuk memilih operator acak dari string
char getRandomOperator(const char *operators) {
    int count = 0;
    while (operators[count] != '\0') count++;
    return operators[rand() % count];
}

// LEVEL 1: Sangat Mudah
Question generateLevel1() {
    Question q;
    int num1 = generateRandomNumber(1, 10);
    int num2 = generateRandomNumber(1, 10);
    char op = getRandomOperator("+-");
    
    q.num1 = num1;
    q.num2 = num2;
    q.op = op;  // GANTI: operator -> op
    
    if (op == '+') {
        q.correctAnswer = num1 + num2;
        sprintf(q.questionText, "%d %c %d = ?", num1, op, num2);
    } else {
        // Pastikan hasil pengurangan tidak negatif
        if (num1 < num2) {
            int temp = num1;
            num1 = num2;
            num2 = temp;
        }
        q.num1 = num1;
        q.num2 = num2;
        q.correctAnswer = num1 - num2;
        sprintf(q.questionText, "%d %c %d = ?", num1, op, num2);
    }
    
    q.timeLimit = 10;
    return q;
}

// LEVEL 2: Mudah
Question generateLevel2() {
    Question q;
    int num1 = generateRandomNumber(1, 15);
    int num2 = generateRandomNumber(1, 15);
    char op = getRandomOperator("+-*");
    
    q.num1 = num1;
    q.num2 = num2;
    q.op = op;  // GANTI: operator -> op
    
    switch(op) {
        case '+':
            q.correctAnswer = num1 + num2;
            break;
        case '-':
            if (num1 < num2) {
                int temp = num1;
                num1 = num2;
                num2 = temp;
                q.num1 = num1;
                q.num2 = num2;
            }
            q.correctAnswer = num1 - num2;
            break;
        case '*':
            q.correctAnswer = num1 * num2;
            break;
    }
    
    sprintf(q.questionText, "%d %c %d = ?", q.num1, op, q.num2);
    q.timeLimit = 12;
    return q;
}

// LEVEL 3: Mudah+
Question generateLevel3() {
    Question q;
    int num1 = generateRandomNumber(1, 20);
    int num2 = generateRandomNumber(1, 20);
    char op = getRandomOperator("+-*/");
    
    q.num1 = num1;
    q.num2 = num2;
    q.op = op;  // GANTI: operator -> op
    
    if (op == '/') {
        // Pastikan pembagian menghasilkan bilangan bulat
        while (num2 == 0 || num1 % num2 != 0) {
            num1 = generateRandomNumber(1, 20);
            num2 = generateRandomNumber(1, 20);
        }
        q.num1 = num1;
        q.num2 = num2;
        q.correctAnswer = num1 / num2;
    } else if (op == '*') {
        q.correctAnswer = num1 * num2;
    } else if (op == '+') {
        q.correctAnswer = num1 + num2;
    } else {
        if (num1 < num2) {
            int temp = num1;
            num1 = num2;
            num2 = temp;
            q.num1 = num1;
            q.num2 = num2;
        }
        q.correctAnswer = num1 - num2;
    }
    
    sprintf(q.questionText, "%d %c %d = ?", q.num1, op, q.num2);
    q.timeLimit = 15;
    return q;
}

// LEVEL 4: Sedang (Campur 2 operator)
Question generateLevel4() {
    Question q;
    int num1 = generateRandomNumber(1, 25);
    int num2 = generateRandomNumber(1, 25);
    int num3 = generateRandomNumber(1, 25);
    char op1 = getRandomOperator("+-*");
    char op2 = getRandomOperator("+-*");
    
    // Hindari pembagian dengan 0
    if (op2 == '/' && num3 == 0) num3 = 1;
    
    q.num1 = num1;
    q.num2 = num2;
    q.num3 = num3;
    q.op = op1;   // GANTI: operator -> op
    q.op2 = op2;  // GANTI: operator2 -> op2
    
    // Hitung sesuai urutan operasi
    int result;
    if ((op1 == '+' || op1 == '-') && (op2 == '*' || op2 == '/')) {
        // op2 dihitung dulu
        if (op2 == '*') result = num2 * num3;
        else if (op2 == '/') {
            while (num3 == 0 || num2 % num3 != 0) {
                num2 = generateRandomNumber(1, 25);
                num3 = generateRandomNumber(1, 25);
            }
            result = num2 / num3;
        } else {
            result = num2 + num3;
        }
        
        if (op1 == '+') q.correctAnswer = num1 + result;
        else q.correctAnswer = num1 - result;
    } else {
        // Dari kiri ke kanan
        if (op1 == '+') result = num1 + num2;
        else if (op1 == '-') result = num1 - num2;
        else if (op1 == '*') result = num1 * num2;
        else {
            while (num2 == 0 || num1 % num2 != 0) {
                num1 = generateRandomNumber(1, 25);
                num2 = generateRandomNumber(1, 25);
            }
            result = num1 / num2;
        }
        
        if (op2 == '+') q.correctAnswer = result + num3;
        else if (op2 == '-') q.correctAnswer = result - num3;
        else if (op2 == '*') q.correctAnswer = result * num3;
        else {
            while (num3 == 0 || result % num3 != 0) {
                num3 = generateRandomNumber(1, 25);
            }
            q.correctAnswer = result / num3;
        }
    }
    
    sprintf(q.questionText, "%d %c %d %c %d = ?", num1, op1, num2, op2, num3);
    q.timeLimit = 18;
    return q;
}

// LEVEL 5: Sedang (Akar sederhana)
Question generateLevel5() {
    Question q;
    int num = generateRandomNumber(10, 50);
    int perfectSquare;
    
    // Cari bilangan kuadrat sempurna terdekat
    do {
        num = generateRandomNumber(10, 50);
        int root = (int)sqrt(num);
        perfectSquare = root * root;
    } while (perfectSquare != num);
    
    q.num1 = num;
    q.correctAnswer = (int)sqrt(num);
    sprintf(q.questionText, "√%d = ?", num);
    q.timeLimit = 20;
    return q;
}

// LEVEL 6: Sedang+ (Campur 3 operator dengan tanda kurung)
Question generateLevel6() {
    Question q;
    int num1 = generateRandomNumber(1, 40);
    int num2 = generateRandomNumber(1, 40);
    int num3 = generateRandomNumber(1, 40);
    char op1 = getRandomOperator("+-*");
    char op2 = getRandomOperator("+-*");
    
    q.num1 = num1;
    q.num2 = num2;
    q.num3 = num3;
    q.op = op1;   // GANTI: operator -> op
    q.op2 = op2;  // GANTI: operator2 -> op2
    
    // Selalu hitung dalam kurung dulu
    int inParentheses;
    if (op1 == '+') inParentheses = num1 + num2;
    else if (op1 == '-') inParentheses = num1 - num2;
    else inParentheses = num1 * num2;
    
    if (op2 == '+') q.correctAnswer = inParentheses + num3;
    else if (op2 == '-') q.correctAnswer = inParentheses - num3;
    else if (op2 == '*') q.correctAnswer = inParentheses * num3;
    else {
        while (num3 == 0 || inParentheses % num3 != 0) {
            num3 = generateRandomNumber(1, 40);
        }
        q.correctAnswer = inParentheses / num3;
    }
    
    sprintf(q.questionText, "(%d %c %d) %c %d = ?", num1, op1, num2, op2, num3);
    q.timeLimit = 22;
    return q;
}

// LEVEL 7: Sulit (Pangkat 2)
Question generateLevel7() {
    Question q;
    int num = generateRandomNumber(1, 60);
    
    q.num1 = num;
    q.correctAnswer = num * num;
    sprintf(q.questionText, "%d² = ?", num);
    q.timeLimit = 25;
    return q;
}

// LEVEL 8: Sulit (Semua operator)
Question generateLevel8() {
    Question q;
    int num1 = generateRandomNumber(1, 80);
    int num2 = generateRandomNumber(1, 80);
    int num3 = generateRandomNumber(1, 80);
    
    // Pastikan num3 adalah bilangan kuadrat sempurna untuk akar
    int root = (int)sqrt(num3);
    while (root * root != num3) {
        num3 = generateRandomNumber(1, 80);
        root = (int)sqrt(num3);
    }
    
    q.num1 = num1;
    q.num2 = num2;
    q.num3 = num3;
    
    // Hitung: num1 / num2 + √num3
    while (num2 == 0 || num1 % num2 != 0) {
        num1 = generateRandomNumber(1, 80);
        num2 = generateRandomNumber(1, 80);
    }
    
    q.correctAnswer = (num1 / num2) + (int)sqrt(num3);
    sprintf(q.questionText, "%d ÷ %d + √%d = ?", num1, num2, num3);
    q.timeLimit = 28;
    return q;
}

// LEVEL 9: Sangat Sulit
Question generateLevel9() {
    Question q;
    int num1 = generateRandomNumber(50, 100);
    int num2 = generateRandomNumber(2, 9); // Pembagi kecil
    int num3 = generateRandomNumber(2, 5); // Pangkat kecil
    
    q.num1 = num1;
    q.num2 = num2;
    q.num3 = num3;
    
    // Pastikan pembagian menghasilkan bilangan bulat
    while (num1 % num2 != 0) {
        num1 = generateRandomNumber(50, 100);
        num2 = generateRandomNumber(2, 9);
    }
    
    int divisionResult = num1 / num2;
    int powerResult = 1;
    for (int i = 0; i < num3; i++) {
        powerResult *= num3;
    }
    
    q.correctAnswer = divisionResult * powerResult;
    sprintf(q.questionText, "%d ÷ %d × %d² = ?", num1, num2, num3);
    q.timeLimit = 30;
    return q;
}

// LEVEL 10: Expert
Question generateLevel10() {
    Question q;
    int num1 = generateRandomNumber(100, 200);
    int num2 = generateRandomNumber(2, 5); // Pangkat 3 kecil
    
    // Pastikan num1 adalah bilangan kuadrat sempurna
    int root = (int)sqrt(num1);
    while (root * root != num1) {
        num1 = generateRandomNumber(100, 200);
        root = (int)sqrt(num1);
    }
    
    q.num1 = num1;
    q.num2 = num2;
    
    int sqrtResult = (int)sqrt(num1);
    int cubeResult = num2 * num2 * num2;
    
    q.correctAnswer = sqrtResult + cubeResult;
    sprintf(q.questionText, "√%d + %d³ = ?", num1, num2);
    q.timeLimit = 32;
    return q;
}

// LEVEL 11: Master
Question generateLevel11() {
    Question q;
    int num1 = generateRandomNumber(2, 5); // Basis pangkat 3
    int num2 = generateRandomNumber(2, 6); // Pengali
    int num3 = generateRandomNumber(1, 100); // Untuk akar
    
    // Pastikan num3 adalah bilangan kuadrat sempurna
    int root = (int)sqrt(num3);
    while (root * root != num3) {
        num3 = generateRandomNumber(1, 100);
        root = (int)sqrt(num3);
    }
    
    q.num1 = num1;
    q.num2 = num2;
    q.num3 = num3;
    
    int cubeResult = num1 * num1 * num1;
    int multiplicationResult = cubeResult * num2;
    int sqrtResult = (int)sqrt(num3);
    
    q.correctAnswer = multiplicationResult - sqrtResult;
    sprintf(q.questionText, "%d³ × %d - √%d = ?", num1, num2, num3);
    q.timeLimit = 35;
    return q;
}

// LEVEL 12: Boss Ekstrem
Question generateLevel12() {
    Question q;
    int num1 = generateRandomNumber(100, 300);
    int num2 = generateRandomNumber(10, 20);
    int num3 = generateRandomNumber(2, 5);
    int num4 = generateRandomNumber(1, 100);
    
    // Pastikan num1 habis dibagi num2
    while (num1 % num2 != 0) {
        num1 = generateRandomNumber(100, 300);
        num2 = generateRandomNumber(10, 20);
    }
    
    // Pastikan num4 adalah bilangan kuadrat sempurna
    int root = (int)sqrt(num4);
    while (root * root != num4) {
        num4 = generateRandomNumber(1, 100);
        root = (int)sqrt(num4);
    }
    
    q.num1 = num1;
    q.num2 = num2;
    q.num3 = num3;
    q.num4 = num4;
    
    int divisionResult = num1 / num2;
    int powerResult = num3 * num3;
    int sum = divisionResult + powerResult;
    int sqrtResult = (int)sqrt(num4);
    
    q.correctAnswer = sum * sqrtResult;
    sprintf(q.questionText, "(%d ÷ %d + %d²) × √%d = ?", num1, num2, num3, num4);
    q.timeLimit = 45;
    return q;
}

// Fungsi utama untuk mendapatkan soal berdasarkan level
Question generateQuestion(int level) {
    srand(time(NULL)); // Inisialisasi seed random
    
    switch(level) {
        case 1: return generateLevel1();
        case 2: return generateLevel2();
        case 3: return generateLevel3();
        case 4: return generateLevel4();
        case 5: return generateLevel5();
        case 6: return generateLevel6();
        case 7: return generateLevel7();
        case 8: return generateLevel8();
        case 9: return generateLevel9();
        case 10: return generateLevel10();
        case 11: return generateLevel11();
        case 12: return generateLevel12();
        default: return generateLevel1(); // Default ke level 1
    }
}