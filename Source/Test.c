#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Include/Test.h"
#include "../Include/Richiesta.h"
#include "../Include/Tecnico.h"
#include "../Include/Utility.h"

static Richiesta *creaRichiestaTest(int codice, const char *luogo, short tipologia,
                                    const char *descrizione, Data data,
                                    int oraInizio, int oraFine, short urgenza,
                                    short stato, int id_tecnico) {
    Richiesta *r = malloc(sizeof(Richiesta));
    if (r == NULL) {
        printf("Errore: memoria non allocabile per il test\n");
        exit(1);
    }

    r->codice = codice;
    strncpy(r->luogo, luogo, sizeof(r->luogo) - 1);
    r->luogo[sizeof(r->luogo) - 1] = '\0';
    r->tipologia = tipologia;
    strncpy(r->descrizione, descrizione, sizeof(r->descrizione) - 1);
    r->descrizione[sizeof(r->descrizione) - 1] = '\0';
    r->data = data;
    r->oraInizio = oraInizio;
    r->oraFine = oraFine;
    r->urgenza = urgenza;
    r->stato = stato;
    r->id_tecnico = id_tecnico;
    r->next = NULL;

    return r;
}

static char *leggiContenutoFile(const char *percorso) {
    FILE *fp = fopen(percorso, "r");
    if (fp == NULL) {
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long dimensione = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buffer = malloc(dimensione + 1);
    if (buffer == NULL) {
        fclose(fp);
        return NULL;
    }

    size_t letti = fread(buffer, 1, dimensione, fp);
    buffer[letti] = '\0';
    fclose(fp);
    return buffer;
}

static int scriviContenutoFile(const char *percorso, const char *contenuto) {
    FILE *fp = fopen(percorso, "w");
    if (fp == NULL) {
        return 0;
    }

    if (contenuto != NULL) {
        fprintf(fp, "%s", contenuto);
    }
    fclose(fp);
    return 1;
}

void test_verifica_registrazione_richieste(void) {
    Data dataTest = {15, 5, 2026};
    Richiesta *richiesta = creaRichiestaTest(
        100001,
        "Appartamento 3A",
        2,
        "Problema con il PC dell'amministratore",
        dataTest,
        9,
        11,
        3,
        0,
        0
    );

    const char *percorsoFile = "Liste/Richiesta.txt";
    char *backupContenuto = leggiContenutoFile(percorsoFile);

    /* Scrive la richiesta su file usando la funzione del modulo Richiesta */
    aggiornaListaRichiesta(richiesta);

    FILE *fp = fopen(percorsoFile, "r");
    if (fp == NULL) {
        printf("TEST FALLITO: Impossibile aprire il file %s\n", percorsoFile);
        free(backupContenuto);
        free(richiesta);
        return;
    }

    char linea[256];
    if (fgets(linea, sizeof(linea), fp) == NULL) {
        printf("TEST FALLITO: File %s vuoto o non leggibile\n", percorsoFile);
        fclose(fp);
        if (backupContenuto != NULL) {
            scriviContenutoFile(percorsoFile, backupContenuto);
        }
        free(backupContenuto);
        free(richiesta);
        return;
    }
    fclose(fp);

    Richiesta richiestaDalFile;
    int campiLetti = sscanf(linea, "%d-%50[^-]-%hd-%100[^-]-%d/%d/%d-%d-%d-%hd-%hd-%d",
                            &richiestaDalFile.codice,
                            richiestaDalFile.luogo,
                            &richiestaDalFile.tipologia,
                            richiestaDalFile.descrizione,
                            &richiestaDalFile.data.giorno,
                            &richiestaDalFile.data.mese,
                            &richiestaDalFile.data.anno,
                            &richiestaDalFile.oraInizio,
                            &richiestaDalFile.oraFine,
                            &richiestaDalFile.urgenza,
                            &richiestaDalFile.stato,
                            &richiestaDalFile.id_tecnico);

    int passed = 1;
    passed &= (campiLetti == 12);
    passed &= (richiestaDalFile.codice == richiesta->codice);
    passed &= (strcmp(richiestaDalFile.luogo, richiesta->luogo) == 0);
    passed &= (richiestaDalFile.tipologia == richiesta->tipologia);
    passed &= (strcmp(richiestaDalFile.descrizione, richiesta->descrizione) == 0);
    passed &= (richiestaDalFile.data.giorno == richiesta->data.giorno);
    passed &= (richiestaDalFile.data.mese == richiesta->data.mese);
    passed &= (richiestaDalFile.data.anno == richiesta->data.anno);
    passed &= (richiestaDalFile.oraInizio == richiesta->oraInizio);
    passed &= (richiestaDalFile.oraFine == richiesta->oraFine);
    passed &= (richiestaDalFile.urgenza == richiesta->urgenza);
    passed &= (richiestaDalFile.stato == richiesta->stato);
    passed &= (richiestaDalFile.id_tecnico == richiesta->id_tecnico);

    if (passed) {
        printf("TEST PASSATO: Verifica della registrazione delle richieste e scrittura su file\n");
    } else {
        printf("TEST FALLITO: Verifica della registrazione delle richieste e scrittura su file\n");
        if (campiLetti != 12) {
            printf("  Errore di parsing della riga file: campi letti = %d\n", campiLetti);
        }
        printf("  Richiesta in memoria: codice=%d, luogo=%s, tipologia=%hd, descrizione=%s, data=%02d/%02d/%04d, ora=%d-%d, urgenza=%hd, stato=%hd, id_tecnico=%d\n",
               richiesta->codice, richiesta->luogo, richiesta->tipologia, richiesta->descrizione,
               richiesta->data.giorno, richiesta->data.mese, richiesta->data.anno,
               richiesta->oraInizio, richiesta->oraFine, richiesta->urgenza, richiesta->stato, richiesta->id_tecnico);
        printf("  Richiesta dal file: codice=%d, luogo=%s, tipologia=%hd, descrizione=%s, data=%02d/%02d/%04d, ora=%d-%d, urgenza=%hd, stato=%hd, id_tecnico=%d\n",
               richiestaDalFile.codice, richiestaDalFile.luogo, richiestaDalFile.tipologia, richiestaDalFile.descrizione,
               richiestaDalFile.data.giorno, richiestaDalFile.data.mese, richiestaDalFile.data.anno,
               richiestaDalFile.oraInizio, richiestaDalFile.oraFine, richiestaDalFile.urgenza, richiestaDalFile.stato, richiestaDalFile.id_tecnico);
    }

    if (backupContenuto != NULL) {
        scriviContenutoFile(percorsoFile, backupContenuto);
    }
    free(backupContenuto);
    free(richiesta);
}

static Tecnico *creaTecnicoTest(int id, const char *nome, int specializzazione) {
    Tecnico *t = malloc(sizeof(Tecnico));
    if (t == NULL) {
        printf("Errore: memoria non allocabile per il test\n");
        exit(1);
    }

    t->id = id;
    strncpy(t->nome, nome, sizeof(t->nome) - 1);
    t->nome[sizeof(t->nome) - 1] = '\0';
    t->specializzazione = specializzazione;
    t->num_ore_lavorate = 0;
    t->next = NULL;

    return t;
}

void test_registrazione_tecnici(void) {
    Tecnico *tecnico = creaTecnicoTest(
        200001,
        "Mario Rossi",
        2
    );

    const char *percorsoFile = "Liste/Tecnico.txt";
    char *backupContenuto = leggiContenutoFile(percorsoFile);

    FILE *fp = fopen(percorsoFile, "w");
    if (fp == NULL) {
        printf("TEST FALLITO: Impossibile aprire il file %s\n", percorsoFile);
        free(tecnico);
        return;
    }

    fprintf(fp, "%s-%d-%d\n", tecnico->nome, tecnico->id, tecnico->specializzazione);
    fclose(fp);

    fp = fopen(percorsoFile, "r");
    if (fp == NULL) {
        printf("TEST FALLITO: Impossibile riaprire il file %s\n", percorsoFile);
        if (backupContenuto != NULL) {
            scriviContenutoFile(percorsoFile, backupContenuto);
        }
        free(backupContenuto);
        free(tecnico);
        return;
    }

    char linea[256];
    if (fgets(linea, sizeof(linea), fp) == NULL) {
        printf("TEST FALLITO: File %s vuoto o non leggibile\n", percorsoFile);
        fclose(fp);
        if (backupContenuto != NULL) {
            scriviContenutoFile(percorsoFile, backupContenuto);
        }
        free(backupContenuto);
        free(tecnico);
        return;
    }
    fclose(fp);

    Tecnico tecnicoDalFile;
    int campiLetti = sscanf(linea, "%50[^-]-%d-%d",
                            tecnicoDalFile.nome,
                            &tecnicoDalFile.id,
                            &tecnicoDalFile.specializzazione);

    int passed = 1;
    passed &= (campiLetti == 3);
    passed &= (strcmp(tecnicoDalFile.nome, tecnico->nome) == 0);
    passed &= (tecnicoDalFile.id == tecnico->id);
    passed &= (tecnicoDalFile.specializzazione == tecnico->specializzazione);

    if (passed) {
        printf("TEST PASSATO: Test della registrazione dei tecnici\n");
    } else {
        printf("TEST FALLITO: Test della registrazione dei tecnici\n");
        if (campiLetti != 3) {
            printf("  Errore di parsing della riga file: campi letti = %d\n", campiLetti);
        }
        printf("  Tecnico in memoria: nome=%s, id=%d, specializzazione=%d\n",
               tecnico->nome, tecnico->id, tecnico->specializzazione);
        printf("  Tecnico dal file: nome=%s, id=%d, specializzazione=%d\n",
               tecnicoDalFile.nome, tecnicoDalFile.id, tecnicoDalFile.specializzazione);
    }

    if (backupContenuto != NULL) {
        scriviContenutoFile(percorsoFile, backupContenuto);
    }
    free(backupContenuto);
    free(tecnico);
}
