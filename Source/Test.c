#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Include/Test.h"
#include "../Include/Richiesta.h"
#include "../Include/Tecnico.h"
#include "../Include/Utility.h"

void test_verifica_registrazione_richiesta(Richiesta *lista) {
    printf("\nTest di verifica registrazione richiesta...\n");
    lista = aggiungiRichiesta(lista, "Ufficio 1", 1, 2, "Problema con il computer");
    
    Richiesta *current = lista;
    int found = 0;
    int codice_da_rimuovere = -1; // Variabile di supporto sicura

    while (current != NULL) {
        if (strcmp(current->luogo, "Ufficio 1") == 0 && 
            current->tipologia == 1 && current->urgenza == 2 && 
            strcmp(current->descrizione, "Problema con il computer") == 0 &&
            current->stato == 0 &&
            current->id_tecnico == 0 &&
            current->data.giorno == 0 && current->data.mese == 0 && current->data.anno == 0 &&
            current->oraInizio == 0 && current->oraFine == 0) {
            
            found = 1;
            codice_da_rimuovere = current->codice; // Codice viene salvato
            break; // Uscita dal ciclo una volta trovata la richiesta
        }
        current = current->next;
    }

    if (found) {
        printf("Test superato: richiesta registrata correttamente\n");
        // Rimuoviamo la richiesta usando il codice salvato
        rimuoviRichiesta(&lista, codice_da_rimuovere); 
    } else {
        printf("Test fallito: richiesta non trovata o dati non corretti\n");
        // Se non è stato trovato nulla, NON chiamiamo la rimozione per evitare crash,
        // ma se aggiungiRichiesta aveva comunque inserito qualcosa a inizio lista, 
        // andrebbe rimosso usando il codice del primo elemento (lista->codice).
        if (lista != NULL) {
            rimuoviRichiesta(&lista, lista->codice);
        }
    }
}

void test_verifica_registrazione_tecnico(Tecnico *lista){
    printf("\nTest di verifica registrazione tecnico...\n");
    lista = aggiungiTecnico(lista, "Mario Rossi", 2);
    
    Tecnico *current = lista;
    int found = 0;
    int id_da_rimuovere = -1; // Variabile di supporto per evitare di usare current dopo il ciclo

    while (current != NULL) {
        if (strcmp(current->nome, "Mario Rossi") == 0 && 
            current->specializzazione == 2 && 
            current->num_ore_lavorate == 0) {
            
            found = 1;
            id_da_rimuovere = current->id; // L'ID viene salvato
            break; // Uscita dal ciclo una volta trovato il tecnico
        }
        current = current->next;
    }

    if (found) {
        printf("Test superato: tecnico registrato correttamente\n");
        // Rimuove il tecnico di test usando l'ID salvato
        rimuoviTecnico(&lista, id_da_rimuovere); 
    } else {
        printf("Test fallito: tecnico non trovato o dati non corretti\n");
    }
}
