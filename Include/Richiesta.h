#ifndef RICHIESTA_H
#define RICHIESTA_H

#include "Utility.h"

typedef struct Tecnico Tecnico;

typedef struct Richiesta {
    int codice;
    char luogo[51];
    short tipologia; // SE MODIFICATO: modificare NUM_SPECIALIZZAZIONI in Tecnico.h
    // 1. Hardware
    // 2. Software
    // 3. Reti
    // 4. Sicurezza
    // 5. Altro
    char descrizione[101];
    Data data;
    int oraInizio;
    int oraFine;
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
    struct Richiesta *next;
} Richiesta;

// Crea la lista delle richieste leggendo dal file "Richiesta.txt"
Richiesta *creaListaRichiesta();

// Crea una lista di richieste vuota (utilizzata per i test)
Richiesta *creaListaRichiestaVuota();

// Aggiorna il file con la lista delle richieste
void aggiornaListaRichiesta(Richiesta *listaRichiesta);

// Aggiunge una nuova richiesta alla lista
Richiesta *aggiungiRichiesta(Richiesta *listaRichiesta, const char *luogo, short tipologia, short urgenza, const char *descrizione);

void rimuoviRichiesta(Richiesta **listaRichiesta, int codice);

// Aggiorna lo stato di una richiesta in base al codice
int aggiornaStatoRichiesta(Richiesta *listaRichiesta, int codice, Tecnico *listaTecnici, short nuovoStato, int id_tecnico, int oraInizio, int oraFine, Data data);

// Ricerca le richieste in base a diversi criteri (1. Tipologia, 2. Codice Richiesta)
void ricercaRichieste(Richiesta *listaRichiesta, short criterio, short tipologia, int codice);

// Genera un report con statistiche sugli interventi
// DA RICONTROLLARE
void generaReport(Richiesta *listaRichiesta, Tecnico *listaTecnico);

// Visualizza le richieste in base a diversi criteri (1. Tipologia, 2. Stato, 3. Urgenza, 4. Luogo, 5. Tecnico)
void visualizzazioneRichieste(Richiesta *lista, short criterio, short tipologia, short stato, short urgenza, const char *luogo, int id_tecnico);

// Dealloca la memoria della lista delle richieste
void deallocaListaRichieste(Richiesta *lista);

void visualizzaStoricoInterventiCompletati(Richiesta *listaRichiesta);

#endif