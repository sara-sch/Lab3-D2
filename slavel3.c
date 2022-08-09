/* 
 * File:   slavel3.c
 * Author: saras
 *
 * Created on August 5, 2022, 11:10 AM
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

#include <xc.h>
#include <stdint.h>
#include <stdio.h>

#include "spi.h"
#include "oscilador_1.h"
#include "tmr0.h"

void setup(void);


void __interrupt() isr (void){
    if (PIR1bits.SSPIF){

        PORTD = spiRead();      
        spiWrite(PORTD << 1);
    }
    PIR1bits.SSPIF = 0;             // Limpiamos bandera de interrupción
    return;
}

void main(void) {
    setup();
    
    while(1){}
}

void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0b00100000;
    PORTA = 0;
    
    TRISD = 0;
    PORTD = 0; 
    
    TRISC = 0b000011000;
    PORTC = 0;
    
    int_osc_MHz(4);     // Osc a 4 MHz
    tmr0_init (256);
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.SSPIE = 1;
    PIR1bits.SSPIF = 0; 
  
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_ACTIVE_2_IDLE);
    
}