#include <msp430.h>
#include <math.h>
#include <stdio.h>

#define TRIG_PIN    BIT0    // P2.0
#define ECHO_PIN    BIT1    // P2.1
#define LED_PIN     BIT0    // P1.0

void delayMicroseconds(unsigned int us);
void delayMilliseconds(unsigned int ms);
int distance_round;
char key;

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

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    // Configure pins
    P2DIR |= TRIG_PIN;    // Trig as output
    P2DIR &= ~ECHO_PIN;   // Echo as input
    P1DIR |= LED_PIN;     // LED as output

    P3DIR |= 0xFF;             // Set all P3 pins as outputs (or adjust as needed)
    P3OUT = 0x00;              // Clear display initially

    P1DIR &= ~(BIT4 | BIT5 | BIT6 | BIT7);
    P1REN |= BIT4 | BIT5 | BIT6 | BIT7;
    P1OUT |= BIT4 | BIT5 | BIT6 | BIT7;
    P5DIR |= BIT3;
    P5OUT &= ~BIT3;             // Column low

    PM5CTL0 &= ~LOCKLPM5;

    while (1)
    {
        key = readKeypad();

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

        unsigned int duration = 0;
        float distance_in = 0;
        float echo_delay = 0;

        // Send 10us pulse to Trigger
        P2OUT &= ~TRIG_PIN;
        delayMicroseconds(2);
        P2OUT |= TRIG_PIN;
        delayMicroseconds(10);
        P2OUT &= ~TRIG_PIN;

        // Wait for Echo to go HIGH
        while ((P2IN & ECHO_PIN) == 0);

        // Measure how long Echo stays HIGH
        while ((P2IN & ECHO_PIN) != 0)
        {
            __delay_cycles(1);  // 1 cycle = 1 us at 1MHz
            duration++;
        }

        echo_delay = (float)duration;
        // Convert to centimeters
        distance_in = echo_delay / 29;
        distance_round = (int)roundf(distance_in);

        if ((distance_round >= 0) && (distance_round < 10)){
            sev_display(distance_round);
        } else {sev_display(0);}

        // Delay before next measurement
        delayMilliseconds(100);
    }
}

void delayMicroseconds(unsigned int us)
{
    while (us--)
        __delay_cycles(1);  // 1 cycle = 1 us at 1MHz
}

void delayMilliseconds(unsigned int ms)
{
    while (ms--)
        __delay_cycles(1000);  // 1000 cycles = 1 ms at 1MHz
}