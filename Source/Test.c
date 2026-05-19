#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Include/Test.h"
#include "../Include/Richiesta.h"
#include "../Include/Tecnico.h"
#include "../Include/Utility.h"

void test_verifica_registrazione_richiesta() {
    printf("\nTest di verifica registrazione richiesta...\n");
    
    // 1. Modifichiamo direttamente la testa della lista reale nel main
    Richiesta *lista = creaListaRichiesta();
    lista = aggiungiRichiesta(lista, "Ufficio 1", 1, 2, "Problema con il computer");
    
    // 2. Usiamo un puntatore singolo locale per scorrere la lista in sicurezza
    Richiesta *current = lista;
    int found = 0;
    int codice_da_rimuovere = -1;

    while (current->next != NULL) {
        current = current->next;
    }
    if (strcmp(current->luogo, "Ufficio 1") == 0 && 
        current->tipologia == 1 && current->urgenza == 2 && 
        strcmp(current->descrizione, "Problema con il computer") == 0 &&
        current->stato == 0 &&
        current->id_tecnico == 0 &&
        current->data.giorno == 0 && current->data.mese == 0 && current->data.anno == 0 &&
        current->oraInizio == 0 && current->oraFine == 0) {
            
            found = 1;
            codice_da_rimuovere = current->codice;
    }
    if (found) {
        printf("Test superato: richiesta registrata correttamente\n");
        // 3. Passiamo l'indirizzo del puntatore reale, rimuoviRichiesta riallineerà il main
        rimuoviRichiesta(lista, codice_da_rimuovere); 
    } else {
        printf("Test fallito: richiesta non trovata o dati non corretti\n");
        // Se l'inserimento è avvenuto ma i dati interni erano fallati, ripuliamo la testa reale
        if (lista != NULL) {
            rimuoviRichiesta(lista, lista->codice);
        }
    }
}

void test_verifica_registrazione_tecnico(Tecnico **lista) {
    printf("\nTest di verifica registrazione tecnico...\n");
    
    // 1. Inseriamo il tecnico modificando direttamente la lista reale.
    // Assumiamo che aggiungiTecnico accetti (Tecnico *lista) e restituisca la nuova testa.
    *lista = aggiungiTecnico(*lista, "Mario Rossi", 2);
    
    // 2. Usiamo un puntatore di supporto per scorrere la lista partendo dalla nuova testa
    Tecnico *current = *lista;
    int found = 0;
    int id_da_rimuovere = -1;

    while (current != NULL) {
        if (strcmp(current->nome, "Mario Rossi") == 0 && 
            current->specializzazione == 2) {
            
            found = 1;
            id_da_rimuovere = current->id; 
            break; 
        }
        current = current->next;
    }

    if (found) {
        printf("Test superato: tecnico registrato correttamente\n");
        // 3. Rimuoviamo il tecnico passando l'indirizzo del puntatore reale.
        // rimuoviTecnico aggiornerà la testa nel main se il tecnico era il primo.
        rimuoviTecnico(lista, id_da_rimuovere); 
    } else {
        printf("Test fallito: tecnico non trovato o dati non corretti\n");
        // Pulizia di emergenza se il nodo è stato creato ma non validato dall'if
        if (lista != NULL) {
            rimuoviTecnico(lista, (*lista)->id);
        }
    }
}

void test_verifica_assegnazione_tecnico_e_aggiornamento_stato(Richiesta **listaRichieste, Tecnico **listaTecnici) {
    printf("\nTest di verifica assegnazione tecnico e aggiornamento stato...\n");
    
    // 1. Inseriamo i dati modificando direttamente le liste reali del main tramite deferenziazione (*)
    *listaRichieste = aggiungiRichiesta(*listaRichieste, "Appartamento test1234", 2, 1, "TEST");
    *listaTecnici = aggiungiTecnico(*listaTecnici, "Test Tecnico", 2);
    
    // 2. Troviamo in modo sicuro i nodi appena creati scorrendo le liste
    // (In questo modo funziona sia se inserisci in testa, sia se inserisci in coda)
    Richiesta *reqTest = *listaRichieste;
    int codice_richiesta = -1;
    while (reqTest != NULL) {
        if (strcmp(reqTest->luogo, "Appartamento test1234") == 0 && strcmp(reqTest->descrizione, "TEST") == 0) {
            codice_richiesta = reqTest->codice;
            break;
        }
        reqTest = reqTest->next;
    }

    Tecnico *tecTest = *listaTecnici;
    int id_tecnico = -1;
    while (tecTest != NULL) {
        if (strcmp(tecTest->nome, "Test Tecnico") == 0 && tecTest->specializzazione == 2) {
            id_tecnico = tecTest->id;
            break;
        }
        tecTest = tecTest->next;
    }

    // Controllo di sicurezza: se non abbiamo trovato i nodi appena creati, evitiamo di procedere
    if (codice_richiesta == -1 || id_tecnico == -1) {
        printf("\nTest fallito: Impossibile recuperare i nodi di test creati.\n");
        return;
    }
    
    // 3. Eseguiamo la funzione di aggiornamento sulla lista reale
    // Usiamo reqTest (che punta specificamente al nodo di test trovato prima) per verificare i dati
    if (aggiornaStatoRichiesta(*listaRichieste, codice_richiesta, *listaTecnici, 1, id_tecnico, 9, 17, (Data){1, 1, 2026}) &&
        reqTest->id_tecnico == id_tecnico &&
        reqTest->stato == 1 &&
        reqTest->oraInizio == 9 &&
        reqTest->oraFine == 17 &&
        reqTest->data.giorno == 1 &&
        reqTest->data.mese == 1 &&
        reqTest->data.anno == 2026) {
        
        printf("\nTest superato: tecnico assegnato correttamente alla richiesta\n");
    } else {
        printf("\nTest fallito: impossibile assegnare il tecnico alla richiesta o dati non corrispondenti\n");
    }
    
    // 4. Pulizia: rimuove la richiesta e il tecnico passando i doppi puntatori reali del main
    rimuoviRichiesta(listaRichieste, codice_richiesta);
    rimuoviTecnico(listaTecnici, id_tecnico);
}