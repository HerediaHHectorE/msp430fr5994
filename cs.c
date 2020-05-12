#include <msp430.h>
#include "cs.h"


void CSInit(){

    CSCTL0_H = CSKEY_H;                                      // Unlock CS registers
    CSCTL1 = DCOFSEL_0 | DCORSEL;                            // Set DCO to 1MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__2 | DIVS__1 | DIVM__1;                    // Set all dividers
    CSCTL0_H = 0;

}
