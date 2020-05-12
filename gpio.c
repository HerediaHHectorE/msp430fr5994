#include <msp430.h>
#include "gpio.h"

void GpioInit(){

  /* Puerto Paralelo 1 */
  P3OUT = 0;
  P3DIR = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;

}
