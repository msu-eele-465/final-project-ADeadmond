#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

char printdist[] = "Distance to the nearest object is: ";
char centimeter[] = " cm";
char dot[] = ".";
char zerro[] = "0";
char newline[] = " \r\n";

volatile int temp[2];
volatile float diff;
volatile unsigned int i=0;
int dst_int;
int dst_flt;
float tmp_flt;
char dst_char[5];
char dst_flt_char[5];
volatile float distance;

void ser_output(char *str);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog

    // Set P1.6 (Trigger) as output
    P1DIR |= BIT6;
    P1OUT &= ~BIT6;

    // Set P2.1 (Echo) as input with Timer_A capture
    P2DIR &= ~BIT1;
    P2SEL0 |= BIT1;
    P2SEL1 &= ~BIT1;

    // UART config on P1.4 (TX) and P1.5 (RX)
    P1SEL0 |= BIT4 | BIT5;
    P1SEL1 &= ~(BIT4 | BIT5);

    // Hold USCI in reset, configure, then release
    UCA0CTLW0 |= UCSWRST;
    UCA0CTLW0 |= UCSSEL__SMCLK;
    UCA0BR0 = 6;                  // 1MHz/9600 = ~104
    UCA0BR1 = 0;
    UCA0MCTLW = UCBRS5 << 8;
    UCA0CTLW0 &= ~UCSWRST;

    // TimerB0 for PWM (Trigger pin)
    TB0CTL = TBSSEL__SMCLK | MC__UP;
    TB0CCR0 = 0xFFFF;
    TB0CCR1 = 0x000A;
    TB0CCTL1 = OUTMOD_7;

    // TimerB1 for Echo capture
    TB1CTL = TBSSEL__SMCLK | MC__CONTINUOUS;
    TB1CCTL1 = CM_3 | CCIS_0 | CAP | CCIE | SCS;

    __bis_SR_register(GIE); // Enable global interrupts

    while (1)
    {
        distance = diff / 58;
        dst_int = floor(distance);
        tmp_flt = distance - dst_int;

        ltoa(dst_int, dst_char, 10);

        if (tmp_flt < 0.01)
        {
            dst_flt = floor(tmp_flt * 1000);
            ltoa(dst_flt, dst_flt_char, 10);
            ser_output(printdist);
            ser_output(dst_char);
            ser_output(dot);
            ser_output(zerro);
            ser_output(zerro);
            ser_output(dst_flt_char);
            ser_output(centimeter);
        }
        else if (tmp_flt < 0.1)
        {
            dst_flt = floor(tmp_flt * 100);
            ltoa(dst_flt, dst_flt_char, 10);
            ser_output(printdist);
            ser_output(dst_char);
            ser_output(dot);
            ser_output(zerro);
            ser_output(dst_flt_char);
            ser_output(centimeter);
        }
        else
        {
            dst_flt = floor(tmp_flt * 100);
            ltoa(dst_flt, dst_flt_char, 10);
            ser_output(printdist);
            ser_output(dst_char);
            ser_output(dot);
            ser_output(dst_flt_char);
            ser_output(centimeter);
        }
        ser_output(newline);

        __delay_cycles(500000);
    }
}

// Timer1_B1 ISR
#pragma vector = TIMER1_B1_VECTOR
__interrupt void Timer1_B1_ISR(void)
{
    temp[i] = TB1CCR1;
    i += 1;
    TB1CCTL1 &= ~CCIFG;
    if (i == 2)
    {
        diff = temp[1] - temp[0];
        i = 0;
    }
}

// Serial output function
void ser_output(char *str)
{
    while (*str != 0)
    {
        while (!(UCB0IFG & UCTXIFG));
        UCB0TXBUF = *str++;
    }
}
