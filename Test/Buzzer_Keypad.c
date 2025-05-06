#include <msp430.h>

char key;
unsigned int distance_round = 6;

char readKeypad() {
    if (!(P1IN & BIT4)) { // 'A'
        P6OUT = 0x00;
        return 'A';
    } else if (!(P1IN & BIT5)) { // 'B'
        P6OUT = 0x01;
        return 'B';
    } else if (!(P1IN & BIT6)) { // 'C'
        P6OUT = 0x02; 
        return 'C';
    } else if (!(P1IN & BIT7)) { // 'D'
        P6OUT = 0x04;    
        return 'D';
    } else {
        P6OUT = P6OUT;
        return key;
    }
}

int main(void){

    WDTCTL = WDTPW | WDTHOLD;

    P5DIR |= BIT0 | BIT3;
    P5OUT &= ~BIT0;

    // Keypad rows
    P1DIR &= ~(BIT4 | BIT5 | BIT6 | BIT7);
    P1REN |= BIT4 | BIT5 | BIT6 | BIT7;
    P1OUT |= BIT4 | BIT5 | BIT6 | BIT7;
    P5OUT &= ~BIT3;             // Column low

    PM5CTL0 &= ~LOCKLPM5;

    TB0CTL |= TBCLR | TBSSEL__SMCLK | MC__UP;
    TB0CCR0 = 32768;

    TB0CCTL0 &= ~CCIFG;
    TB0CCTL0 |= CCIE;
    __enable_interrupt();

    while(1) {
        key = readKeypad();
        __delay_cycles(10000);
    }

}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void ISR_TB0_CCR0(void) {
    switch (key) {
        case 'A':
            P5OUT &= ~BIT0;    // Turn off LED
            break;
        case 'B':
            P5OUT = (distance_round < 5) ? (P5OUT | BIT0) : (P5OUT & ~BIT0);
            break;
        case 'C':
            P5OUT = (distance_round > 5) ? (P5OUT | BIT0) : (P5OUT & ~BIT0);
            break;
        case 'D':
            P5OUT ^= BIT0;     // Toggle LED
            break;
        default:
            break; 
    }
    TB0CCTL0 &= ~CCIFG;
}