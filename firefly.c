/*
 * ATtiny13a LED Firefly
 * File: firefly.c
 *
 * 
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// defines lights[]
#include "photinus_consimilis.h"

unsigned int c=0; // 16 bit
ISR(WDT_vect) {
    // set PORT B to whatever the lookup table says
    unsigned char ff1_idx = c % FF1_ITERATIONS;
    unsigned char ff2_idx = c % FF2_ITERATIONS;
    PORTB = (lights[ff1_idx] & FF1_MASK) | (lights[ff2_idx] & FF2_MASK);

    // increment loop counter; reset if we've reached the LCM
    if (++c==COMBINED_ITERATIONS)
        c=0;
}

int main(void)
{
    // Setup

    // Power Saving cf. 7.4 p.32
    //
    // turn off ADC
    ADCSRA &= ~(1<<ADEN);
    //
    // turn off Analog Comparator
    ACSR &= ~(1<<ACD);
    //
    // turn off Brownout Detector
    // not necessary; off by default
    //
    // turn off Internal Voltage Reference
    // not necessary; ADC is off & BOD is off
    //
    // turn off all digital inputs Port Pins
    DIDR0 = 
        (1<<AIN0D) |
        (1<<AIN1D) |
        (1<<ADC0D) |
        (1<<ADC1D) |
        (1<<ADC2D) |
        (1<<ADC3D);


    // Set up Port B pin 0,1,3,4 mode to output
    DDRB = 1<<DDB0 | 1<<DDB1 | 1<<DDB3 | 1<<DDB4;

    // Set up Port B data to be all low
    PORTB = 0;  

    // set wdt prescaler
    WDTCR = (1<<WDP2);   // 32k ~.25s  8.5.2 p.43

    // Enable watchdog timer interrupts
    WDTCR |= (1<<WDTIE);

    sei(); // Enable global interrupts 

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    while (1) {
        sleep_mode();
    }

    return 0;
}

