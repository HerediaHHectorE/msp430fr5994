//******************************************************************************
//  Example No. 2: Prueba del Timer y UART
//
//  Presenta: Hector Enrique Heredia
//  Estudiante de MIE | hectorherediahernandez@gmail.com
//  Pontificia Universidad Javeriana  | Sede Bogotá
//  Bogota D.C. | Colombia
//
//******************************************************************************
#include <msp430.h>
#include "cs.h"
#include "timer.h"
#include "general.h"
#include "gpio.h"

#include "uart.h"

#define DELAY_CYCLES(x) __delay_cycles(x)

/* TIMERS Y  TIMEOUTS */
#define NTIMERS 1           // Tiempo x Dato en el display 160 Hz
#define NTIMEOUTS 3         // Comando 0x23 5 seg. Disminuye intensidad.
                            // Comando 0x25 2 seg. Apaga el display
                            // 10 seg. sin recibir datos, pone -- en el display

/* FLAGS */
unsigned char flags=0x00;

/* Tiempo */
Tm_Control TmControl;

static Tm_Period Timers[NTIMERS];
static Tm_Timeout Touts[NTIMEOUTS];

/* UART RX y TX */
char dataRx;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    DELAY_CYCLES(100000);


    P1OUT &= ~(BIT0|BIT2|BIT3|BIT4);                         // Clear P1.0 output latch for a defined power-on state
    P1DIR |= (BIT0|BIT2|BIT3|BIT4);                          // Set P1.0 to output direction
	P1OUT |= (BIT0|BIT2|BIT3|BIT4);

    /* INICIALIZACIÓN */
    CSInit();
    UartInit();
    TimerInit();

    ArrayTimersInit(&TmControl, Timers, NTIMERS, Touts, NTIMEOUTS);
	
    while(!(UCA3IFG & UCRXIFG));             // Para que me espere mientras cargo el archivo...
    TimerStart(&TmControl, 160, 0);            // Timer 1/20 s
    TimeOutStart(&TmControl, 320, 0);         // TimeOut 1 seg. sin recibir datos.
	TimeOutStart(&TmControl, 800, 1);		 // TimeOut 10 seg.
	TimeOutStart(&TmControl, 1600, 2);		 // TimeOut 1 Min

    while (1){

       if(AttendTimer()){
          TimerProcess(&TmControl);
       }

       /* CONTROL DE CARACTERES */
       /* Saca un elemento de la cola y valida si este es uno de los casos 0x23 al 0x26.
        * En caso de que sea uno de estos, activa la bandera correspondiente, pero no
        * limpia la bandera de periodo, ya que este caracter no será impreso en el display
        * Cuando llega el caracter de la pila no es un caracter de control, limpia la bandera
        */
      if(Period_Flag(&TmControl,0)){
		   P1OUT ^= BIT0;
		   UCA3TXBUF = 0x31;
		   Period_Clear(&TmControl,0);
	   }
		

       if(Timeout_Flag(&TmControl,0)){
           UCA3TXBUF = 0x41;
		   P1OUT ^= BIT2;
		   TimeOutStart(&TmControl, 320, 0);
	   }
	   
	   if(Timeout_Flag(&TmControl,1)){
	       UCA3TXBUF = 0x42;
		   P1OUT ^= BIT3;
		   TimeOutStart(&TmControl, 800, 1);
	   }
	   
	   if(Timeout_Flag(&TmControl,2)){
	       UCA3TXBUF = 0x43;
		   P1OUT ^= BIT4;
		   TimeOutStart(&TmControl, 1600, 2);
	   }

    }

}

