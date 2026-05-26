#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Include/Test.h"
#include "../Include/Richiesta.h"
#include "../Include/Tecnico.h"
#include "../Include/Utility.h"

void test_verifica_registrazione_richiesta(Richiesta **lista) {
    printf("\nTest di verifica registrazione richiesta...\n");
    
    // 1. Modifichiamo direttamente la testa della lista reale nel main
    *lista = aggiungiRichiesta(*lista, TEST_RICHIESTE_FILE, "Ufficio 1", 1, 2, "Problema con il computer");
    
    // 2. Usiamo un puntatore singolo locale per scorrere la lista in sicurezza
    Richiesta *current = *lista;
    int found = 0;
    int codice_da_rimuovere = -1;

    while (current != NULL) {
        if (strcmp(current->luogo, "Ufficio 1") == 0 && 
            current->tipologia == 1 && current->urgenza == 2 && 
            strcmp(current->descrizione, "Problema con il computer") == 0 &&
            current->stato == 0 &&
            current->id_tecnico == 0 &&
            current->data.giorno == 0 && current->data.mese == 0 && current->data.anno == 0 &&
            current->oraInizio == 0 && current->oraFine == 0) {
            
            found = 1;
            codice_da_rimuovere = current->codice;
            break;
        }
        current = current->next;
    }

    printf("Dati inseriti: Luogo: %s, Tipologia: %hd, Urgenza: %hd, Descrizione: %s\n", 
           "Ufficio 1", 1, 2, "Problema con il computer");
    printf("Dati trovati: Luogo: %s, Tipologia: %hd, Urgenza: %hd, Descrizione: %s\n", 
           found ? current->luogo : "N/A", 
           found ? current->tipologia : -1, 
           found ? current->urgenza : -1, 
           found ? current->descrizione : "N/A");

           

    if (found) {
        printf("Test superato: richiesta registrata correttamente\n");
    } else {
        printf("Test fallito: richiesta non trovata o dati non corretti\n");
    }
    rimuoviRichiesta(lista, TEST_RICHIESTE_FILE, codice_da_rimuovere);
}

void test_verifica_registrazione_tecnico(Tecnico **lista) {
    printf("\nTest di verifica registrazione tecnico...\n");
    
    // 1. Inseriamo il tecnico modificando direttamente la lista reale.
    // Assumiamo che aggiungiTecnico accetti (Tecnico *lista) e restituisca la nuova testa.
    *lista = aggiungiTecnico(*lista, TEST_TECNICO_FILE, "Mario Rossi", 2);
    
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
    } else {
        printf("Test fallito: tecnico non trovato o dati non corretti\n");
    }
    rimuoviTecnico(lista, TEST_TECNICO_FILE, id_da_rimuovere);
}

void test_verifica_assegnazione_tecnico_e_aggiornamento_stato(Richiesta **listaRichieste, Tecnico **listaTecnici) {
    printf("\nTest di verifica assegnazione tecnico e aggiornamento stato...\n");
    
    // 1. Inseriamo i dati modificando direttamente le liste reali del main tramite deferenziazione (*)
    *listaRichieste = aggiungiRichiesta(*listaRichieste, TEST_RICHIESTE_FILE, "Appartamento test1234", 2, 1, "TEST");
    *listaTecnici = aggiungiTecnico(*listaTecnici, TEST_TECNICO_FILE, "Test Tecnico", 2);
    
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
    int id_test = -1;
    while (tecTest != NULL) {
        if (strcmp(tecTest->nome, "Test Tecnico") == 0 && tecTest->specializzazione == 2) {
            id_test = tecTest->id;
            break;
        }
        tecTest = tecTest->next;
    }

    // Controllo di sicurezza: se non abbiamo trovato i nodi appena creati, evitiamo di procedere
    if (codice_richiesta == -1 || id_test == -1) {
        printf("Test fallito: Impossibile recuperare i nodi di test creati.\n");
        return;
    }
    
    Data data_oracolo = {1, 1, 2026};
    // 3. Eseguiamo la funzione di aggiornamento sulla lista reale
    // Usiamo reqTest (che punta specificamente al nodo di test trovato prima) per verificare i dati
    if (aggiornaStatoRichiesta(*listaRichieste, TEST_RICHIESTE_FILE, codice_richiesta, *listaTecnici, 1, id_test, 9, 17, (Data){1, 1, 2026}) &&
        reqTest->id_tecnico == id_test &&
        reqTest->stato == 1 &&
        reqTest->oraInizio == 9 &&
        reqTest->oraFine == 17 &&
        reqTest->data.giorno == 1 &&
        reqTest->data.mese == 1 &&
        reqTest->data.anno == 2026) {
        
        printf("Test superato: tecnico assegnato correttamente alla richiesta\n");
    } else {
        printf("Test fallito: impossibile assegnare il tecnico alla richiesta o dati non corrispondenti\n");
    }
    
    // 4. Pulizia: rimuove la richiesta e il tecnico passando i doppi puntatori reali del main
    rimuoviRichiesta(listaRichieste, TEST_RICHIESTE_FILE, codice_richiesta);
    rimuoviTecnico(listaTecnici, TEST_TECNICO_FILE, id_test);
}

void test_report(Richiesta **listaRichieste, Tecnico **listaTecnici) {
    // Creazione del file "oracolo.txt" con dati da aspettarsi come output
    FILE *file = fopen("Test/TC8_Report_Oracolo.txt", "w");
    if (file == NULL) {
        printf("Errore: Impossibile creare il file oracolo.txt\n");
        return;
    }

    const char *tipologiaNomi[NUM_SPECIALIZZAZIONI] = {"Hardware", "Software", "Reti", "Sicurezza", "Altro"};
    int tipologiaCount[NUM_SPECIALIZZAZIONI] = {0};
    int statoCount[5] = {0};
    int completatiCount = 0;
    int totalCompletionTime = 0;

    #define MAX_AREE 100
    char aree[MAX_AREE][51] = {{0}};
    int areeCount[MAX_AREE] = {0};
    int areeSize = 0;

    Richiesta *current = *listaRichieste;
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
    Tecnico *currentTecnico = *listaTecnici;

    while (currentTecnico != NULL) {
        // Calcoli le ore al volo per il tecnico corrente
        int oreTotali = orelavorate(*listaRichieste, currentTecnico);

        // Il calcolo del massimo rimane identico usando la variabile locale oreTotali
        if (oreTotali > maxOreTecnico || tecnicoAttivo == NULL) {
            maxOreTecnico = oreTotali;
            tecnicoAttivo = currentTecnico;
        }
        currentTecnico = currentTecnico->next;
    }

    fprintf(file, "\n========== REPORT STATISTICHE INTERVENTI ==========\n");
    fprintf(file, "Interventi per tipologia:\n");
    for (int i = 0; i < NUM_SPECIALIZZAZIONI; i++) {
        fprintf(file, "  %s: %d\n", tipologiaNomi[i], tipologiaCount[i]);
    }

    int aperteCount = statoCount[0] + statoCount[1] + statoCount[2];
    int chiuseCount = statoCount[3] + statoCount[4];
    fprintf(file, "\nInterventi aperti: %d\n", aperteCount);
    fprintf(file, "Interventi chiusi: %d\n", chiuseCount);
    fprintf(file, "  - Aperte: %d\n", statoCount[0]);
    fprintf(file, "  - Pianificate: %d\n", statoCount[1]);
    fprintf(file, "  - In lavorazione: %d\n", statoCount[2]);
    fprintf(file, "  - Concluse: %d\n", statoCount[3]);
    fprintf(file, "  - Annullate: %d\n", statoCount[4]);

    if (completatiCount > 0) {
        double media = (double)totalCompletionTime / completatiCount;
        fprintf(file, "\nTempo medio di completamento (ore per intervento concluso): %.2f\n", media);
    } else {
        fprintf(file, "\nTempo medio di completamento: nessun intervento concluso\n");
    }

    if (tecnicoAttivo != NULL) {
        fprintf(file, "\nTecnico piu\' attivo: %s (ID %d) con %d ore lavorate\n",
               tecnicoAttivo->nome, tecnicoAttivo->id, maxOreTecnico);
    } else {
        fprintf(file, "\nTecnico piu\' attivo: nessun tecnico trovato\n");
    }

    if (maxAreeCount > 0) {
        fprintf(file, "\nAree con piu\' problemi (numero di richieste):\n");
        for (int i = 0; i < areeSize; i++) {
            if (areeCount[i] == maxAreeCount) {
                fprintf(file, "  %s: %d\n", aree[i], areeCount[i]);
            }
        }
    } else {
        fprintf(file, "\nAree con piu\' problemi: nessuna richiesta registrata\n");
    }
    fprintf(file, "===================================================");

    fclose(file);
    printf("File oracolo.txt creato con successo. Controllare il file manualmente per confrontare i dati.\n");

    generaReport(*listaRichieste, *listaTecnici); // Genera il report che dovrebbe includere l'intervento completato
}

void test_storico_interventi(Richiesta *listaRichieste) {
        // Creazione del file "oracolo.txt" con dati da aspettarsi come output
    FILE *file = fopen("Test/TC7_Storico_Oracolo.txt", "w");
    if (file == NULL) {
        printf("Errore: Impossibile creare il file oracolo.txt\n");
        return;
    }

    fprintf(file ,"\n========== STORICO INTERVENTI COMPLETATI ==========\n");
    
    Richiesta *current = listaRichieste;
    int count = 0;
    
    while (current != NULL) {
        if (current->stato == 3) { // 3 = conclusa
            count++;
            fprintf(file ,"\n[%d] Codice Richiesta: %d\n", count, current->codice);
            fprintf(file ,"    Luogo: %s\n", current->luogo);
            fprintf(file ,"    Tipologia: ");
            switch(current->tipologia) {
                case 1: fprintf(file ,"Hardware\n"); break;
                case 2: fprintf(file ,"Software\n"); break;
                case 3: fprintf(file ,"Reti\n"); break;
                case 4: fprintf(file ,"Sicurezza\n"); break;
                case 5: fprintf(file ,"Altro\n"); break;
                default: fprintf(file ,"Non specificata\n");
            }
            fprintf(file ,"    Descrizione: %s\n", current->descrizione);
            fprintf(file ,"    Data Richiesta: %02d/%02d/%04d\n", current->data.giorno, current->data.mese, current->data.anno);
            fprintf(file ,"    Fascia Oraria: %d:00-%d:00\n", current->oraInizio, current->oraFine);
            fprintf(file ,"    Urgenza: ");
            switch(current->urgenza) {
                case 1: fprintf(file ,"Bassa\n"); break;
                case 2: fprintf(file ,"Media\n"); break;
                case 3: fprintf(file ,"Alta\n"); break;
                default: fprintf(file ,"Non specificata\n");
            }
            fprintf(file ,"    ID Tecnico Assegnato: %d\n", current->id_tecnico);
            fprintf(file ,"    ---\n");
        }
        current = current->next;
    }
    
    if (count == 0) {
        fprintf(file ,"Nessun intervento completato al momento.\n");
    } else {
        fprintf(file ,"\nTotale interventi completati: %d\n", count);
    }
    fprintf(file ,"===================================================");

    fclose(file);
    visualizzaStoricoInterventiCompletati(listaRichieste);
}