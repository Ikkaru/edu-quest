#ifndef LOGIKA_H
#define LOGIKA_H

#include <stdio.h>
#include <stdbool.h>

#define MAKS_SOAL 200
#define MAKS_TEKST 256

typedef struct {
    int nomor;                 // nomor soal otomatis
    char pertanyaan[MAKS_TEKST];
    char pilihanA[MAKS_TEKST];
    char pilihanB[MAKS_TEKST];
    char pilihanC[MAKS_TEKST];
} Soal;

typedef struct {
    int nomor;                 // nomor soal
    char jawaban;              // 'a','b','c'
} KunciJawaban;

// Load soal dari quest.txt
int muatSoal(const char *namaFile, Soal *daftarSoal, int maksSoal);

// Load jawaban dari answ.txt
int muatJawaban(const char *namaFile, KunciJawaban *daftarJawaban, int maksJawaban);

// Ambil soal acak dari bank soal
int ambilSoalAcak(const Soal *daftarSoal, int jumlahSoal,
                  const KunciJawaban *daftarJawaban, int jumlahJawaban,
                  Soal *soalAcak, char *jawabanBenar);

// Cek jawaban user → return string "BENAR" atau "SALAH"
const bool cekJawaban(char jawabanUser, char jawabanBenar);

#endif
