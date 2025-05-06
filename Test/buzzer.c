#include <msp430.h>

int main(void){

    P5DIR |= BIT0;
    P5OUT &= ~BIT0;

    PM5CTL0 &= ~LOCKLPM5;

    while(1) {
        P5OUT ^= BIT0;
        __delay_cycles(50000);
    }
}