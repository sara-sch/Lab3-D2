#include <xc.h>
#include "tmr0.h"
#define _XTAL_FREQ 4000000

void tmr0_init (uint8_t prescaler){
    OPTION_REGbits.T0CS = 0;    // TMR0 con internal clock
    OPTION_REGbits.PSA = 0; //prescaler a TMR0
    switch(prescaler){ //escogemos prescaler
        case 2:
            OPTION_REGbits.PS = 0b000;
            break;
        case 4:
            OPTION_REGbits.PS = 0b001;
            break;
        case 8:
            OPTION_REGbits.PS = 0b010;
            break;
        case 16:
            OPTION_REGbits.PS = 0b011;
            break;
        case 32:
            OPTION_REGbits.PS = 0b100;
            break;
        case 64:
            OPTION_REGbits.PS = 0b101;
            break;
        case 128:
            OPTION_REGbits.PS = 0b110;
            break;
        case 256:
            OPTION_REGbits.PS = 0b111;
            break;
        default:
            break;
    }
    TMR0 = _tmr0_value; // cargamos valor
    INTCONbits.T0IF = 0; //limpiamos bandera
}

void tmr0_reload(void){
    INTCONbits.T0IF = 0;
    TMR0 = _tmr0_value;
}