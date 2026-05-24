#ifndef RICHIESTA_H
#define RICHIESTA_H

#include "Utility.h"

#define RICHIESTAFILE "Liste/Richiesta.txt"

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
//    - Pre: nel caso in cui il file non sia vuoto, questo deve rispettare la seguente formattazione: "%d-%50[^-]-%hd-%100[^-]-%d/%d/%d-%d-%d-%hd-%hd-%d".
//            Di cui l'ordine delle variabili (della struct Richiesta) corrisponde a: 
//            codice, luogo, tipologia, descrizione, data.giorno, data.mese, data.anno, oraInizio, oraFine, urgenza, stato, id_tecnico
//
//    - Post: r è la lista di richieste creata leggendo il file "Richiesta.txt",
//            se il file è invece vuoto, r sarà una lista vuota anch'essa

// Crea una lista di richieste vuota (utilizzata per i test)
Richiesta *creaListaRichiestaTest();

// Aggiorna il file con la lista delle richieste
void aggiornaListaRichiesta(Richiesta *lista, char *filename);
//    - Pre: "lista" è una lista di richieste in cui è presente almeno una richiesta.
//           "filename" è la stringa corrispondente al documento di testo dove vengono memorizzate le richieste
//
//    - Post: come side-effect, il file di testo "Richiesta.txt" viene ri-scritto con tutte le richieste presenti nella lista data.

// Aggiunge una nuova richiesta alla lista
Richiesta *aggiungiRichiesta(Richiesta *listaRichiesta, char *filename, const char *luogo, short tipologia, short urgenza, const char *descrizione);
//    - Pre: "filename" è la stringa corrispondente al documento di testo dove vengono memorizzate le richieste
//    - Post: r è la lista di richieste che conterrà le richieste di "listaRichiesta" e la richiesta aggiunta.

// Rimuove una richiesta specifica dalla lista di richieste
void rimuoviRichiesta(Richiesta **listaRichiesta, char *filename, int codice);
//    - Pre: "listaRichiesta" deve contenere ALMENO una richiesta;
//           "codice" deve essere il codice di una delle richieste presenti
//           "filename" deve corrispondere al documento di testo in cui le richieste di "listaRichiesta" vengono memorizzate
//
//    - Post: come side-effect "listaRichiesta" conterrà tutte le richieste che aveva precedentemente,
//            a meno della richiesta col codice stato dato in input alla funzione

// Aggiorna lo stato di una richiesta in base al codice
int aggiornaStatoRichiesta(Richiesta *listaRichiesta, char *filename, int codice, Tecnico *listaTecnici, short nuovoStato, int id_tecnico, int oraInizio, int oraFine, Data data);
//    - Pre: "listaRichiesta" deve contenere almeno una richiesta
//           "codice" deve corrispondere al codice di una delle richieste presenti nella lista
//           "filename" deve corrispondere al documento di testo in cui le richieste di "listaRichiesta" vengono memorizzate

// Ricerca le richieste in base a diversi criteri (1. Tipologia, 2. Codice Richiesta)
void ricercaRichieste(Richiesta *listaRichiesta, short criterio, short tipologia, int codice);
//    - Pre: "listaRichiesta" deve contenere almeno una richiesta della tipologia data (se "criterio" è 1)
//            OPPURE "codice" deve essere il codice di una tra le richieste nella lista (se "criterio" è 2)
//
//    - Post: Due casi di side-effect, in base al criterio:
//            se criterio è 1: stampa su schermo tutte le richieste della tipologia data (se ce ne sono)
//            se criterio è 2: stampa su schermo la richiesta col codice dato

// Genera un report con statistiche sugli interventi
// DA RICONTROLLARE
void generaReport(Richiesta *listaRichiesta, Tecnico *listaTecnico);

// Visualizza le richieste in base a diversi criteri (1. Tipologia, 2. Stato, 3. Urgenza, 4. Luogo, 5. Tecnico)
void visualizzazioneRichieste(Richiesta *lista, short criterio, short tipologia, short stato, short urgenza, const char *luogo, int id_tecnico);
//    - Pre: "lista" deve contenere almeno una richiesta 
//    - Post: tutte le richieste che corrispondo al criterio di ricerca, verrano stampate allo schermo
//            (per cercare, ad esempio, secondo lo stato, tutte le altre variabili (eccetto lista) 
//            verranno settate a zero (eccetto luogo, che verrebbe settato a NULL))


// Dealloca la memoria della lista delle richieste
void deallocaListaRichieste(Richiesta *lista);
//    - Pre: NESSUNA
//    - Post: la variabile "lista" viene deallocata dalla memoria
//    
//    NOTA: questo NON cancella il file .txt dove le richieste di quella lista erano memorizzate

void visualizzaStoricoInterventiCompletati(Richiesta *listaRichiesta);
//    - Pre: NESSUNA
//    - Post: stampa a schermo le informazioni di tutte le richieste segnate come "completate" nella "listaRichiesta",
//            se nessuna richiesta è completata, allora verrà stampato a schermo un messaggio che lo chiarirà

#endif