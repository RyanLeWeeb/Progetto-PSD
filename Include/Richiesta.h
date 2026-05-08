#ifndef RICHIESTA_H
#define RICHIESTA_H
#include "utility.h"

typedef struct {
    int codice;
    char luogo[51];
    short tipologia;
    // 1. Hardware
    // 2. Software
    // 3. Reti
    // 4. Sicurezza
    // 5. Altro
    char descrizione[101];
    Data data;
    char ore[24];
    short urgenza;
    // 1. Bassa
    // 2. Media
    // 3. Alta
    short stato; 
    // 0. aperta
    // 1. pianificata
    // 2. in lavorazione
    // 3. conclusa
    // 4. annullata
    int id_tecnico;
    Richiesta *next;
} Richiesta;

// Crea la lista delle richieste leggendo dal file "Richiesta.txt"
Richiesta *creaListaRichiesta();

// Aggiorna il file con la lista delle richieste
void aggiornaListaRichiesta(Richiesta *listaRichiesta);

// Aggiunge una nuova richiesta alla lista
void aggiungiRichiesta(Richiesta *listaRichiesta);

// Aggiorna lo stato di una richiesta in base al codice
void aggiornaStatoRichiesta(Richiesta *listaRichiesta, int codice, short nuovoStato);

// Visualizza le richieste filtrate per stato
void visualizzazioneRichiestePerStato(Richiesta *listaRichiesta, short stato);

// Visualizza le richieste filtrate per urgenza
void visualizzazioneRichiestePerUrgenza(Richiesta *listaRichiesta, short urgenza);

// Visualizza le richieste filtrate per tipologia
void visualizzazioneRichiestePerTipologia(Richiesta *listaRichiesta, short tipologia);

// Visualizza le richieste filtrate per luogo
void visualizzazioneRichiestePerLuogo(Richiesta *listaRichiesta, char luogo[51]);

// Visualizza le richieste filtrate per tecnico assegnato
void visualizzazioneRichiestePerTecnico(Richiesta *listaRichiesta, int id_tecnico);

#endif