#include <xc.h>
#include "oscilador_1.h"

void int_osc_MHz(uint8_t freq){
    OSCCONbits.SCS = 1;                 //Oscilador sin salida de reloj
    switch(freq){                        // Switch case para indicar frecuencia de oscilador
        case 1:
            OSCCONbits.IRCF = 0b100;
            break;
        case 2:
            OSCCONbits.IRCF = 0b101;
            break;
        case 4:
            OSCCONbits.IRCF = 0b110;
            break;
        case 8:
            OSCCONbits.IRCF = 0b111;
            break;
        default:
            OSCCONbits.IRCF = 0b111;
            break;
    } 
    
}

