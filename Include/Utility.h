#ifndef UTILITY_H
#define UTILITY_H
#include <stdbool.h>

typedef struct {
    int giorno;
    int mese;
    int anno;
} Data;

short confrontaDate(Data d1, Data d2);

#endif