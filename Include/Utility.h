#ifndef UTILITY_H
#define UTILITY_H
#include <stdbool.h>

typedef struct {
    int giorno;
    int mese;
    int anno;
} Data;

short confrontaDate(Data d1, Data d2);

short sonoOrariSovrapposti(int inizio1, int fine1, int inizio2, int fine2);


#endif