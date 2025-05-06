#include <msp430.h>
#include <math.h>
#include <stdio.h>

#define TRIG_PIN    BIT0    // P2.0
#define ECHO_PIN    BIT1    // P2.1
#define LED_PIN     BIT0    // P1.0

void delayMicroseconds(unsigned int us);
void delayMilliseconds(unsigned int ms);
int distance_round;

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

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    // Configure pins
    P2DIR |= TRIG_PIN;    // Trig as output
    P2DIR &= ~ECHO_PIN;   // Echo as input
    P1DIR |= LED_PIN;     // LED as output

    P3DIR |= 0xFF;             // Set all P3 pins as outputs (or adjust as needed)
    P3OUT = 0x00;              // Clear display initially

    PM5CTL0 &= ~LOCKLPM5;

    while (1)
    {
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