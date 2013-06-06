/*
 * ATtiny13a LED Firefly
 * File: firefly.c
 *
 * 
 *
 */

#include <avr/io.h>
#define F_CPU 9.6E6L /* CPU Freq. Must come before delay.h include. 9.6MHz */
#include <util/delay.h>

const int msecsDelayPost = 125;      // delay between iterations

#define COMBINED_ITERATIONS 60    // number of iterations in a cycle 
#define FF1_ITERATIONS 10
#define FF2_ITERATIONS 12

#define FF1_MALE   PB3   // pin 2
#define FF1_FEMALE PB4   // pin 3
#define FF2_MALE   PB0   // pin 5
#define FF2_FEMALE PB1   // pin 6

#define FF1_MASK (1<<FF1_MALE | 1<<FF1_FEMALE)
#define FF2_MASK (1<<FF2_MALE | 1<<FF2_FEMALE)

// lookup table for lights status at each iteration
const unsigned char lights[] = {
    1<<FF1_MALE | 0<<FF1_FEMALE | 1<<FF2_MALE | 0<<FF2_FEMALE ,  //  0
    1<<FF1_MALE | 0<<FF1_FEMALE | 1<<FF2_MALE | 0<<FF2_FEMALE ,  //  1
    0<<FF1_MALE | 0<<FF1_FEMALE | 1<<FF2_MALE | 0<<FF2_FEMALE ,  //  2
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  3
    0<<FF1_MALE | 1<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  4
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 1<<FF2_FEMALE ,  //  5
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 1<<FF2_FEMALE ,  //  6
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  7
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  8
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  9
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  10
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  11
    0<<FF1_MALE | 0<<FF1_FEMALE | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  12
};

int main(void)
{

    // Set up Port B pin 3,4 mode to output
    DDRB = 1<<DDB0 | 1<<DDB1 | 1<<DDB3 | 1<<DDB4;

    // Set up Port B data to be all low
    PORTB = 0;  

    while (1) {
        unsigned char c;
        for ( c = 0; c < COMBINED_ITERATIONS; c++ ) {
            // set PORT B to whatever the lookup table says
            unsigned char ff1_idx = c % FF1_ITERATIONS;
            unsigned char ff2_idx = c % FF2_ITERATIONS;
            PORTB = (lights[ff1_idx] & FF1_MASK) | (lights[ff2_idx] & FF2_MASK);

            // Pause a little while
            _delay_ms (msecsDelayPost);
        }
    }

    return 0;
}

