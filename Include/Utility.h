#ifndef UTILITY_H
#define UTILITY_H
#include <stdbool.h>

typedef struct {
    int giorno;
    int mese;
    int anno;
} Data;

short confrontaDate(Data d1, Data d2);
//    - Pre: nessuna
//
//    - Post: la funzione restituisce 0 se le date sono uguali, 1 altrimenti

short sonoOrariSovrapposti(int inizio1, int fine1, int inizio2, int fine2);
//    - Pre: nessuna
//
//    - Post: la funzione restituisce 1 se gli orari si sovrappongono, 0 altrimenti


#endif