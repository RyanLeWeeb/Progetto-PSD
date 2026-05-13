#include <stdio.h>
#include <stdlib.h>
#include "Include/Richiesta.h"
#include "Include/Utility.h"
#include "Include/Tecnico.h"


int main() {
    int i;
    Richiesta *r =creaListaRichiesta();
    printf("Lista delle richieste:\n");
    scanf("%d", &i);
    return 0;
}