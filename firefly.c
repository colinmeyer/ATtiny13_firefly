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

#define COMBINED_ITERATIONS 1155     // number of iterations in a cycle 
#define FF1_ITERATIONS 33
#define FF2_ITERATIONS 35

#define FF1_MALE   PB3   // pin 2
#define FF1_FEMALE PB4   // pin 3
#define FF2_MALE   PB0   // pin 5
#define FF2_FEMALE PB1   // pin 6

#define FF1_MASK (1<<FF1_MALE | 1<<FF1_FEMALE)
#define FF2_MASK (1<<FF2_MALE | 1<<FF2_FEMALE)

// lookup table for lights status at each iteration
const unsigned char lights[] = {
    1<<FF1_MALE | 0<<FF1_FEMALE | 1<<FF2_MALE | 0<<FF2_FEMALE ,  //  0
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  1
    1<<FF1_MALE | 0<<FF1_FEMALE | 1<<FF2_MALE | 0<<FF2_FEMALE ,  //  2
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  3
    1<<FF1_MALE | 0<<FF1_FEMALE | 1<<FF2_MALE | 0<<FF2_FEMALE ,  //  4
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  5
    1<<FF1_MALE | 0<<FF1_FEMALE | 1<<FF2_MALE | 0<<FF2_FEMALE ,  //  6
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  7
    1<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  8
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  9
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  10
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  11
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  12
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  13
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  14
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  15
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  16
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  17
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 1<<FF2_FEMALE ,  //  18
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  19
    0<<FF1_MALE | 1<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  20
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  21
    0<<FF1_MALE | 1<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  22
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  23
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  24
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  25
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  26
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  27
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  28
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  29
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  30
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  31
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  32
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  33
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  34
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  35
};


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

