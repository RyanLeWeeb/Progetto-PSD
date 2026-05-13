#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h> // Per la funzione sleep()
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

    if (listaRichiesta == NULL) {
        listaRichiesta = newNode;
    } else {
        Richiesta *current = listaRichiesta;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

    newNode->oraInizio = NULL; // Orario di inizio non ancora pianificato
    newNode->oraFine = NULL; // Orario di fine non ancora pianificato
    newNode->id_tecnico = NULL; // Tecnico non ancora assegnato

    aggiornaListaRichiesta(listaRichiesta);
}

void aggiornaStatoRichiesta(Richiesta *listaRichiesta, int codice, Tecnico *listaTecnici) {
    Richiesta *currentRichiesta = listaRichiesta;
    Tecnico *currentTecnici = listaTecnici;

    while(currentRichiesta != NULL) {
        if (currentRichiesta->codice == codice) {
            short choice;
            printf("Quale nuovo stato vuoi assegnare alla richiesta?\n");
            printf("1. Pianificata\n2. In lavorazione\n3. Conclusa\n4. Annullata\n");
            scanf("%hd", &choice);
            switch (choice){
                case 1:
                    if(currentRichiesta->stato != 0){ // Si può passare da "aperta" a "pianificata"
                        printf("Errore: Lo stato può essere aggiornato a Pianificata solo se la richiesta è attualmente Aperta\n");
                        return;
                    }
                    while(currentTecnici != NULL)
                    {
                        if (currentTecnici->specializzazione == currentRichiesta->tipologia) {
                            // Stampa le informazioni di ogni tecnico con specializzazione corrispondente alla tipologia della richiesta
                            printf("Tecnico ID: %d, Nome: %s\n", currentTecnici->id, currentTecnici->nome);
                            break;
                        }
                        currentTecnici = currentTecnici->next;
                    }

                    back:
                    printf("Inserire l'ID del tecnico da assegnare alla richiesta: ");
                    int id_tecnico;
                    scanf("%d", &id_tecnico);

                    // Verifica che l'ID inserito corrisponda a un tecnico con specializzazione corrispondente alla tipologia della richiesta
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
                        goto back;
                    }

                    backtwo:
                    printf("Inserire l'ora di inizio dell'intervento (0-23): ");
                    scanf("%d", &currentRichiesta->oraInizio);

                    printf("Inserire l'ora di fine dell'intervento (0-23): ");
                    scanf("%d", &currentRichiesta->oraFine);

                    Richiesta *tempRichiesta = listaRichiesta;
                    int conflitto = 0;
                    while (tempRichiesta != NULL) {
                        if (tempRichiesta != currentRichiesta &&
                            confrontaDate(tempRichiesta->data, currentRichiesta->data) == 0 &&
                            sonoOrariSovrapposti(tempRichiesta->oraInizio, tempRichiesta->oraFine,
                                                 currentRichiesta->oraInizio, currentRichiesta->oraFine) &&
                            tempRichiesta->id_tecnico == id_tecnico && tempRichiesta->stato != 4 && tempRichiesta->stato != 5 && tempRichiesta->stato != 0) { 
                                // Esclude richieste annullate, concluse, o aperte (non ancora pianificate)
                            conflitto = 1;
                            break;
                        }
                        tempRichiesta = tempRichiesta->next;
                    }

                    // Se c'è un conflitto, avvisa l'utente e chiedi se vuole cambiare la data dell'intervento
                    if (conflitto) {
                        printf("Errore: Il tecnico selezionato è già assegnato a un intervento nello stesso orario\n");
                        printf("Cambiare data dell'intervento? (1. Sì, 2. No)\n");
                        if(scanf("%d", &choice) && choice == 1) {
                            printf("Inserire data dell'intervento (gg/mm/aaaa): ");
                            scanf("%d/%d/%d", &currentRichiesta->data.giorno, &currentRichiesta->data.mese, &currentRichiesta->data.anno);
                        }
                        goto backtwo;
                    }

                    currentRichiesta->stato = 1; // Pianificata
                    printf("Stato aggiornato a Pianificata\n");
                break;
                case 2:
                    if(currentRichiesta->stato != 1){ // Si può passare da "pianificata" a "in lavorazione"
                        printf("Errore: Lo stato può essere aggiornato a In lavorazione solo se la richiesta è attualmente Pianificata\n");
                        return;
                    }
                    currentRichiesta->stato = 2; // In lavorazione
                    printf("Stato aggiornato a In lavorazione\n");
                break;
                case 3:
                    if(currentRichiesta->stato != 2){ // Si può passare da "in lavorazione" a "conclusa"
                        printf("Errore: Lo stato può essere aggiornato a Conclusa solo se la richiesta è attualmente In lavorazione\n");
                        return;
                    }
                    currentRichiesta->stato = 3; // Conclusa
                    printf("Stato aggiornato a Conclusa\n");
                break;
                case 4:
                    if(currentRichiesta->stato != 3){ // Si può passare da "conclusa" a "annullata"
                        printf("Errore: Lo stato può essere aggiornato a Annullata solo se la richiesta è attualmente Conclusa\n");
                        return;
                    }
                    currentRichiesta->stato = 4; // Annullata
                    printf("Stato aggiornato a Annullata\n");
                break;
                default:
                    printf("Scelta non valida\n");
            }
        }else{
            currentRichiesta = currentRichiesta->next;
        }
    }
    aggiornaListaRichiesta(listaRichiesta);
}

// 1. Tipologia
// 2. Codice Richiesta
void ricercaRichieste(Richiesta *listaRichiesta) {
    Richiesta *current = listaRichiesta;
    short choice;

    printf("Quale criterio vuoi utilizzare per ricercare le richieste?\n");
    printf("1. Tipologia\n2. Codice Richiesta\n");
    scanf("%hd", &choice);

    switch (choice){
        case 1:
            while (current != NULL) {

                printf("Quale tipologia vuoi ricercare?\n(1. Hardware, 2. Software, 3. Reti, 4. Sicurezza, 5. Altro)\n");
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

                printf("Inserisci il codice della richiesta da ricercare: ");
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

    printf("\n========== REPORT STATISTICHE INTERVENTI ==========%\n");
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

// per visualizzare le richieste in base a diversi criteri (1. Tipologia, 2. Stato, 3. Urgenza, 4. Luogo, 5. Tecnico)
void visualizzazioneRichieste(Richiesta *lista){
    Richiesta *current = lista;
    short choice;

    printf("Quale criterio vuoi utilizzare per visualizzare le richieste?\n");
    printf("1. Tipologia\n2. Stato\n3. Urgenza\n4. Luogo\n5. Tecnico\n");
    scanf("%hd", &choice);

    switch (choice){
        case 1:
            printf("Visualizzazione delle richieste per tipologia...\n");    
            sleep(2); // Pausa di 2 secondi per permettere all'utente di leggere il messaggio prima di visualizzare le richieste

            short tipologia_corrente = 1;
            while (tipologia_corrente <= NUM_SPECIALIZZAZIONI){
                while (current != NULL) {

                    // Si passa per ogni tipologia, stampando ogni tipo in ordine: 1. Hardware, 2. Software, 3. Reti, 4. Sicurezza, 5. Altro

                    if (current->tipologia == tipologia_corrente) {
                        printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %s, Stato: %hd, ID Tecnico: %d\n",
                            current->codice, current->luogo, current->tipologia, current->descrizione,
                            current->data.giorno, current->data.mese, current->data.anno,
                            current->oraInizio, current->oraFine, current->urgenza, current->stato, current->id_tecnico);
                    }
                    current = current->next;
                }
                tipologia_corrente += 1;
            }

        break;
        case 2:
            printf("Visualizzazione delle richieste per stato, saltando le richieste concluse e annullate...\n");    
            sleep(2); // Pausa di 2 secondi per permettere all'utente di leggere il messaggio prima di visualizzare le richieste

            short stato_corrente = 0; // Gli stati iniziano da 0 (aperta)
            while (stato_corrente <= 2){ // "2. in lavorazione" è l'ultimo stato che vogliamo visualizzare, saltando "3. conclusa" e "4. annullata"
                while (current != NULL) {

                    /*
                    Si passa per ogni stato, stampando ogni tipo in ordine: 0. aperta, 1. pianificata, 2. in lavorazione
                    saltando però le richieste con stato 3. conclusa e 4. annullata, che vengono visualizzate in un report a parte
                    */

                    if (current->stato == stato_corrente) {
                        printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %s, Stato: %hd, ID Tecnico: %d\n",
                            current->codice, current->luogo, current->tipologia, current->descrizione,
                            current->data.giorno, current->data.mese, current->data.anno,
                            current->oraInizio, current->oraFine, current->urgenza, current->stato, current->id_tecnico);
                    }
                    current = current->next;
                }
                stato_corrente += 1;
            }
        break;
        case 3:
            printf("Visualizzazione delle richieste per urgenza...\n");    
            sleep(2); // Pausa di 2 secondi per permettere all'utente di leggere il messaggio prima di visualizzare le richieste

            short urgenza_corrente = 3; // Si parte dall'urgenza più alta (3. Alta) e si scende fino alla più bassa (1. Bassa)
            while (urgenza_corrente >= 1){
                while (current != NULL) {

                    if (current->urgenza == urgenza_corrente) {
                        printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %s, Stato: %hd, ID Tecnico: %d\n",
                            current->codice, current->luogo, current->tipologia, current->descrizione,
                            current->data.giorno, current->data.mese, current->data.anno,
                            current->oraInizio, current->oraFine, current->urgenza, current->stato, current->id_tecnico);
                    }
                    current = current->next;
                }
                urgenza_corrente -= 1;
            }
        break;
        case 4:
            printf("Visualizzazione delle richieste per luogo...\n");    
            sleep(2); // Pausa di 2 secondi per permettere all'utente di leggere il messaggio prima di visualizzare le richieste

            char luogo_corrente[51];
            printf("Inserisci il luogo da visualizzare: ");
            scanf("%50s", luogo_corrente);

                // Si visualizzano tutte le richieste per il luogo inserito, saltando le richieste concluse oppure annullate.

            while (current != NULL) {
                if (strcmp(current->luogo, luogo_corrente) == 0 && current->stato != 3 && current->stato != 4) { // Stato 3 = conclusa, Stato 4 = annullata
                    printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %s, Stato: %hd, ID Tecnico: %d\n",
                        current->codice, current->luogo, current->tipologia, current->descrizione,
                        current->data.giorno, current->data.mese, current->data.anno,
                        current->oraInizio, current->oraFine, current->urgenza, current->stato, current->id_tecnico);
                }
                current = current->next;
            }
        break;
        case 5:
            printf("Visualizzazione delle richieste per tecnico...\n");    
            sleep(2); // Pausa di 2 secondi per permettere all'utente di leggere il messaggio prima di visualizzare le richieste

            int id_tecnico_corrente;
            printf("Inserisci l'ID del tecnico da visualizzare: ");
            scanf("%d", &id_tecnico_corrente);

                // Si visualizzano tutte le richieste assegnate al tecnico con l'ID inserito, saltando le richieste concluse oppure annullate.

            while (current != NULL) {
                if (current->id_tecnico == id_tecnico_corrente && current->stato != 3 && current->stato != 4) { // Stato 3 = conclusa, Stato 4 = annullata
                    printf("Codice: %d, Luogo: %s, Tipologia: %hd, Descrizione: %s, Data: %02d/%02d/%04d, Ore: %s, Stato: %hd, ID Tecnico: %d\n",
                        current->codice, current->luogo, current->tipologia, current->descrizione,
                        current->data.giorno, current->data.mese, current->data.anno,
                        current->oraInizio, current->oraFine, current->urgenza, current->stato, current->id_tecnico);
                }
                current = current->next;
            }
        break;
        default:
            printf("Scelta non valida\n");
    }
}