#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Include/Richiesta.h"
#include "Include/Utility.h"
#include "Include/Tecnico.h"
#include "Include/Test.h"

// Funzione di utilità per svuotare il buffer in modo sicuro
void svuotaBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    Richiesta *r = creaListaRichiesta();
    Tecnico *t = creaListaTecnico();

    int codice;
    int choice;
    char luogo[51];
    char descrizione[101];
    char nome[51];
    short tipologia;
    short urgenza;
    short specializzazione;
    short criterio;
    int id_tecnico;
    int oraInizio;
    int oraFine;
    short stato;
    Data data;

    do {
        printf("\n========== GESTIONE RICHIESTE DI ASSISTENZA ==========\n");
        printf("1. Aggiungi richiesta\n");
        printf("2. Aggiungi Tecnico\n");
        printf("3. Visualizza tutte le richieste\n");
        printf("4. Ricerca richieste\n");
        printf("5. Aggiorna stato richiesta\n");
        printf("6. Visualizza storico interventi completati\n");
        printf("7. Genera report\n");
        printf("8. Esegui test\n");
        printf("0. Esci\n");
        printf("Scegli un'opzione: ");
        
        // Protezione contro input non numerici
        if (scanf("%d", &choice) != 1) {
            printf("Errore: Inserire un numero valido.\n");
            svuotaBuffer(); // Svuota l'input errato per evitare loop infiniti
            choice = -1;    // Forza il default del sistema
            continue;
        }
        svuotaBuffer(); // Consuma il newline residuo in modo sicuro

        switch (choice) {
            case 1: 
                printf("\nInserisci il luogo dell'intervento (MAX. 50 caratteri):\n");
                if (fgets(luogo, sizeof(luogo), stdin) != NULL) {
                    luogo[strcspn(luogo, "\n")] = '\0';
                }
                
                printf("Inserisci la tipologia dell'intervento (1-5):\n1. Hardware\n2. Software\n3. Reti\n4. Sicurezza\n5. Altro\n");
                scanf("%hd", &tipologia);
                printf("Inserisci l'urgenza dell'intervento (1-3):\n1. Bassa\n2. Media\n3. Alta\n");
                scanf("%hd", &urgenza);
                svuotaBuffer();
                
                printf("Inserisci la descrizione dell'intervento (MAX. 100 caratteri):\n");
                if (fgets(descrizione, sizeof(descrizione), stdin) != NULL) {
                    descrizione[strcspn(descrizione, "\n")] = '\0';
                }
                r = aggiungiRichiesta(r, luogo, tipologia, urgenza, descrizione);
                break;

            case 2: 
                printf("Inserisci il nome del tecnico (MAX. 50 caratteri):\n");
                if (fgets(nome, sizeof(nome), stdin) != NULL) {
                    nome[strcspn(nome, "\n")] = '\0';
                }
                printf("Inserisci la specializzazione del tecnico:\n1. Hardware\n2. Software\n3. Reti\n4. Sicurezza\n5. Altro\n");
                scanf("%hd", &specializzazione);
                svuotaBuffer();
                t = aggiungiTecnico(t, nome, specializzazione);
                break;

            case 3: 
                printf("Quale criterio vuoi utilizzare per visualizzare le richieste?\n1. Tipologia\n2. Stato\n3. Urgenza\n4. Luogo\n5. Tecnico\n");
                scanf("%hd", &criterio);
                svuotaBuffer();
                
                if (criterio == 4) {
                    printf("Inserisci il luogo da visualizzare: ");
                    if (fgets(luogo, sizeof(luogo), stdin) != NULL) {
                        luogo[strcspn(luogo, "\n")] = '\0';
                    }
                    visualizzazioneRichieste(r, criterio, 0, 0, 0, luogo, 0);
                } else if (criterio == 5) {
                    printf("Inserisci l'ID del tecnico da visualizzare: ");
                    scanf("%d", &id_tecnico);
                    svuotaBuffer();
                    visualizzazioneRichieste(r, criterio, 0, 0, 0, NULL, id_tecnico);
                } else {
                    visualizzazioneRichieste(r, criterio, 0, 0, 0, NULL, 0);
                }
                break;

            case 4: 
                printf("Quale criterio vuoi utilizzare per ricercare le richieste?\n1. Tipologia\n2. Codice Richiesta\n");
                scanf("%hd", &criterio);
                if (criterio == 1) {
                    printf("Quale tipologia vuoi ricercare?\n(1. Hardware, 2. Software, 3. Reti, 4. Sicurezza, 5. Altro)\n");
                    scanf("%hd", &tipologia);
                    ricercaRichieste(r, criterio, tipologia, 0);
                } else if (criterio == 2) {
                    printf("Inserisci il codice della richiesta da ricercare: ");
                    scanf("%d", &codice);
                    ricercaRichieste(r, criterio, 0, codice);
                } else {
                    printf("Scelta non valida\n");
                }
                svuotaBuffer();
                break;

            case 5: 
                printf("Inserisci il codice della richiesta: ");
                scanf("%d", &codice);
                printf("Quale nuovo stato vuoi assegnare alla richiesta?\n");
                printf("1. Pianificata\n2. In lavorazione\n3. Conclusa\n4. Annullata\n");
                scanf("%hd", &stato);
                
                id_tecnico = 0;
                oraInizio = 0;
                oraFine = 0;
                data.giorno = data.mese = data.anno = 0;
                
                if (stato == 1) {
                    printf("Inserire l'ID del tecnico da assegnare alla richiesta: ");
                    scanf("%d", &id_tecnico);
                    printf("Inserire l'ora di inizio dell'intervento (0-23): ");
                    scanf("%d", &oraInizio);
                    printf("Inserire l'ora di fine dell'intervento (0-23): ");
                    scanf("%d", &oraFine);
                    printf("Inserire data dell'intervento (gg/mm/aaaa): ");
                    scanf("%d/%d/%d", &data.giorno, &data.mese, &data.anno);
                }
                svuotaBuffer();
                
                if (aggiornaStatoRichiesta(r, codice, t, stato, id_tecnico, oraInizio, oraFine, data)) {
                    printf("Stato aggiornato con successo.\n");
                } else {
                    printf("Impossibile aggiornare lo stato della richiesta.\n");
                }
                break;

            case 6: 
                visualizzaStoricoInterventiCompletati(r);
                break;

            case 7: 
                generaReport(r, t);
                break;

            case 8: 
                printf("Scegli quale test eseguire:\n");
                printf("1. Verifica registrazione richiesta\n");
                printf("2. Verifica registrazione tecnico\n");
                printf("3. Verifica assegnazione tecnico e aggiornamento stato\n");
                printf("Scegli un'opzione: ");
                int choice_two;
                if (scanf("%d", &choice_two) == 1) {
                    svuotaBuffer();
                    switch (choice_two) {
                        case 1:
                            test_verifica_registrazione_richiesta(&r);
                            break;
                        case 2:
                            test_verifica_registrazione_tecnico(&t);
                            break;
                        case 3:
                            test_verifica_assegnazione_tecnico_e_aggiornamento_stato(&r, &t);
                            break;
                        default:
                            printf("Scelta non valida\n");
                    }
                } else {
                    printf("Input non valido.\n");
                    svuotaBuffer();
                }
                break; 

            case 0:
                printf("Uscita in corso...\n");
                break;
                
            default:
                printf("Scelta non valida, riprova.\n");
        }
    } while (choice != 0);

    deallocaListaRichieste(r);
    deallocaListaTecnici(t);
    return 0; 
}