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
                   &newNode->oraInizio, &newNode->oraFine, &newNode->urgenza, &newNode->stato, &newNode->id_tecnico) == 10) {
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
                current->oraInizio, current->oraFine, current->urgenza, current->stato, current->id_tecnico);
        current = current->next;
    }

    free(current);
    fclose(fp);
}

// Manca ID_tecnico
void aggiungiRichiesta(Richiesta *listaRichiesta){
    Richiesta *newNode = malloc(sizeof(Richiesta));
    if (newNode == NULL) {
        printf("Errore: Memoria non allocabile\n");
        exit(1);
    }

    newNode->stato = 0; // 0 = aperta

    int codice = 100000 + rand() % 900000;
    Richiesta *current = listaRichiesta;
    while (current != NULL) {
        if (current->codice == codice) {
            // Se il codice generato esiste già, genera un nuovo codice
            codice = 100000 + rand() % 900000;
            current = listaRichiesta; // Ricomincia a controllare dall'inizio
        } else {
            current = current->next;
        }
    }
    newNode->codice = codice;

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

    printf("Inserire data dell'intervento (gg/mm/aaaa): ");
    scanf("%d/%d/%d", &newNode->data.giorno, &newNode->data.mese, &newNode->data.anno);

    // gestione automatica di codice, id_tecnico

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

// 1. Tipologia
// 2. Codice Richiesta
void ricercaRichieste(Richiesta *listaRichiesta) {
    Richiesta *current = listaRichiesta;
    short choice;

    printf("Quale criterio vuoi utilizzare per visualizzare le richieste?\n");
    printf("1. Tipologia\n2. Codice Richiesta\n");
    scanf("%hd", &choice);

    switch (choice){
        case 1:
            while (current != NULL) {

                printf("Quale tipologia vuoi visualizzare?\n(1. Hardware, 2. Software, 3. Reti, 4. Sicurezza, 5. Altro)\n");
                short tipologia;
                scanf("%hd", &tipologia);

                if (current->tipologia == tipologia) {
                    printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %s, Stato: %hd, ID Tecnico: %d\n",
                        current->codice, current->luogo, current->tipologia, current->descrizione,
                        current->data.giorno, current->data.mese, current->data.anno,
                        current->oraInizio, current->oraFine, current->urgenza, current->stato, current->id_tecnico);
                }
                current = current->next;
            }
            break;
        case 2:
            while (current != NULL) {

                printf("Inserisci il codice della richiesta da visualizzare: ");
                int codice;
                scanf("%d", &codice);

                if (current->codice == codice) {
                    printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %s, Stato: %hd, ID Tecnico: %d\n",
                        current->codice, current->luogo, current->tipologia, current->descrizione,
                        current->data.giorno, current->data.mese, current->data.anno,
                        current->oraInizio, current->oraFine, current->urgenza, current->stato, current->id_tecnico);
                    break; // Esce dal ciclo dopo aver trovato la richiesta
                }
                current = current->next;
            }
            break;
        default:
            printf("Scelta non valida\n");
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
            printf("    Fascia Oraria: %d:00-%d:00\n", current->oraInizio, current->oraFine);
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

/* void generaReport(Richiesta *listaRichiesta, Tecnico *listaTecnico) {
    printf("\n========== REPORT STATISTICHE INTERVENTI ==========\n");
    
    // Contatori per tipologia
    int countTipologia[6] = {0}; // Indice 1-5
    
    // Contatori per stato
    int aperti = 0, chiusi = 0;
    
    // Per tempo medio: assumiamo che ogni intervento completato richieda un numero fisso di ore, 
    // o calcoliamo media delle ore lavorate dei tecnici
    int totaleOre = 0, numTecnici = 0;
    Tecnico *tec = listaTecnico;
    while (tec != NULL) {
        totaleOre += tec->num_ore_lavorate;
        numTecnici++;
        tec = tec->next;
    }
    float tempoMedio = numTecnici > 0 ? (float)totaleOre / numTecnici : 0;
    
    // Tecnico più attivo
    Tecnico *tecnicoAttivo = NULL;
    int maxOre = -1;
    tec = listaTecnico;
    while (tec != NULL) {
        if (tec->num_ore_lavorate > maxOre) {
            maxOre = tec->num_ore_lavorate;
            tecnicoAttivo = tec;
        }
        tec = tec->next;
    }
    
    // Aree con più problemi: usiamo una mappa semplice, assumiamo max 100 aree uniche
    char aree[100][51];
    int countAree[100] = {0};
    int numAree = 0;
    
    Richiesta *current = listaRichiesta;
    while (current != NULL) {
        // Tipologia
        if (current->tipologia >= 1 && current->tipologia <= 5) {
            countTipologia[current->tipologia]++;
        }
        
        // Stato
        if (current->stato >= 0 && current->stato <= 2) {
            aperti++;
        } else if (current->stato == 3 || current->stato == 4) {
            chiusi++;
        }
        
        // Aree
        int found = 0;
        for (int i = 0; i < numAree; i++) {
            if (strcmp(aree[i], current->luogo) == 0) {
                countAree[i]++;
                found = 1;
                break;
            }
        }
        if (!found && numAree < 100) {
            strcpy(aree[numAree], current->luogo);
            countAree[numAree] = 1;
            numAree++;
        }
        
        current = current->next;
    }
    
    // Trova area con più problemi
    char areaMax[51] = "";
    int maxProblemi = 0;
    for (int i = 0; i < numAree; i++) {
        if (countAree[i] > maxProblemi) {
            maxProblemi = countAree[i];
            strcpy(areaMax, aree[i]);
        }
    }
    
    // Output del report
    printf("Interventi per tipologia:\n");
    printf("  Hardware: %d\n", countTipologia[1]);
    printf("  Software: %d\n", countTipologia[2]);
    printf("  Reti: %d\n", countTipologia[3]);
    printf("  Sicurezza: %d\n", countTipologia[4]);
    printf("  Altro: %d\n", countTipologia[5]);
    
    printf("\nInterventi aperti: %d\n", aperti);
    printf("Interventi chiusi: %d\n", chiusi);
    
    printf("\nTempo medio di completamento: %.2f ore per tecnico\n", tempoMedio);
    
    if (tecnicoAttivo != NULL) {
        printf("\nTecnico più attivo: %s (ID: %d) con %d ore lavorate\n", tecnicoAttivo->nome, tecnicoAttivo->id, tecnicoAttivo->num_ore_lavorate);
    } else {
        printf("\nNessun tecnico trovato\n");
    }
    
    printf("\nArea con più problemi: %s (%d interventi)\n", areaMax, maxProblemi);
    
    printf("===================================================\n\n");
} */