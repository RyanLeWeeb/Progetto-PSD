#include <stdio.h>
#include "../Include/Utility.h"

short confrontaDate(Data d1, Data d2){
    if(d1.anno == d2.anno && d1.mese == d2.mese && d1.giorno == d2.giorno) {
        return 0; // Date uguali
    }
    return 1; // Date diverse
}

short sonoOrariSovrapposti(int inizio1, int fine1, int inizio2, int fine2){
    if (inizio1 < fine2 && inizio2 < fine1) {
        return 1; // Gli orari si sovrappongono
    } else {
        return 0; // Gli orari non si sovrappongono
    }
}