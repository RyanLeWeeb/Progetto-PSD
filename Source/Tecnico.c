#include <stdio.h>
#include <stdlib.h>
#include "tecnico.h"

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

    printf("Inserisci il nome del tecnico: ");
    scanf("%50s", newNode->nome);
    printf("Inserisci l'ID del tecnico: ");
    scanf("%d", &newNode->id);
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
}