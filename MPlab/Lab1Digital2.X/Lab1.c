/*
 * File:   Lab1.c
 * Author: Emilio Velasquez 18352
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
void rst(void);             //Se hizo un prototipo de las funciones, se declaro y mas abajo se definio
int bandera_inicio = 0;
/*int banderaP1 = 0;
int banderaP2 = 0;*/




void main(void) 
{
    setup();           // Se creo el loop donde se ejecutaran las instrucciones
    while (1)
    {
        if ((PORTBbits.RB0 == 1) && (!bandera_inicio))      // al presionar boton 1 y tener la bandera de inicio desactivada
        {                                                   // llama la funcion de semaforo para iniciar la cuenta regresiva
            semaforo();
        }
        
        if ((PORTBbits.RB1 == 1) && (bandera_inicio))      //al presionar boton 2 y tener la bandera de inicio activada
        {                                                  //llama la funcion de jugador 1 y empieza a aumentar las leds
            jugador1();
            __delay_ms(50);                                //se hace un delay de 50ms para evitar el rebote
        }
        if ((PORTBbits.RB2 == 1) && (bandera_inicio))     //al presionar boton 3 y tener la bandera de inicio activada
        {                                                 //llama la funcion de jugador 2 y empieza a aumentar las leds
            jugador2();
            __delay_ms(50);                               //se hace un delay de 50ms para evitar el rebote
        }
        if ((PORTBbits.RB3 == 1))                        //al presionar boton 4 llama la funcion de reset y reinicia el juego 
        {                                                
            rst();
            __delay_ms(50);                              //se hace un delay de 50ms para evitar el rebote
        }
    }
}





void setup(void)                                        
{
    ANSEL = 0;                  //se establece que los puertos seran analogos
    ANSELH = 0;
        
    TRISA = 0;                  //establecemos puertos en A como salida y se limpian
    PORTA = 0;
    
    TRISB = 0b00001111;         //establecemos los primeros 3 como entrada y el resto como salida en puerto B y se limpian
    PORTB = 0;
    
    TRISC = 0;                  //establecemos puertos en C como salida y se limpian
    PORTC = 0;
    
    TRISE = 0;                  //establecemos puertos en E como salida y se limpian
    PORTE = 0;
    
}

void semaforo(void)             //se define la funcion de semforo de partida
{
    PORTE = 1;                  //se enciende la primera luz de partida y se espera 100ms
    __delay_ms(100);
    for (int i=0; i<=2; i++)    //un for que hace 2 repeticiones
    {
     PORTE = PORTE << 1;        //se corre el bit 1 posicion a la derecha y se hace un delay de 100ms
     __delay_ms(100);  
    }
    bandera_inicio = 1;         //se hace verdadera la bandera de que inicio la carrera y se limpian contadores al reiniciar la partida
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
}

void jugador1(void)             //se define la funcion de jugador 1 de la partida
{
    if (PORTA == 0)             //se crea un if en el cual al comenzar en 0 y presionar el boton dara la primera vuelta 
    {
       PORTA = PORTA++; 
    }
    else                        //al estar iniciado ya no sumara uno
    {
        PORTA = PORTA << 1;     //sino desplazara el bit a la derecha
        if (PORTAbits.RA7 == 1)//si el bit numero 7 esta encendido significa que gano 
        {
            PORTBbits.RB6 = 1; //encendera la led de ganador numero 1
            bandera_inicio= 0; //apaga la bandera de inicio para que el jugador 2 no siga y perdio
        }
        else
        {
            PORTBbits.RB6 = 0; //de lo contrario no enciende la bandera de ganador jugador 1
        }  
    }
}

void jugador2(void)             //se define la funcion de jugador 2 de la partida
{
    if (PORTC == 0)             //se crea un if en el cual al comenzar en 0 y presionar el boton dara la primera vuelta 
    {
       PORTC = PORTC++; 
    }   
    else                        //al estar iniciado ya no sumara uno
    {       
        PORTC = PORTC << 1;     //sino desplazara el bit a la derecha
        if (PORTCbits.RC7 == 1)//si el bit numero 7 esta encendido significa que gano 
        {
            PORTBbits.RB7 = 1; //encendera la led de ganador numero 1
            bandera_inicio= 0; //apaga la bandera de inicio para que el jugador 1 no siga y perdio
        }
        else
        {
            PORTBbits.RB7 = 0; //de lo contrario no enciende la bandera de ganador jugador 2
        }
    }
}

void rst(void)                  //se define la funcion de reset y limpia todos los puertos y resetea bandera
{
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTE = 0;
    bandera_inicio = 0;
}