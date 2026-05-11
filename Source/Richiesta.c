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

    printf("Inserisci la tipologia dell'intervento (1-5):\n1. Hardware\n2. Software\n3. Reti\n4. Sicurezza\n5. Altro\n");
    scanf("%hd", &newNode->tipologia);

    printf("Inserisci l'urgenza dell'intervento (1-3):\n1. Bassa\n2. Media\n3. Alta\n");
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

void aggiornaStatoRichiesta(Richiesta *listaRichiesta, int codice, short nuovoStato){
    Richiesta *current = listaRichiesta;
    while (current != NULL) {
        if (current->codice == codice) {
            current->stato = nuovoStato;
            return;
        }
        current = current->next;
    }
    printf("Richiesta con codice %d non trovata\n", codice);
}

void visualizzazioneRichiestePerStato(Richiesta *listaRichiesta, short stato) {
    Richiesta *current = listaRichiesta;
    while (current != NULL) {
        if (current->stato == stato) {
            printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %s, Urgenza: %hd, ID Tecnico: %d\n",
                   current->codice, current->luogo, current->tipologia, current->descrizione,
                   current->data.giorno, current->data.mese, current->data.anno,
                   current->ore, current->urgenza, current->id_tecnico);
        }
        current = current->next;
    }
}

void visualizzazioneRichiestePerUrgenza(Richiesta *listaRichiesta, short urgenza) {
    Richiesta *current = listaRichiesta;
    while (current != NULL) {
        if (current->urgenza == urgenza) {
            printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %s, Stato: %hd, ID Tecnico: %d\n",
                   current->codice, current->luogo, current->tipologia, current->descrizione,
                   current->data.giorno, current->data.mese, current->data.anno,
                   current->ore, current->stato, current->id_tecnico);
        }
        current = current->next;
    }
}

void visualizzazioneRichiestePerTipologia(Richiesta *listaRichiesta, short tipologia) {
    Richiesta *current = listaRichiesta;
    while (current != NULL) {
        if (current->tipologia == tipologia) {
            printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %s, Stato: %hd, ID Tecnico: %d\n",
                   current->codice, current->luogo, current->tipologia, current->descrizione,
                   current->data.giorno, current->data.mese, current->data.anno,
                   current->ore, current->stato, current->id_tecnico);
        }
        current = current->next;
    }
}

void visualizzazioneRichiestePerLuogo(Richiesta *listaRichiesta, char luogo[51]) {
    Richiesta *current = listaRichiesta;
    while (current != NULL) {
        if (strcmp(current->luogo, luogo) == 0) {
            printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %s, Stato: %hd, ID Tecnico: %d\n",
                   current->codice, current->luogo, current->tipologia, current->descrizione,
                   current->data.giorno, current->data.mese, current->data.anno,
                   current->ore, current->stato, current->id_tecnico);
        }
        current = current->next;
    }
}

// Da modificare (?)
void visualizzazioneRichiestePerTecnico(Richiesta *listaRichiesta, int id_tecnico) {
    Richiesta *current = listaRichiesta;
    while (current != NULL) {
        if (current->id_tecnico == id_tecnico) {
            printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %s, Stato: %hd\n",
                   current->codice, current->luogo, current->tipologia, current->descrizione,
                   current->data.giorno, current->data.mese, current->data.anno,
                   current->ore, current->stato);
        }
        current = current->next;
    }

}

void visualizzaStoricoInterventiCompletati(Richiesta *listaRichiesta) {
    printf("\n========== STORICO INTERVENTI COMPLETATI ==========\n");
    
    Richiesta *current = listaRichiesta;
    int count = 0;
    
    while (current != NULL) {
        if (current->stato == 3) { // 3 = conclusa
            count++;
            printf("\n[%d] Codice Richiesta: %d\n", count, current->codice);
            printf("    Luogo: %s\n", current->luogo);
            printf("    Tipologia: ");
            switch(current->tipologia) {
                case 1: printf("Hardware\n"); break;
                case 2: printf("Software\n"); break;
                case 3: printf("Reti\n"); break;
                case 4: printf("Sicurezza\n"); break;
                case 5: printf("Altro\n"); break;
                default: printf("Non specificata\n");
            }
            printf("    Descrizione: %s\n", current->descrizione);
            printf("    Data Richiesta: %02d/%02d/%04d\n", current->data.giorno, current->data.mese, current->data.anno);
            printf("    Fascia Oraria: %s\n", current->ore);
            printf("    Urgenza: ");
            switch(current->urgenza) {
                case 1: printf("Bassa\n"); break;
                case 2: printf("Media\n"); break;
                case 3: printf("Alta\n"); break;
                default: printf("Non specificata\n");
            }
            printf("    ID Tecnico Assegnato: %d\n", current->id_tecnico);
            printf("    ---\n");
        }
        current = current->next;
    }
    
    if (count == 0) {
        printf("Nessun intervento completato al momento.\n");
    } else {
        printf("\nTotale interventi completati: %d\n", count);
    }
    printf("===================================================\n\n");
}

void assegnaTecnicoARichiesta(Richiesta *richiesta, Tecnico *listaTecnici) {
    if (richiesta == NULL || listaTecnici == NULL) {
        printf("Errore: Richiesta o lista tecnici nulla\n");
        return;
    }
    
    Tecnico *tecnicoAssegnato = NULL;
    Tecnico *current = listaTecnici;
    int oreMinimoTrovate = INT_MAX;
    
    // Prima cerca un tecnico con specializzazione corrispondente
    while (current != NULL) {
        if (current->specializzazione == richiesta->tipologia) {
            
            // Seleziona il tecnico con meno ore lavorate (meno carico)
            if (current->num_ore_lavorate < oreMinimoTrovate) {
                tecnicoAssegnato = current;
                oreMinimoTrovate = current->num_ore_lavorate;
            }
        }
        current = current->next;
    }
    
    // Se non trova specializzazione esatta, cerca un tecnico "Altro" (specializzazione 5)
    if (tecnicoAssegnato == NULL) {
        current = listaTecnici;
        oreMinimoTrovate = INT_MAX;
        while (current != NULL) {
            if (current->specializzazione == 5) { // 5 = Altro
                if (current->num_ore_lavorate < oreMinimoTrovate) {
                    tecnicoAssegnato = current;
                    oreMinimoTrovate = current->num_ore_lavorate;
                }
            }
            current = current->next;
        }
    }
    
    // Assegna il tecnico se trovato
    if (tecnicoAssegnato != NULL) {
        richiesta->id_tecnico = tecnicoAssegnato->id;
        printf("Tecnico assegnato: %s (ID: %d, Specializzazione: ", tecnicoAssegnato->nome, tecnicoAssegnato->id);
        switch(tecnicoAssegnato->specializzazione) {
            case 1: printf("Hardware"); break;
            case 2: printf("Software"); break;
            case 3: printf("Reti"); break;
            case 4: printf("Sicurezza"); break;
            case 5: printf("Altro"); break;
            default: printf("Non specificata");
        }
        printf(")\n");
        printf("Richiesta con codice %d assegnata con successo.\n", richiesta->codice);
    } else {
        printf("Errore: Nessun tecnico disponibile per l'assegnazione\n");
        richiesta->id_tecnico = -1; // -1 indica tecnico non assegnato
    }
}