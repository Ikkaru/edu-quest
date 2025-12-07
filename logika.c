#include "logika.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void trim_newline(char *s) {
    size_t n = strlen(s);
    if (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r')) s[n-1] = '\0';
}

// Format quest.txt per blok:
int load_questions(const char *questFile, Question *questions, int maxQuestions) {
    FILE *f = fopen(questFile, "r");
    if (!f) {
        fprintf(stderr, "Tidak bisa membuka %s\n", questFile);
        return 0;
    }

    char line[1024];
    int count = 0;
    Question q;
    int inBlock = 0;

    while (fgets(line, sizeof(line), f)) {
        trim_newline(line);
        if (strncmp(line, "LEVEL:", 6) == 0) {
            if (count >= maxQuestions) break;
            memset(&q, 0, sizeof(q));
            inBlock = 1;
            q.level = atoi(line + 6);
        } else if (strncmp(line, "QID:", 4) == 0 && inBlock) {
            q.qid = atoi(line + 4);
        } else if (strncmp(line, "QUESTION:", 9) == 0 && inBlock) {
            strncpy(q.question, line + 9, MAX_TEXT - 1);
        } else if (strncmp(line, "A:", 2) == 0 && inBlock) {
            strncpy(q.choiceA, line + 2, MAX_TEXT - 1);
        } else if (strncmp(line, "B:", 2) == 0 && inBlock) {
            strncpy(q.choiceB, line + 2, MAX_TEXT - 1);
        } else if (strncmp(line, "C:", 2) == 0 && inBlock) {
            strncpy(q.choiceC, line + 2, MAX_TEXT - 1);
        } else if (strcmp(line, "---") == 0 && inBlock) {
            // akhir blok
            questions[count++] = q;
            inBlock = 0;
        }
    }

    fclose(f);
    return count;
}

// Format answ.txt per baris:
// LEVEL:<int> QID:<int> ANSWER:<a|b|c>
int load_answers(const char *answFile, Answer *answers, int maxAnswers) {
    FILE *f = fopen(answFile, "r");
    if (!f) {
        fprintf(stderr, "Tidak bisa membuka %s\n", answFile);
        return 0;
    }

    char line[512];
    int count = 0;

    while (fgets(line, sizeof(line), f)) {
        trim_newline(line);
        if (strlen(line) == 0) continue;

        int level = 0, qid = 0;
        char ans = 0;

        // parsing sederhana
        // contoh: LEVEL:1 QID:1 ANSWER:a
        char *pLevel = strstr(line, "LEVEL:");
        char *pQid   = strstr(line, "QID:");
        char *pAns   = strstr(line, "ANSWER:");

        if (pLevel && pQid && pAns) {
            level = atoi(pLevel + 6);
            qid   = atoi(pQid + 4);
            ans   = *(pAns + 7);
            if (count < maxAnswers) {
                answers[count].level  = level;
                answers[count].qid    = qid;
                answers[count].answer = ans;
                count++;
            }
        }
    }

    fclose(f);
    return count;
}

// Ambil 1 soal acak dari dua soal dalam level
int battle(int level,
           const Question *questions, int qCount,
           const Answer *answers, int aCount,
           Question *outQ, char *outCorrect) {

    // Kumpulkan dua soal untuk level ini
    Question levelQuestions[2];
    int found = 0;

    for (int i = 0; i < qCount; i++) {
        if (questions[i].level == level) {
            if (found < 2) {
                levelQuestions[found++] = questions[i];
            }
        }
    }

    if (found < 2) {
        fprintf(stderr, "Soal untuk level %d tidak lengkap.\n", level);
        return 0;
    }

    // Random pilih 1 dari 2
    srand((unsigned int)time(NULL));
    int idx = rand() % 2;
    *outQ = levelQuestions[idx];

    // Cari jawaban yang cocok
    char correct = 0;
    for (int j = 0; j < aCount; j++) {
        if (answers[j].level == outQ->level && answers[j].qid == outQ->qid) {
            correct = answers[j].answer;
            break;
        }
    }

    if (!correct) {
        fprintf(stderr, "Jawaban untuk level %d QID %d tidak ditemukan.\n", outQ->level, outQ->qid);
        return 0;
    }

    *outCorrect = correct;
    return 1;
}

int check_answer(char userAnswer, char correctAnswer, int *score, int *lives) {
    // normalisasi ke huruf kecil
    if (userAnswer >= 'A' && userAnswer <= 'C') {
        userAnswer = (char)(userAnswer - 'A' + 'a');
    }

    if (userAnswer == correctAnswer) {
        *score += 100;
        return 1;
    } else {
        *lives -= 1;
        return 0;
    }
}
