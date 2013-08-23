/*
 * ATtiny13a LED Firefly
 * File: firefly.c
 *
 * 
 *
 */

#include <avr/eeprom.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define F_CPU 9.6E6L/8 // CPU Freq. Must come before delay.h include. 9.6MHz
                       // by default, sysclock is /8
#include <util/delay.h>

#define POWER_DELAY          1    // ms

#define FF1_MALE   PB3   // pin 2
#define FF1_MALE_  PB4   // pin 3
#define FF1_FEMALE PB1   // pin 6

//                 ---------                       
//              1--|.   AT |--8  Vcc       
//         PB3  2--|  tiny |--7            
//  ADC2 / PB4  3--|   13a |--6  PB1       
//         GND  4--|       |--5            
//                 ---------               

#define FF1_MASK (1<<FF1_MALE | 1<<FF1_FEMALE)


// light returns count of ADC samples for LED capacitor to reach zero
//   count < 30 means light out
uint8_t light_detect() {
    // read ADC2 input
    ADMUX = 2;              // Select channel ADC = 2
    ADCSRA |= (1<<ADEN)     // turn on ADC
        | (1<<ADPS0)        // set prescaler to sysclck/128 for greater accuracy
        | (1<<ADPS1)        // cf. 14.5 p.84 & 14.12.2 p.94
        | (1<<ADPS2);

    // throw away
    ADCSRA |= (1 << ADSC);        // start single conversion
    while (ADCSRA & (1 << ADSC)); // wait until conversion is done

    uint8_t   adcl;
    uint8_t   adch;

    // charge detector LED briefly (it is a capacitor)
    DDRB  =  (1<<DDB4); // select pin 3 for output
    PORTB =  (1<<FF1_MALE_ ); // set pin to high
    _delay_ms(POWER_DELAY); // charge for some time

    DDRB  = 0;           // select pin 3 for tri-state
                         // currently, pin is still tied to pull-up resistor
                         // cf.  10.2.3 p.51
    PORTB &= ~(1<<FF1_MALE_ ); // set pin to low (switch pull-up resistor off)
                         // now pin is tri-state

    uint8_t c = 0;
    do {
        c++;
        ADCSRA |= (1 << ADSC);        // start single conversion
        while (ADCSRA & (1 << ADSC)); // wait until conversion is done

        // take first voltage measurement
        adcl = ADCL;
        adch = ADCH;
    }
    while ( adch > 0 && adcl > 0x10 );

    return c;
}



int main(void)
{
    uint8_t c;
    for ( c = 0; c < 64; c++ ) { // writing four bytes each call it light_is_low_enough()
        uint8_t light;
        light = light_detect();
        eeprom_update_byte( (uint8_t *)c, light );
        _delay_ms(100);
    }

    // set output pins
    DDRB  = (1<<DDB3) | (1<<DDB4) | (1<<DDB1);
    // turn on light
    PORTB = 1<<FF1_MALE;
    _delay_ms(250);


    return 0;
}

