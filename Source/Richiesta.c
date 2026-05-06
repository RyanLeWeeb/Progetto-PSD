#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Richiesta.h"
#include "Tecnico.h"
#include "Utility.h"

Richiesta *creaListaRichiesta(){
    FILE *fp = fopen("Liste/Richiesta.txt", "r");
    if (fp == NULL) {
        printf("Errore: Impossibile aprire il file Richiesta.txt\n");
        exit(1);
    }

    Richiesta *head = NULL;
    Richiesta *tail = NULL;
    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        Richiesta *newNode = malloc(sizeof(Richiesta));
        if (newNode == NULL) {
            printf("Errore: Memoria non allocabile\n");
            fclose(fp);
            exit(1);
        }

        // Parse the line: codice-luogo-tipologia-descrizione-data-ore-urgenza-stato-id_tecnico
        if (sscanf(line, "%d-%50[^-]-%hd-%100[^-]-%d/%d/%d-%24[^-]-%hd-%hd-%d",
                   &newNode->codice, newNode->luogo, &newNode->tipologia, newNode->descrizione,
                   &newNode->data.giorno, &newNode->data.mese, &newNode->data.anno,
                   newNode->ore, &newNode->urgenza, &newNode->stato, &newNode->id_tecnico) == 10) {
            newNode->next = NULL;
            if (head == NULL) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        } else {
            // Riga non valida, libera il nodo
            free(newNode);
        }
    }

    fclose(fp);
    return head;
}

void aggiornaListaRichiesta(Richiesta *listaRichiesta){
    FILE *fp = fopen("Liste/Richiesta.txt", "w");
    if (fp == NULL) {
        printf("Errore: Impossibile aprire il file Richiesta.txt\n");
        exit(1);
    }

    Richiesta *current = listaRichiesta;
    while (current != NULL) {
        fprintf(fp, "%d-%s-%hd-%s-%02d/%02d/%04d-%s-%hd-%hd-%d\n",
                current->codice, current->luogo, current->tipologia, current->descrizione,
                current->data.giorno, current->data.mese, current->data.anno,
                current->ore, current->urgenza, current->stato, current->id_tecnico);
        current = current->next;
    }

    free(current);
    fclose(fp);
}

// MANUALMENTE: luogo, tipologia, urgenza, descrizione
// Il codice, lo stato, id_tecnico, data, e ore, vengono gestiti automaticamente

// FUNZIONE DA MODIFICARE
void aggiungiRichiesta(Richiesta *listaRichiesta){
    Richiesta *newNode = malloc(sizeof(Richiesta));
    if (newNode == NULL) {
        printf("Errore: Memoria non allocabile\n");
        exit(1);
    }

    // L'utente inserisce i dati della richiesta (luogo, tipologia, urgenza, descrizione)

    printf("Inserisci il luogo dell'intervento: ");
    scanf("%50s", newNode->luogo);

    printf("Inserisci la tipologia dell'intervento (1-5): ");
    scanf("%hd", &newNode->tipologia);

    printf("Inserisci l'urgenza dell'intervento (1-5): "); // NUMERI DA MODIFICARE
    scanf("%hd", &newNode->urgenza);

    printf("Inserisci la descrizione dell'intervento: ");
    getchar();
    fgets(newNode->descrizione, sizeof(newNode->descrizione), stdin);
    newNode->descrizione[strcspn(newNode->descrizione, "\n")] = '\0';


    if (listaRichiesta == NULL) {
        listaRichiesta = newNode;
    } else {
        Richiesta *current = listaRichiesta;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}