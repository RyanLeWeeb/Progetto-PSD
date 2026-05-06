#ifndef RICHIESTA_H
#define RICHIESTA_H
#include "utility.h"

typedef struct {
    int codice;
    char luogo[51];
    short tipologia;
    char descrizione[101];
    Data data;
    char ore[24];
    short urgenza;
    short stato; 
    // 0. aperta
    // 1. pianificata
    // 2. in lavorazione
    // 3. conclusa
    // 4. annullata
    int id_tecnico;
    Richiesta *next;
} Richiesta;

Richiesta *creaListaRichiesta();

void aggiornaListaRichiesta(Richiesta *listaRichiesta);

void aggiungiRichiesta(Richiesta *listaRichiesta);

#endif