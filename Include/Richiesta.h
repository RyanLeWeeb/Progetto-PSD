#ifndef RICHIESTA_H
#define RICHIESTA_H
#include "utility.h"

typedef struct {
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
    Richiesta *next;
} Richiesta;

// Crea la lista delle richieste leggendo dal file "Richiesta.txt"
Richiesta *creaListaRichiesta();

// Aggiorna il file con la lista delle richieste
void aggiornaListaRichiesta(Richiesta *listaRichiesta);

// Aggiunge una nuova richiesta alla lista
void aggiungiRichiesta(Richiesta *listaRichiesta);

// Aggiorna lo stato di una richiesta in base al codice
void aggiornaStatoRichiesta(Richiesta *listaRichiesta, int codice, Tecnico *listaTecnici);

// Ricerca le richieste in base a diversi criteri (1. Tipologia, 2. Codice Richiesta)
void ricercaRichieste(Richiesta *listaRichiesta);

// Genera un report con statistiche sugli interventi
// DA RICONTROLLARE
void generaReport(Richiesta *listaRichiesta, Tecnico *listaTecnico);

// Visualizza le richieste in base a diversi criteri (1. Tipologia, 2. Stato, 3. Urgenza, 4. Luogo, 5. Tecnico)
void visualizzazioneRichieste(Richiesta *lista);

#endif