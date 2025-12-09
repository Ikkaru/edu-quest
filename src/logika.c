#include "logika.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void hapusNewline(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r')) {
        s[--n] = '\0';
    }
}

int muatSoal(const char *namaFile, Soal *daftarSoal, int maksSoal) {
    FILE *f = fopen(namaFile, "r");
    if (!f) return 0;

    char baris[512];
    int jumlahSoal = 0;
    Soal s;
    int dalamBlok = 0;

    while (fgets(baris, sizeof(baris), f)) {
        hapusNewline(baris);
        if (strncmp(baris, "QUESTION:", 9) == 0) {
            memset(&s, 0, sizeof(s));
            dalamBlok = 1;
            s.nomor = jumlahSoal + 1;
            strncpy(s.pertanyaan, baris+9, MAKS_TEKST-1);
        } else if (strncmp(baris, "A:", 2) == 0) {
            strncpy(s.pilihanA, baris+2, MAKS_TEKST-1);
        } else if (strncmp(baris, "B:", 2) == 0) {
            strncpy(s.pilihanB, baris+2, MAKS_TEKST-1);
        } else if (strncmp(baris, "C:", 2) == 0) {
            strncpy(s.pilihanC, baris+2, MAKS_TEKST-1);
        } else if (strcmp(baris, "---") == 0 && dalamBlok) {
            daftarSoal[jumlahSoal++] = s;
            dalamBlok = 0;
        }
    }
    fclose(f);
    return jumlahSoal;
}

int muatJawaban(const char *namaFile, KunciJawaban *daftarJawaban, int maksJawaban) {
    FILE *f = fopen(namaFile, "r");
    if (!f) return 0;

    char baris[128];
    int jumlahJawaban = 0;
    int id; char ans;
    while (fgets(baris, sizeof(baris), f)) {
        hapusNewline(baris);
        if (sscanf(baris, "ID:%d ANSWER:%c", &id, &ans) == 2) {
            daftarJawaban[jumlahJawaban].nomor = id;
            daftarJawaban[jumlahJawaban].jawaban = ans;
            jumlahJawaban++;
        }
    }
    fclose(f);
    return jumlahJawaban;
}

int ambilSoalAcak(const Soal *daftarSoal, int jumlahSoal,
                  const KunciJawaban *daftarJawaban, int jumlahJawaban,
                  Soal *soalAcak, char *jawabanBenar) {
    if (jumlahSoal == 0 || jumlahJawaban == 0) return 0;
    srand((unsigned int)time(NULL));
    int idx = rand() % jumlahSoal;
    *soalAcak = daftarSoal[idx];

    for (int i=0; i<jumlahJawaban; i++) {
        if (daftarJawaban[i].nomor == soalAcak->nomor) {
            *jawabanBenar = daftarJawaban[i].jawaban;
            return 1;
        }
    }
    return 0;
}

const char* cekJawaban(char jawabanUser, char jawabanBenar) {
    if (jawabanUser >= 'A' && jawabanUser <= 'C') jawabanUser = jawabanUser - 'A' + 'a';
    if (jawabanBenar >= 'A' && jawabanBenar <= 'C') jawabanBenar = jawabanBenar - 'A' + 'a';
    return (jawabanUser == jawabanBenar) ? "BENAR" : "SALAH";
}
