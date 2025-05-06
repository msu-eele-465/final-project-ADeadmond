#include <msp430.h>
#include <math.h>
#include <stdbool.h>

#define TRIG_PIN    BIT0    // P2.0
#define ECHO_PIN    BIT1    // P2.1
#define LED_PIN     BIT0    // P1.0
#define US_PER_CM   29      // Microseconds per cm for sound
#define BUZZER_DIV  9       // Buzzer frequency divisor
#define BASE_FREQ   32768   // Base frequency for buzzer

const char SEGMENTS[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void delayMicroseconds(unsigned int us) {
    while (us--) __delay_cycles(1);
}

void delayMilliseconds(unsigned int ms) {
    while (ms--) __delay_cycles(1000);
}

void sev_display(unsigned int distance) {
    P3OUT = (distance < 10) ? SEGMENTS[distance] : 0x00;
}

char readKeypad() {
    if (!(P1IN & BIT4)) return 'A';
    if (!(P1IN & BIT5)) return 'B';
    if (!(P1IN & BIT6)) return 'C';
    if (!(P1IN & BIT7)) return 'D';
    return 0;
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    // Configure pins
    P2DIR |= TRIG_PIN;          // Trig output
    P2DIR &= ~ECHO_PIN;         // Echo input
    P1DIR |= LED_PIN;           // LED output
    P3DIR |= 0x7F;              // 7-segment output
    P5DIR |= BIT0 | BIT3;       // Buzzer and keypad column output
    
    // Keypad rows
    P1REN |= BIT4 | BIT5 | BIT6 | BIT7;
    P1OUT |= BIT4 | BIT5 | BIT6 | BIT7;
    P5OUT &= ~BIT3;             // Column low

    // Timer configuration
    TB0CTL = TBCLR | TBSSEL__SMCLK | MC__UP;
    TB0CCR0 = BASE_FREQ;
    TB0CCTL0 = CCIE;
    __enable_interrupt();

    while (1) {
        char key = readKeypad();
        unsigned int duration = 0;
        
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
        sev_display(distance);
        
        if (distance > 0) {
            TB0CCR0 = (int)roundf(BASE_FREQ * distance / BUZZER_DIV);
        }

        delayMilliseconds(100);
    }
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void ISR_TB0_CCR0(void) {
    static char lastKey = 0;
    char currentKey = lastKey;
    
    switch (currentKey) {
        case 'A': P5OUT &= ~BIT0; break;
        case 'B': P5OUT = (distance_round < 5) ? (P5OUT | BIT0) : (P5OUT & ~BIT0); break;
        case 'C': P5OUT = (distance_round > 5) ? (P5OUT | BIT0) : (P5OUT & ~BIT0); break;
        case 'D': P5OUT ^= BIT0; break;
    }
    TB0CCTL0 &= ~CCIFG;
    lastKey = currentKey;
}