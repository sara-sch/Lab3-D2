/* 
 * File:   mainl3.c
 * Author: saras
 *
 * Created on August 5, 2022, 10:46 AM
 */

// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <stdio.h>
#include <stdlib.h>

#include <xc.h>
#include "spi.h"
#include "tmr0.h"
#include "oscilador_1.h"

#define _XTAL_FREQ 4000000

uint8_t cont = 0;
uint8_t cont2 = 0;

void setup(void);

void __interrupt() isr (void){      // Int. tmr0
    if(INTCONbits.T0IF){
        cont++;
        if (cont == 40){
            cont = 0; 
            cont2++; 
        }
        PORTB = cont2; 
    }
    tmr0_reload();
    return;
}

void main(void) {
    setup();
    
    while (1){      
       
        PORTCbits.RC2 = 1;
        __delay_ms(10);
        PORTCbits.RC2 = 0;
        __delay_ms(1000);
        spiWrite(cont2);
        PORTD = spiRead();
          
 
    }
              
}

void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISD = 0; 
    PORTD = 0; 
    
    TRISB = 0; 
    PORTB = 0;
    
    TRISC = 0b00010000;
    PORTC = 0;
    
    INTCONbits.T0IE = 1;        // Habiltamos interrupciones
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    int_osc_MHz (4);           // Oscilador en 4 MHz 
    tmr0_init (256);              
    spiInit(SPI_MASTER_OSC_DIV4,SPI_DATA_SAMPLE_MIDDLE,SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}