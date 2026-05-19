#include <stdio.h>
#include <stdlib.h>
#include "../Include/Tecnico.h"
#include "../Include/Richiesta.h"

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

        // Parse the line: nome-ID-specializzazione-num_ore_lavorate
        if (sscanf(line, "%50[^-]-%d-%d-%d", newNode->nome, &newNode->id, &newNode->specializzazione, &newNode->num_ore_lavorate) == 4) {
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
        fprintf(fp, "%s-%d-%d-%d\n", current->nome, current->id, current->specializzazione, current->num_ore_lavorate);
        current = current->next;
    }

    fclose(fp);
}

Tecnico *aggiungiTecnico(Tecnico *listaTecnico, const char *nome, int specializzazione){
    Tecnico *newNode = malloc(sizeof(Tecnico));
    if (newNode == NULL) {
        printf("Errore: Memoria non allocabile\n");
        exit(1);
    }
    
    int id = 100000 + rand() % 900000; 

    Tecnico *current = listaTecnico;
    while (current != NULL) {
        if (current->id == id) {
            id = 100000 + rand() % 900000;
            current = listaTecnico;
        } else {
            current = current->next;
        }
    }
    newNode->id = id;
    strncpy(newNode->nome, nome, sizeof(newNode->nome) - 1);
    newNode->nome[sizeof(newNode->nome) - 1] = '\0';
    newNode->specializzazione = specializzazione;
    newNode->num_ore_lavorate = 0;
    newNode->next = NULL;

    if (listaTecnico == NULL) {
        listaTecnico = newNode;
    } else {
        Tecnico *tail = listaTecnico;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = newNode;
    }
    aggiornaListaTecnico(listaTecnico);
    return listaTecnico;
}

void rimuoviTecnico(Tecnico **listaTecnico, int id){
    if (*listaTecnico == NULL) {
        printf("La lista dei tecnici è vuota.\n");
        return;
    }

    Tecnico *current = *listaTecnico;
    Tecnico *previous = NULL;

    while (current != NULL) {
        if (current->id == id) {
            if (previous == NULL) {
                *listaTecnico = current->next; 
            } else {
                previous->next = current->next; 
            }
            
            aggiornaListaTecnico(*listaTecnico);
            
            free(current); 
            
            printf("Tecnico con ID %d rimosso.\n", id);
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("Tecnico con ID %d non trovato.\n", id);
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

void deallocaListaTecnici(Tecnico *lista) {
    Tecnico *current = lista;
    Tecnico *next_node;

    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
}