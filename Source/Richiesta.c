#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h> // Per la funzione sleep()
#include "../Include/Richiesta.h"
#include "../Include/Tecnico.h"
#include "../Include/Utility.h"

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
        newNode->next = NULL;

        // Parse the line: codice-luogo-tipologia-descrizione-data-ore-urgenza-stato-id_tecnico
        if (sscanf(line, "%d-%50[^-]-%hd-%100[^-]-%d/%d/%d-%d-%d-%hd-%hd-%d",
                   &newNode->codice, newNode->luogo, &newNode->tipologia, newNode->descrizione,
                   &newNode->data.giorno, &newNode->data.mese, &newNode->data.anno,
                   &newNode->oraInizio, &newNode->oraFine, &newNode->urgenza, &newNode->stato, &newNode->id_tecnico) == 12) {
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
        fprintf(fp, "%d-%s-%hd-%s-%02d/%02d/%04d-%d-%d-%hd-%hd-%d\n",
                current->codice, current->luogo, current->tipologia, current->descrizione,
                current->data.giorno, current->data.mese, current->data.anno,
                current->oraInizio, current->oraFine, current->urgenza, current->stato, current->id_tecnico);
        current = current->next;
    }

    fclose(fp);
}

Richiesta *aggiungiRichiesta(Richiesta *listaRichiesta, const char *luogo, short tipologia, short urgenza, const char *descrizione){
    Richiesta *newNode = malloc(sizeof(Richiesta));
    if (newNode == NULL) {
        printf("Errore: Memoria non allocabile\n");
        exit(1);
    }

    newNode->stato = 0; // 0 = aperta
    newNode->next = NULL;

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

    strncpy(newNode->luogo, luogo, sizeof(newNode->luogo) - 1);
    newNode->luogo[sizeof(newNode->luogo) - 1] = '\0';
    newNode->tipologia = tipologia;
    strncpy(newNode->descrizione, descrizione, sizeof(newNode->descrizione) - 1);
    newNode->descrizione[sizeof(newNode->descrizione) - 1] = '\0';
    newNode->urgenza = urgenza;

    if (listaRichiesta == NULL) {
        listaRichiesta = newNode;
    } else {
        Richiesta *tail = listaRichiesta;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = newNode;
    }

    newNode->data.giorno = 0; // Data non ancora pianificata
    newNode->data.mese = 0;
    newNode->data.anno = 0;
    newNode->oraInizio = 0; // Orario di inizio non ancora pianificato
    newNode->oraFine = 0; // Orario di fine non ancora pianificato
    newNode->id_tecnico = 0; // Tecnico non ancora assegnato

    aggiornaListaRichiesta(listaRichiesta);
    return listaRichiesta;
}

void rimuoviRichiesta(Richiesta **listaRichiesta, int codice) {
    Richiesta *current = *listaRichiesta;
    Richiesta *prev = NULL;

    while (current != NULL) {
        if (current->codice == codice) {
            if (prev == NULL) {
                // Rimuove il primo nodo
                *listaRichiesta = current->next;
            } else {
                prev->next = current->next; // Rimuove il nodo corrente
            }
            aggiornaListaRichiesta(*listaRichiesta);
            printf("Richiesta con codice %d rimossa con successo.\n", codice);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Errore: richiesta con codice %d non trovata.\n", codice);
}

int aggiornaStatoRichiesta(Richiesta *listaRichiesta, int codice, Tecnico *listaTecnici, short nuovoStato, int id_tecnico, int oraInizio, int oraFine, Data data) {
    Richiesta *currentRichiesta = listaRichiesta;

    while (currentRichiesta != NULL) {
        if (currentRichiesta->codice == codice) {
            switch (nuovoStato){
                case 1:
                printf("\nStato della richiesta prima dell'aggiornamento: %hd\n", currentRichiesta->stato);
                printf("\nTipologia: %hd, Urgenza: %hd", currentRichiesta->tipologia, currentRichiesta->urgenza);
                
                    if (currentRichiesta->stato != 0) {
                        printf("Errore: Lo stato può essere aggiornato a Pianificata solo se la richiesta è attualmente Aperta\n");
                        return 0;
                    }

                    if (id_tecnico <= 0) {
                        printf("Errore: ID tecnico non valido\n");
                        return 0;
                    }

                    Tecnico *tempTecnico = listaTecnici;
                    int idValido = 0;
                    while (tempTecnico != NULL) {
                        if (tempTecnico->id == id_tecnico && tempTecnico->specializzazione == currentRichiesta->tipologia) {
                            idValido = 1;
                            break;
                        }
                        tempTecnico = tempTecnico->next;
                    }
                    if (!idValido) {
                        printf("Errore: L'ID del tecnico inserito non è valido o non corrisponde alla specializzazione della richiesta\n");
                        return 0;
                    }

                    if (oraInizio < 0 || oraInizio > 23 || oraFine < 0 || oraFine > 23 || oraInizio >= oraFine) {
                        printf("Errore: Orario non valido\n");
                        return 0;
                    }

                    currentRichiesta->id_tecnico = id_tecnico;
                    currentRichiesta->oraInizio = oraInizio;
                    currentRichiesta->oraFine = oraFine;
                    currentRichiesta->data = data;

                    {
                        Richiesta *tempRichiesta = listaRichiesta;
                        int conflitto = 0;
                        while (tempRichiesta != NULL) {
                            if (tempRichiesta != currentRichiesta &&
                                confrontaDate(tempRichiesta->data, currentRichiesta->data) == 0 &&
                                sonoOrariSovrapposti(tempRichiesta->oraInizio, tempRichiesta->oraFine,
                                                     currentRichiesta->oraInizio, currentRichiesta->oraFine) &&
                                tempRichiesta->id_tecnico == id_tecnico && tempRichiesta->stato != 4 && tempRichiesta->stato != 5 && tempRichiesta->stato != 0) {
                                    conflitto = 1;
                                    break;
                                }
                            tempRichiesta = tempRichiesta->next;
                        }
                        if (conflitto) {
                            printf("Errore: Il tecnico selezionato è già assegnato a un intervento nello stesso orario\n");
                            return 0;
                        }
                    }

                    currentRichiesta->stato = 1; // Pianificata
                    break;
                case 2:
                    if (currentRichiesta->stato != 1) {
                        printf("Errore: Lo stato può essere aggiornato a In lavorazione solo se la richiesta è attualmente Pianificata\n");
                        return 0;
                    }
                    currentRichiesta->stato = 2; // In lavorazione
                    break;
                case 3:
                    if (currentRichiesta->stato != 2) {
                        printf("Errore: Lo stato può essere aggiornato a Conclusa solo se la richiesta è attualmente In lavorazione\n");
                        return 0;
                    }
                    currentRichiesta->stato = 3; // Conclusa
                    break;
                case 4:
                    if (currentRichiesta->stato != 3) {
                        printf("Errore: Lo stato può essere aggiornato a Annullata solo se la richiesta è attualmente Conclusa\n");
                        return 0;
                    }
                    currentRichiesta->stato = 4; // Annullata
                    break;
                default:
                    printf("Scelta non valida\n");
                    return 0;
            }
            aggiornaListaRichiesta(listaRichiesta);
            return 1;
        }
        currentRichiesta = currentRichiesta->next;
    }
    printf("Errore: richiesta con codice %d non trovata\n", codice);
    return 0;
}

void ricercaRichieste(Richiesta *listaRichiesta, short criterio, short tipologia, int codice) {
    Richiesta *current = listaRichiesta;

    switch (criterio){
        case 1:
            while (current != NULL) {
                if (current->tipologia == tipologia) {
                    printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %02d-%02d, Urgenza: %hd, Stato: %hd, ID Tecnico: %d\n",
                        current->codice, current->luogo, current->tipologia, current->descrizione,
                        current->data.giorno, current->data.mese, current->data.anno,
                        current->oraInizio, current->oraFine, current->urgenza, current->stato, current->id_tecnico);
                }
                current = current->next;
            }
            break;
        case 2:
            while (current != NULL) {
                if (current->codice == codice) {
                    printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %02d-%02d, Urgenza: %hd, Stato: %hd, ID Tecnico: %d\n",
                        current->codice, current->luogo, current->tipologia, current->descrizione,
                        current->data.giorno, current->data.mese, current->data.anno,
                        current->oraInizio, current->oraFine, current->urgenza, current->stato, current->id_tecnico);
                    break;
                }
                current = current->next;
            }
            break;
        default:
            printf("Scelta non valida\n");
            break;
    }
}

void visualizzazioneRichieste(Richiesta *lista, short criterio, short tipologia, short stato, short urgenza, const char *luogo, int id_tecnico){
    Richiesta *current;

    switch (criterio){
        case 1:
            printf("Visualizzazione delle richieste per tipologia...\n");
            Sleep(2000);
            for (short tip = 1; tip <= NUM_SPECIALIZZAZIONI; tip++) {
                current = lista;
                while (current != NULL) {
                    if (current->tipologia == tip) {
                        printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %02d-%02d, Urgenza: %hd, Stato: %hd, ID Tecnico: %d\n",
                            current->codice, current->luogo, current->tipologia, current->descrizione,
                            current->data.giorno, current->data.mese, current->data.anno,
                            current->oraInizio, current->oraFine, current->urgenza, current->stato, current->id_tecnico);
                    }
                    current = current->next;
                }
            }
            break;
        case 2:
            printf("Visualizzazione delle richieste per stato, saltando le richieste concluse e annullate...\n");
            Sleep(2000);
            for (short stato_corrente = 0; stato_corrente <= 2; stato_corrente++) {
                current = lista;
                while (current != NULL) {
                    if (current->stato == stato_corrente) {
                        printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %02d-%02d, Urgenza: %hd, Stato: %hd, ID Tecnico: %d\n",
                            current->codice, current->luogo, current->tipologia, current->descrizione,
                            current->data.giorno, current->data.mese, current->data.anno,
                            current->oraInizio, current->oraFine, current->urgenza, current->stato, current->id_tecnico);
                    }
                    current = current->next;
                }
            }
            break;
        case 3:
            printf("Visualizzazione delle richieste per urgenza...\n");
            Sleep(2000);
            for (short urgenza_corrente = 3; urgenza_corrente >= 1; urgenza_corrente--) {
                current = lista;
                while (current != NULL) {
                    if (current->urgenza == urgenza_corrente) {
                        printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %02d-%02d, Urgenza: %hd, Stato: %hd, ID Tecnico: %d\n",
                            current->codice, current->luogo, current->tipologia, current->descrizione,
                            current->data.giorno, current->data.mese, current->data.anno,
                            current->oraInizio, current->oraFine, current->urgenza, current->stato, current->id_tecnico);
                    }
                    current = current->next;
                }
            }
            break;
        case 4:
            printf("Visualizzazione delle richieste per luogo...\n");
            Sleep(2000);
            current = lista;
            while (current != NULL) {
                if (luogo != NULL && strcmp(current->luogo, luogo) == 0 && current->stato != 3 && current->stato != 4) {
                    printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %02d-%02d, Urgenza: %hd, Stato: %hd, ID Tecnico: %d\n",
                        current->codice, current->luogo, current->tipologia, current->descrizione,
                        current->data.giorno, current->data.mese, current->data.anno,
                        current->oraInizio, current->oraFine, current->urgenza, current->stato, current->id_tecnico);
                }
                current = current->next;
            }
            break;
        case 5:
            printf("Visualizzazione delle richieste per tecnico...\n");
            Sleep(2000);
            current = lista;
            while (current != NULL) {
                if (current->id_tecnico == id_tecnico && current->stato != 3 && current->stato != 4) {
                    printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %02d-%02d, Urgenza: %hd, Stato: %hd, ID Tecnico: %d\n",
                        current->codice, current->luogo, current->tipologia, current->descrizione,
                        current->data.giorno, current->data.mese, current->data.anno,
                        current->oraInizio, current->oraFine, current->urgenza, current->stato, current->id_tecnico);
                }
                current = current->next;
            }
            break;
        default:
            printf("Scelta non valida\n");
            break;
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

void generaReport(Richiesta *listaRichiesta, Tecnico *listaTecnico) {
    const char *tipologiaNomi[NUM_SPECIALIZZAZIONI] = {"Hardware", "Software", "Reti", "Sicurezza", "Altro"};
    int tipologiaCount[NUM_SPECIALIZZAZIONI] = {0};
    int statoCount[5] = {0};
    int completatiCount = 0;
    int totalCompletionTime = 0;

    #define MAX_AREE 100
    char aree[MAX_AREE][51] = {{0}};
    int areeCount[MAX_AREE] = {0};
    int areeSize = 0;

    Richiesta *current = listaRichiesta;
    while (current != NULL) {
        if (current->tipologia >= 1 && current->tipologia <= NUM_SPECIALIZZAZIONI) {
            tipologiaCount[current->tipologia - 1]++;
        }

        if (current->stato >= 0 && current->stato <= 4) {
            statoCount[current->stato]++;
        }

        if (current->stato == 3) {
            int durata = current->oraFine - current->oraInizio;
            if (durata > 0) {
                totalCompletionTime += durata;
            }
            completatiCount++;
        }

        if (current->luogo[0] != '\0') {
            int found = 0;
            for (int i = 0; i < areeSize; i++) {
                if (strcmp(aree[i], current->luogo) == 0) {
                    areeCount[i]++;
                    found = 1;
                    break;
                }
            }
            if (!found && areeSize < MAX_AREE) {
                strncpy(aree[areeSize], current->luogo, sizeof(aree[areeSize]) - 1);
                aree[areeSize][sizeof(aree[areeSize]) - 1] = '\0';
                areeCount[areeSize] = 1;
                areeSize++;
            }
        }

        current = current->next;
    }

    int maxAreeCount = 0;
    for (int i = 0; i < areeSize; i++) {
        if (areeCount[i] > maxAreeCount) {
            maxAreeCount = areeCount[i];
        }
    }

    int maxOreTecnico = 0;
    Tecnico *tecnicoAttivo = NULL;
    Tecnico *currentTecnico = listaTecnico;
    while (currentTecnico != NULL) {
        int oreTotali = 0;
        Richiesta *richiesta = listaRichiesta;
        while (richiesta != NULL) {
            if (richiesta->id_tecnico == currentTecnico->id && richiesta->stato == 3) {
                int durata = richiesta->oraFine - richiesta->oraInizio;
                if (durata > 0) {
                    oreTotali += durata;
                }
            }
            richiesta = richiesta->next;
        }

        currentTecnico->num_ore_lavorate = oreTotali;
        if (oreTotali > maxOreTecnico || tecnicoAttivo == NULL) {
            maxOreTecnico = oreTotali;
            tecnicoAttivo = currentTecnico;
        }
        currentTecnico = currentTecnico->next;
    }

    printf("\n========== REPORT STATISTICHE INTERVENTI ==========" "\n");
    printf("Interventi per tipologia:\n");
    for (int i = 0; i < NUM_SPECIALIZZAZIONI; i++) {
        printf("  %s: %d\n", tipologiaNomi[i], tipologiaCount[i]);
    }

    int aperteCount = statoCount[0] + statoCount[1] + statoCount[2];
    int chiuseCount = statoCount[3] + statoCount[4];
    printf("\nInterventi aperti: %d\n", aperteCount);
    printf("Interventi chiusi: %d\n", chiuseCount);
    printf("  - Aperte: %d\n", statoCount[0]);
    printf("  - Pianificate: %d\n", statoCount[1]);
    printf("  - In lavorazione: %d\n", statoCount[2]);
    printf("  - Concluse: %d\n", statoCount[3]);
    printf("  - Annullate: %d\n", statoCount[4]);

    if (completatiCount > 0) {
        double media = (double)totalCompletionTime / completatiCount;
        printf("\nTempo medio di completamento (ore per intervento concluso): %.2f\n", media);
    } else {
        printf("\nTempo medio di completamento: nessun intervento concluso\n");
    }

    if (tecnicoAttivo != NULL) {
        printf("\nTecnico piu\' attivo: %s (ID %d) con %d ore lavorate\n",
               tecnicoAttivo->nome, tecnicoAttivo->id, maxOreTecnico);
    } else {
        printf("\nTecnico piu\' attivo: nessun tecnico trovato\n");
    }

    if (maxAreeCount > 0) {
        printf("\nAree con piu\' problemi (numero di richieste):\n");
        for (int i = 0; i < areeSize; i++) {
            if (areeCount[i] == maxAreeCount) {
                printf("  %s: %d\n", aree[i], areeCount[i]);
            }
        }
    } else {
        printf("\nAree con piu\' problemi: nessuna richiesta registrata\n");
    }
    printf("===================================================\n\n");
}

