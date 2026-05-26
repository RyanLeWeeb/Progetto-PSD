#ifndef TECNICO_H
#define TECNICO_H
#include "Utility.h"

typedef struct Richiesta Richiesta;

#define NUM_SPECIALIZZAZIONI 5
#define TECNICOFILE "Liste/Tecnico.txt"

typedef struct Tecnico {
    int id;
    char nome[51];
    int specializzazione;
    // 1. Hardware
    // 2. Software
    // 3. Reti
    // 4. Sicurezza
    // 5. Altro
    struct Tecnico *next;
} Tecnico;

// Crea una lista di tecnici leggendo i dati da un file
Tecnico* creaListaTecnico();
//    - Pre: nel caso in cui il file non sia vuoto, questo deve rispettare la seguente formattazione: "%50[^-]-%d-%d-%d".
//            Di cui l'ordine delle variabili (della struct Tecnico) corrisponde a: 
//            nome, id, specializzazione, num_ore_lavorate
//
//    - Post: r è la lista di tecnici creata leggendo il file "Tecnico.txt",
//            se il file è invece vuoto, r sarà una lista vuota anch'essa

// Crea una lista di tecnici vuota (utilizzata per i test)
Tecnico *creaListaTecnicoTest();

// Aggiorna il file con i dati della lista di tecnici
void aggiornaListaTecnico(Tecnico *listaTecnico, char *filename);
//    - Pre: "listaTecnico" è una lista di richieste in cui è presente almeno un tecnico.
//           "filename" è la stringa corrispondente al documento di testo dove vengono memorizzati i tecnici
//
//    - Post: come side-effect, il file di testo "Tecnico.txt" viene ri-scritto con tutti i tecnici presenti nella lista data.

// Aggiunge un nuovo tecnico alla lista
Tecnico *aggiungiTecnico(Tecnico *listaTecnico, char *filename, const char *nome, int specializzazione);
//    - Pre: "filename" è la stringa corrispondente al documento di testo dove vengono memorizzati i tecnici
//    - Post: r è la lista di richieste che conterrà le richieste di "listaRichiesta" e la richiesta aggiunta.

// Rimuove un tecnico specifica dalla lista di tecnici
void rimuoviTecnico(Tecnico **listaTecnico, char *filename, int id);
//    - Pre: "listaTecnico" deve contenere ALMENO una tecnico;
//           "id" deve essere il id di una delle richieste presenti
//           "filename" deve corrispondere al documento di testo in cui i tecnici di "listaTecnico" vengono memorizzati
//
//    - Post: come side-effect "listaTecnico" conterrà tutti i tecnici che aveva precedentemente,
//            a meno del tecnico con l'id stato dato in input alla funzione

// Calcola le ore lavorate da un tecnico sommando le ore delle richieste a lui assegnate
int orelavorate(Richiesta *listaRichieste, Tecnico *tecnico);
//    - Pre: "listaRichiesta" deve contenere almeno una richiesta assegnata al tecnico dato
//    - Post: restituisce il numero di ore lavorate di quel tecnico su tutte le richieste completate

// Stampa le ore lavorate da ogni tecnico
void stampaOreLavorateTecnici(Tecnico *listaTecnico, Richiesta *listaRichieste);
//    - Pre: "listaTecnico" deve contenere almeno un tecnico
//           "listaRichieste" deve contenere almeno una richiesta di almeno un tecnico presente in "listaTecnico"
//    - Post: stampa a schermo il numero di ore lavorate di ogni tecnico trovato in entrambe le liste

// Dealloca la memoria della lista dei tecnici
void deallocaListaTecnici(Tecnico *lista);
//    - Pre: NESSUNA
//    - Post: la variabile "lista" viene deallocata dalla memoria
//    
//    NOTA: questo NON cancella il file .txt dove i tecnici di quella lista sono memorizzati

#endif