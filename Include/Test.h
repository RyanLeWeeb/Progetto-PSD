#ifndef TEST_H
#define TEST_H
#include "Tecnico.h"
#include "Richiesta.h"

void test_verifica_registrazione_richiesta(Richiesta **lista);

void test_verifica_registrazione_tecnico(Tecnico **lista);

void test_verifica_assegnazione_tecnico_e_aggiornamento_stato(Richiesta **listaRichieste, Tecnico **listaTecnici);

#endif
