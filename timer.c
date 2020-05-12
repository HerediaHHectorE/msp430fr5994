#include <msp430.h>
#include "timer.h"

void TimerInit(){

    TA0CCR0 = 6250;
    TA0CTL = TASSEL__SMCLK | MC__UP;     // SMCLK, continuous mode

}

unsigned char AttendTimer(){
  if(!(TA0CCTL0 & CCIFG)){
    return NO;
  }
    TA0CCTL0 &= ~CCIFG;
    return YES;
}

void ArrayTimersInit(Tm_Control *tcp, Tm_Period *t_period, Tm_Qty nts, Tm_Timeout *t_timeouts, Tm_Qty nto){

  /* Variables internas */
  Tm_Qty n;
  Tm_Period  *tts;
  Tm_Timeout *tto;

  /* Paso de los valores ingresados como parámetros */
  tcp->t_period = t_period;
  tcp->nts = nts;

  tcp->t_timeouts = t_timeouts;
  tcp->nto = nto;

  /* Inicializa los periodos de la tabla tts apunta a tcp->t_period */
  for (n = nts, tts = t_period; n; --n, ++tts){
      tts->flags = 0;
      tts->count = tts->period = 0;
  };

  /* Inicializa los timeouts de la tabla tto apunta a tcp->t_timeouts */
  for (n = nto, tto = t_timeouts; n; --n, ++tto){
      (*tto) = 0;
  };

}


void TimerProcess(Tm_Control *tcp){

   /* Varables internas */
   Tm_Qty n;
   Tm_Period *tts;
   Tm_Timeout *tto;

   /* Revisa si hay TICK */
   //if (!(AttendTimer())) return;

   /* Si hay TICK decrementa los contadores de TIMERS y TIMEOUTS */
   /* Revisa la tabla de períodos y procesa los que estén activos */
   for (n = tcp->nts, tts = tcp->t_period; n; --n, ++tts){
      if (tts->flags & TM_PER_B_ACTIVO){        /* Valida que el timer este ACTIVO */
         --(tts->count);                        /* Decrementa el contador */
         if (!(tts->count)){                    /* Una vez es cero lo reinicia */
            tts->flags |= TM_PER_B_TC;
            tts->count = tts->period;
         }
      }
   }
   /* Revisa la tabla de timeouts y procesa los que estén activos */
   for (n = tcp->nto, tto = tcp->t_timeouts; n; --n, ++tto){
      if (*tto) --(*tto);
   }
 }


void TimerStart(Tm_Control *tcp, unsigned int ntick, unsigned char tnum){

  /* Varables internas */
  Tm_Period *tts;

  tts = tcp->t_period + tnum;                  //Paso por valor, Si uso &(tpc->t_period[tnum]) tocaría luego ajustar la linea 3 porque tendría la dirección
  tts->flags |= TM_PER_B_ACTIVO;
  tts->period = tts->count = ntick;

}

void TimeOutStart(Tm_Control *tcp, unsigned int ntick, unsigned char tnum){

  tcp->t_timeouts[tnum] = ntick;

}
