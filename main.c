#include <stdio.h>
#include <stdlib.h>
#include "Include/Richiesta.h"
#include "Include/Utility.h"
#include "Include/Tecnico.h"


int main() {
    Richiesta *r =creaListaRichiesta();
    Tecnico *t = creaListaTecnico();

    int codice;
    int choice;
    do {
        printf("\n========== GESTIONE RICHIESTE DI ASSISTENZA ==========\n");
        printf("1. Aggiungi richiesta\n");
        printf("2. Aggiungi Tecnico\n");
        printf("3. Visualizza tutte le richieste\n");
        printf("4. Ricerca richieste\n");
        printf("5. Aggiorna stato richiesta\n");
        printf("6. Visualizza storico interventi completati\n");
        printf("7. Genera report\n");
        printf("0. Esci\n");
        printf("Scegli un'opzione: ");
        scanf("%d", &choice);
        getchar(); // Consuma il newline rimasto nel buffer dopo scanf

        switch (choice) {
            case 1:
                aggiungiRichiesta(r);
                break;
            case 2:
                aggiungiTecnico(t);
                break;
            case 3:
                visualizzazioneRichieste(r);
                break;
            case 4:
                ricercaRichieste(r);
                break;
            case 5:
                printf("Inserisci il codice della richiesta: ");
                scanf("%d", &codice);
                aggiornaStatoRichiesta(r, codice, t);
                break;
            case 6:
                visualizzaStoricoInterventiCompletati(r);
                break;
            case 7:
                generaReport(r, t);
                break;
            case 0:
                printf("Uscita in corso...\n");
                break;
            default:
                printf("Scelta non valida, riprova.\n");
        }
    } while (choice != 0);

}