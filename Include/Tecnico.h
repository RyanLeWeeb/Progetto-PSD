#ifndef TECNICO_H
#define TECNICO_H
#include "Utility.h"

typedef struct Tecnico {
    int id;
    char nome[51];
    int specializzazione;
    struct Tecnico *next;
} Tecnico;

// Crea una lista di tecnici leggendo i dati da un file
Tecnico* creaListaTecnico();

// Aggiorna il file con i dati della lista di tecnici
void aggiornaListaTecnico(Tecnico *listaTecnico);

#endif