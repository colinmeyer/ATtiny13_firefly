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
// #include "photinus_pyralis.h"

unsigned int c=0; // 16 bit
ISR(WDT_vect) {
    if (mode==0) {
        // signal mode 0, FF1_FEMALE light on
        DDRB  |= FF1_FEMALE_DD;
        PORTB |= FF1_FEMALE;

        if (dark_out()) {
            // switch to firefly
            mode=1;

            // turn off mode0 light
            PORTB &= ~(FF1_FEMALE);

            // adjust WDT prescaler
            WDTCR = (WDTCR & 0xd8)     // 0xd8 == 0b11011000 => mask out prescaler bits
                | (1<<WDP2)|(1<<WDP1); // 1s //(1<<WDP3);   // 10M ~8s  8.5.2 p.43
                //| (1<<WDP2);       // 32k ~.25 8.5.2 p.43
        }
    }

    if (mode==1) {
        // set output pins
        DDRB  |= FF1_FEMALE_DD | FF1_MALE_DD;

        // mode1 light
        PORTB |= FF1_MALE;

        if (++c==FF1_ITERATIONS) {
            // change to light meter
            mode=0;
            c=0;

            PORTB &= ~(FF1_MALE | FF1_FEMALE); // shut off all lights
            DDRB  &= ~(FF1_MALE_DD | FF1_FEMALE_DD); // shut off outputs

            // adjust WDT prescaler
            WDTCR = (WDTCR & 0xd8)     // 0xd8 == 0b11011000 => mask out prescaler bits
                | (1<<WDP2)|(1<<WDP1); // 1s //(1<<WDP3);   // 10M ~8s  8.5.2 p.43

        }
    }
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

