#include <msp430.h>
#include "uart.h"

#define B9600 104
#define B2400 416


void UartInit(){

    // Configure GPIO
    P6SEL1 &= ~(BIT0 | BIT1);
    P6SEL0 |= (BIT0 | BIT1);                // USCI_A3 UART operation

    // Configure USCI_A3 for UART mode
    UCA3CTLW0 = UCSWRST;                    // Put eUSCI in reset
    UCA3CTLW0 |= UCSSEL__SMCLK;             // CLK = SMCLK
    UCA3BRW = B9600;                        // B9600 o B2400
    UCA3CTLW0 &= ~UCSWRST;                  // Initialize eUSCI
}

unsigned char receive(char *data){

    if(!(UCA3IFG & UCRXIFG))
    return NO;
    *data = UCA3RXBUF;
    return YES;

}
