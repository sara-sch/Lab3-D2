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
#include "adc_1.h"

void setup(void);

uint8_t flag = 0;
uint8_t send_pot1 = 0;
uint8_t send_pot2 = 0;
uint8_t adc = 0;
uint8_t pot1 = 0;
uint8_t pot2 = 0;

void __interrupt() isr (void){
   
        if(PIR1bits.SSPIF){                  // int. SPI
            if (flag == 1){              // enviamos pot1
                spiWrite(send_pot1);
                flag = 0;
            }
            else if (flag == 0){         // enviamos pot1
                spiWrite(send_pot2);
                flag = 1;
            }
            PIR1bits.SSPIF = 0;             // limpiamos bandera de interrupción
        }
        else if(PIR1bits.ADIF){
            if(adc == 0){
                pot1 = adc_read();          // leemos pot1
            }
            else if(adc == 1){
                pot2 = adc_read();          // leemos pot2
            }
        }
    
    return;
}

void main(void) {
    setup();
    while(1){
        if (adc == 0){                // canal 0
            adc_start(0);
            adc = 1;
        }
        else if (adc == 1){           // canal 1
            adc_start(1);
            adc = 0;
        }
        send_pot1 = pot1 & 0b11111110;     // indicador de potenciómetros
        send_pot2 = pot2 | 0b00000001;     
    }
}

void setup(void){
    ANSEL = 0b00000011;
    ANSELH = 0;
    TRISA = 0b00100000;
    PORTA = 0;
    
    TRISC = 0b000011000;
    PORTC = 0;
    
    int_osc_MHz(4);                         // Ooscilador a 4 MHz
    adc_init(1,0,0);
    tmr0_init (256);
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.SSPIE = 1;
    PIR1bits.SSPIF = 0; 
    PIR1bits.ADIF = 0;          
    PIE1bits.ADIE = 1;          
  
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_ACTIVE_2_IDLE);
    
}