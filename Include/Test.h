#ifndef TEST_H
#define TEST_H
#include "Tecnico.h"
#include "Richiesta.h"

#define TEST_RICHIESTE_FILE "Test/Richieste.txt"
#define TEST_TECNICO_FILE "Test/Tecnici.txt"

void test_verifica_registrazione_richiesta(Richiesta **lista); // file generato: "Richieste.txt" nella cartella "Test"

void test_verifica_registrazione_tecnico(Tecnico **lista); // file generato: "Tecnici.txt" nella cartella "Test"

void test_verifica_assegnazione_tecnico_e_aggiornamento_stato(Richiesta **listaRichieste, Tecnico **listaTecnici);

void test_report(Richiesta **listaRichieste, Tecnico **listaTecnici); // file generato: "TC8_Report_Oracolo" nella cartella "Test"

void test_storico_interventi(Richiesta *listaRichieste); // file generato "TC7_Storico_Oracolo" nella cartella "Test"

#endif
