#include <msp430.h>
#include <math.h>
#include <stdbool.h>

#define TRIG_PIN    BIT0    // P2.0
#define ECHO_PIN    BIT1    // P2.1
#define US_PER_CM   29      // Microseconds per cm for sound
#define BUZZER_DIV  9       // Buzzer frequency divisor
#define BASE_FREQ   32768   // Base frequency for buzzer

volatile int distance_round;
char key;

void delayMicroseconds(unsigned int us) {
    while (us--) __delay_cycles(1);
}

void delayMilliseconds(unsigned int ms) {
    while (ms--) __delay_cycles(1000);
}

void sev_display(unsigned int distance) {
    switch(distance) {
        case 0:
            P3OUT = 0x00; // 00000000
            break;
        case 1:
            P3OUT = 0x01; // 00000001
            break;
        case 2:
            P3OUT = 0x03; // 00000011
            break;
        case 3:
            P3OUT = 0x07; // 00000111
            break;
        case 4:
            P3OUT = 0x0F; // 00001111
            break;
        case 5:
            P3OUT = 0x1F; // 00011111
            break;
        case 6:
            P3OUT = 0x3F; // 00111111
            break;
        case 7:
            P3OUT = 0x7F; // 01111111
            break;
        case 8:
            P3OUT = 0xFF; // 11111111
            break;
        case 9:
            P3OUT = 0xFF; // 11111111
            break;
        default:
            P3OUT = 0x00;
            break;
    }
}

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

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;

    // Configure pins
    P2DIR |= TRIG_PIN;          // Trig output
    P2DIR &= ~ECHO_PIN;         // Echo input
    P3DIR |= 0x7F;              // LED-Bar output
    P5DIR |= BIT0 | BIT3;       // Buzzer and keypad column output
    P6DIR |= BIT0 | BIT1 | BIT2;
    P6OUT = 0x00;
    
    // Keypad rows
    P1DIR &= ~(BIT4 | BIT5 | BIT6 | BIT7);
    P1REN |= BIT4 | BIT5 | BIT6 | BIT7;
    P1OUT |= BIT4 | BIT5 | BIT6 | BIT7;
    P5OUT &= ~BIT3;             // Column low

    PM5CTL0 &= ~LOCKLPM5;

    while (1) {
        key = readKeypad();
        unsigned int duration = 0;
        int prox_delay = 900;
        
        // Trigger pulse
        P2OUT &= ~TRIG_PIN;
        delayMicroseconds(2);
        P2OUT |= TRIG_PIN;
        delayMicroseconds(10);
        P2OUT &= ~TRIG_PIN;

        // Wait for echo
        while (!(P2IN & ECHO_PIN));
        while (P2IN & ECHO_PIN) {
            __delay_cycles(1);
            duration++;
        }

        int distance = (int)roundf(duration / (float)US_PER_CM);
        prox_delay = distance * 100;

        switch(key){
            case 'A':
                P3OUT = 0x00;
                break;
            default:
                sev_display(distance);
                break;
        }
        
        switch (key) {
            case 'A': P5OUT &= ~BIT0; break;
            case 'B': P5OUT = (distance_round < 5) ? (P5OUT | BIT0) : (P5OUT & ~BIT0); break;
            case 'C': P5OUT = (distance_round > 5) ? (P5OUT | BIT0) : (P5OUT & ~BIT0); break;
            case 'D': delayMilliseconds(prox_delay); P5OUT ^= BIT0; break;

        delayMilliseconds(100);
        }
    }
}
