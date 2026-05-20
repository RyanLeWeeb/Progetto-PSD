#ifndef TECNICO_H
#define TECNICO_H
#include "Utility.h"

typedef struct Richiesta Richiesta;

#define NUM_SPECIALIZZAZIONI 5

typedef struct Tecnico {
    int id;
    char nome[51];
    int specializzazione;
    // 1. Hardware
    // 2. Software
    // 3. Reti
    // 4. Sicurezza
    // 5. Altro
    int num_ore_lavorate; // Per il Report del tecnico più attivo
    struct Tecnico *next;
} Tecnico;

// Crea una lista di tecnici leggendo i dati da un file
Tecnico* creaListaTecnico();

// Crea una lista di tecnici vuota (utilizzata per i test)
Tecnico *creaListaTecnicoVuota();

// Aggiorna il file con i dati della lista di tecnici
void aggiornaListaTecnico(Tecnico *listaTecnico);

// Aggiunge un nuovo tecnico alla lista
Tecnico *aggiungiTecnico(Tecnico *listaTecnico, const char *nome, int specializzazione);

void rimuoviTecnico(Tecnico **listaTecnico, int id);

// Calcola le ore lavorate da un tecnico sommando le ore delle richieste a lui assegnate
int orelavorate(Richiesta *listaRichieste, Tecnico *tecnico);

// Stampa le ore lavorate da ogni tecnico
void stampaOreLavorateTecnici(Tecnico *listaTecnico, Richiesta *listaRichieste);

// Dealloca la memoria della lista dei tecnici
void deallocaListaTecnici(Tecnico *lista);

#endif