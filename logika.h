#ifndef LOGIKA_H
#define LOGIKA_H

#include <stdio.h>

#define MAX_QUESTIONS 24
#define MAX_TEXT 256

typedef struct {
    int level;          // 1..12
    int qid;            // 1..2 (nomor soal di level)
    char question[MAX_TEXT];
    char choiceA[MAX_TEXT];
    char choiceB[MAX_TEXT];
    char choiceC[MAX_TEXT];
} Question;

typedef struct {
    int level;          // 1..12
    int qid;            // 1..2
    char answer;        // 'a', 'b', atau 'c'
} Answer;

/*
Flowchart SOAL LOGIKA:
battle > load file soal dan jawaban > ambil 1 nomor soal acak dari 2 nomor soal dalam 1 level
> ambil jawaban dari File jawaban > return soal dan jawaban

Flowchart CEK JAWABAN:
bandingkan jawaban > jika benar, score += 100
bandingkan jawaban > jika salah, nyawwa -1
*/

// Load pertanyaan dari file quest.txt
int load_questions(const char *questFile, Question *questions, int maxQuestions);

// Load jawaban dari file answ.txt
int load_answers(const char *answFile, Answer *answers, int maxAnswers);

// Ambil 1 soal acak untuk satu level dan kembalikan soal + jawaban benar
// Return 1 jika berhasil, 0 jika gagal
int battle(int level,
           const Question *questions, int qCount,
           const Answer *answers, int aCount,
           Question *outQ, char *outCorrect);

// Cek jawaban pemain dan update score / nyawa
// Return 1 jika benar, 0 jika salah
int check_answer(char userAnswer, char correctAnswer, int *score, int *lives);

#endif // LOGIKA_H
