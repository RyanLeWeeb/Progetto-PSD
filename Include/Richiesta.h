#ifndef RICHIESTA_H
#define RICHIESTA_H
#include "utility.h"

typedef struct {
    int codice;
    char luogo[51];
    short tipologia;
    char descrizione[101];
    Data data;
    bool ore[24];
    short urgenza;
    short stato;
} Richiesta;

#endif