/*
 * ATtiny13 LED Firefly
 * File: firefly.c
 *
 * 
 *
 */

#include <avr/io.h>
#define F_CPU 9.6E6L /* CPU Freq. Must come before delay.h include. 9.6MHz */
#include <util/delay.h>

const int msecsDelayPost = 125;      // delay between iterations
const unsigned char iterations = 12; // number of iterations in a cycle 
                                     // lookup table for lights status at each iteration
const unsigned char lights[] = {
    1<<PB3,
    1<<PB3,
    0,
    0,
    1<<PB4,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

int main(void)
{

    // Set up Port B pin 3,4 mode to output
    DDRB = 3<<DDB3;

    // Set up Port B data to be all low
    PORTB = 0;  

    while (1) {
        unsigned char c;
        for ( c = 0; c < iterations; c++ ) {
            // set PORT B to whatever the lookup table says
            PORTB = lights[c];

            // Pause a little while
            _delay_ms (msecsDelayPost);
        }
    }

    return 0;
}

