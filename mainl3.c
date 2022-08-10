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
#include "adc_1.h"
#include "lcd.h"

#define _XTAL_FREQ 4000000
#define FLAG_SPI 0x0F


uint8_t cont = 0;
uint8_t cont2 = 0;

void setup(void);

uint8_t valpot = 0;
uint8_t pot1 = 0;
uint8_t pot2 = 0;
uint8_t indicador = 0;
uint8_t entero1 = 0;
uint8_t dec1 = 0;
uint8_t entero2 = 0;
uint8_t dec2 = 0;
unsigned short volt1 = 0;
unsigned short volt2 = 0;
char s[];

unsigned short map(uint8_t val, uint8_t in_min, uint8_t in_max, 
            unsigned short out_min, unsigned short out_max);


void main(void) {

    setup();
    Lcd_Clear_8();
    Lcd_Set_Cursor_8(1,1); // primera fila
    Lcd_Write_String_8("  POT1    POT2");// escribir en la lcd
    
    
    while(1){
        spiWrite(FLAG_SPI); // valor para generar respuesta del slave
        spiReceiveWait(); // se espera envío

        PORTCbits.RC2 = 1;  // deshabilitamos slave
        __delay_ms(10);  
        PORTCbits.RC2 = 0;  // habilitamos slave
        __delay_ms(10);

        spiReceiveWait(); 
        valpot = spiRead();    // guardamos lo recibido
        PORTB = valpot;
        
        __delay_ms(10);
        
        indicador = 0b00000001 & valpot;
        if (indicador == 1){
            pot1 = valpot;       //Guardamos pot1 
            
            volt1 = map(pot1, 1, 255, 0, 500); // Enteros y decimales de pot1
            entero1 = volt1/100;
            dec1 = volt1-entero1*100;
        }
        else if (indicador == 0) {
            pot2 = valpot;      //Guardamos pot2 
            
            volt2 = map(pot2, 1, 255, 0, 500);// Enteros y decimales de pot2
            entero2 = volt2/100;
            dec2 = volt2-entero2*100;
        }

        sprintf(s, "  %d.%d    %d.%d ", entero1, dec1, entero2, dec2); // Cadena de valores
        Lcd_Set_Cursor_8(2,1); // segunda fila
        Lcd_Write_String_8(s); // escribir en la lcd

    }
    
}

void setup(void){
    ANSEL = 0;
    
    TRISD = 0;
    PORTD = 0;
    
    TRISC = 0b00010001;
    PORTC = 0;
    
    TRISE = 0;
    PORTE = 0;
    
    int_osc_MHz(4);
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE); 
    Lcd_Init_8();
}



unsigned short map(uint8_t x, uint8_t x0, uint8_t x1,
            unsigned short y0, unsigned short y1){
    return (unsigned short)(y0+((float)(y1-y0)/(x1-x0))*(x-x0));
}