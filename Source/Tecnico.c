#include <stdio.h>
#include <stdlib.h>
#include "tecnico.h"
#include "Richiesta.h"

Tecnico* creaListaTecnico(){
    FILE *fp = fopen("Liste/Tecnico.txt", "r");
    if (fp == NULL) {
        printf("Errore: Impossibile aprire il file Tecnico.txt\n");
        exit(1);
    }

    Tecnico *head = NULL;
    Tecnico *tail = NULL;
    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        Tecnico *newNode = malloc(sizeof(Tecnico));
        if (newNode == NULL) {
            printf("Errore: Memoria non allocabile\n");
            fclose(fp);
            exit(1);
        }

        // Parse the line: nome-ID-specializzazione
        if (sscanf(line, "%50[^-]-%d-%d", newNode->nome, &newNode->id, &newNode->specializzazione) == 3) {
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

void aggiornaListaTecnico(Tecnico *listaTecnico) {
    FILE *fp = fopen("Liste/Tecnico.txt", "w");
    if (fp == NULL) {
        printf("Errore: Impossibile aprire il file Tecnico.txt\n");
        exit(1);
    }

    Tecnico *current = listaTecnico;
    while (current != NULL) {
        fprintf(fp, "%s-%d-%d\n", current->nome, current->id, current->specializzazione);
        current = current->next;
    }

    free(current);
    fclose(fp);
}

void aggiungiTecnico(Tecnico *listaTecnico){
    Tecnico *newNode = malloc(sizeof(Tecnico));
    if (newNode == NULL) {
        printf("Errore: Memoria non allocabile\n");
        exit(1);
    }
    
    // srand(time(NULL)); // Inizializza il generatore di numeri casuali con il tempo attuale
    int id = 100000 + rand() % 900000; // Genera un numero casuale tra 100000 e 999999

    Tecnico *current = listaTecnico;
    while (current != NULL) {
        if (current->id == id) {
            // Se l'ID generato esiste già, genera un nuovo ID
            id = 100000 + rand() % 900000;
            current = listaTecnico; // Ricomincia a controllare dall'inizio
        } else {
            current = current->next;
        }
    }
    newNode->id = id; // Assegna l'ID univoco al nuovo tecnico

    printf("Inserisci il nome del tecnico: ");
    scanf("%50s", newNode->nome);
    printf("Inserisci la specializzazione del tecnico:\n1. Hardware\n2. Software\n3. Reti\n4. Sicurezza\n5. Altro\n");
    scanf("%d", &newNode->specializzazione);
    newNode->next = NULL;

    if (listaTecnico == NULL) {
        listaTecnico = newNode;
    } else {
        Tecnico *current = listaTecnico;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    aggiornaListaTecnico(listaTecnico);
}

int orelavorate(Richiesta *listaRichieste, Tecnico *tecnico) {
    Richiesta *current = listaRichieste;
    int oreTotali = 0;

    while (current != NULL) {
        if (current->id_tecnico == tecnico->id) {
            if (current->stato == 3) { // Considera solo le richieste concluse
                int durata = current->oraFine - current->oraInizio;
                if (durata > 0) {
                    oreTotali += durata; // Somma le ore lavorate per il tecnico
                }
            }
        }
        current = current->next;
    }

    return oreTotali;
}

void stampaOreLavorateTecnici(Tecnico *listaTecnico, Richiesta *listaRichieste){
    Tecnico *currentTecnico = listaTecnico;
    
    printf("Ore lavorate da ogni tecnico:\n");
    while (currentTecnico != NULL) {
        int ore = orelavorate(listaRichieste, currentTecnico);
        printf("Tecnico ID: %d, Nome: %s, Ore lavorate: %d\n", currentTecnico->id, currentTecnico->nome, ore);
        currentTecnico = currentTecnico->next;
    }
}