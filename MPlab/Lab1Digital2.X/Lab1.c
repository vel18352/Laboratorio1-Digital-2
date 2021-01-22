/*
 * File:   Lab1.c
 * Author: colum
 *
 * Created on 22 de enero de 2021, 08:12 AM
 */


// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
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

#define _XTAL_FREQ   8000000


// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>


void semaforo(void);
void setup(void);
void jugador1(void);
void jugador2(void);
void rst(void);
int bandera_inicio = 0;
/*int banderaP1 = 0;
int banderaP2 = 0;*/




void main(void) 
{
    setup();
    while (1)
    {
        if ((PORTBbits.RB0 == 1) && (!bandera_inicio))
        {
            semaforo();
        }
        
        if ((PORTBbits.RB1 == 1) && (bandera_inicio))
        {
            jugador1();
            __delay_ms(50);
        }
        if ((PORTBbits.RB2 == 1) && (bandera_inicio))
        {
            jugador2();
            __delay_ms(50);
        }
        if ((PORTBbits.RB3 == 1))
        {
            rst();
            __delay_ms(50);
        }
        PORTBbits.RB4 = bandera_inicio;
    }
}





void setup(void)
{
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    PORTA = 0;
    
    TRISB = 0b00001111;
    PORTB = 0;
    
    TRISC = 0;
    PORTC = 0;
    
    TRISE = 0;
    PORTE = 0;
    
}

void semaforo(void)
{
    PORTE = 1;
    __delay_ms(100);
    for (int i=0; i<=2; i++)
    {
     PORTE = PORTE << 1;
     __delay_ms(100);  
    }
    bandera_inicio = 1;
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
}

void jugador1(void)
{
    if (PORTA == 0)
    {
       PORTA = PORTA++; 
    }
    else
    {
        PORTA = PORTA << 1;
        if (PORTAbits.RA7 == 1)
        {
            PORTBbits.RB6 = 1;
            bandera_inicio= 0;
        }
        else
        {
            PORTBbits.RB6 = 0;
        }  
    }
}

void jugador2(void)
{
    if (PORTC == 0)
    {
       PORTC = PORTC++; 
    }
    else
    {
        PORTC = PORTC << 1;
        if (PORTCbits.RC7 == 1)
        {
            PORTBbits.RB7 = 1;
            bandera_inicio= 0;
        }
        else
        {
            PORTBbits.RB7 = 0;
        }
    }
}

void rst(void)
{
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTE = 0;
    bandera_inicio = 0;
}