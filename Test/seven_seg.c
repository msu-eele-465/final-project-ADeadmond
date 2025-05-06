#include "msp430fr2355.h"
#include <msp430.h>

unsigned int count = 9;

#include <msp430.h>

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


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;  // Disable watchdog timer
    PM5CTL0 &= ~LOCKLPM5;      // Unlock GPIO

    P3DIR |= 0xFF;             // Set all P3 pins as outputs (or adjust as needed)
    P3OUT = 0x00;              // Clear display initially

    while(1) {
        if(count > 0) {
            count--;
        } else {
            count = 9;
        }
        sev_display(count);
        __delay_cycles(100000);  // Adjust delay if needed
    }
}
