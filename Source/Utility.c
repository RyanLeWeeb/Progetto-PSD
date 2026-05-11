#include <stdio.h>
#include "utility.h"

short sonoDateIdentiche(Data d1, Data d2){
    if(d1.anno == d2.anno && d1.mese == d2.mese && d1.giorno == d2.giorno) {
        return 1; // Date uguali
    } else{
        return 0; // Date diverse
    }
}