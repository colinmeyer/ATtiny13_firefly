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
#define MEASURE_GAP_DELAY 100000   // us

#define FF1_ITERATIONS 33

#define FF1_MALE   PB3   // pin 2
#define FF1_MALE_  PB4   // pin 3
#define FF1_FEMALE PB1   // pin 6

#define FF1_MASK (1<<FF1_MALE | 1<<FF1_FEMALE)


// light meter returns true when it's dark out
uint8_t light_is_low_enough() {
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
    uint16_t  first_measure  = 0;
    uint16_t  second_measure = 0;

    // charge detector LED briefly (it is a capacitor)
    DDRB  =  (1<<DDB4); // select pin 3 for output
    PORTB =  (1<<FF1_MALE_ ); // set pin to high
    _delay_ms(POWER_DELAY); // charge for some time

    DDRB  = 0;           // select pin 3 for tri-state
                         // currently, pin is still tied to pull-up resistor
                         // cf.  10.2.3 p.51
    PORTB &= ~(1<<FF1_MALE_ ); // set pin to low (switch pull-up resistor off)
                         // now pin is tri-state

    ADCSRA |= (1 << ADSC);        // start single conversion
    while (ADCSRA & (1 << ADSC)); // wait until conversion is done

    // take first voltage measurement
    adcl = ADCL;
    adch = ADCH;
    first_measure = adch << 8 | adcl;
    static uint8_t write_count = 0;
    uint8_t offset = 4 * write_count;
    eeprom_update_byte((uint8_t *)offset,   adch);
    eeprom_update_byte((uint8_t *)offset+1, adcl);
    eeprom_update_word((uint16_t *)offset+2, first_measure);
    if ( ++write_count >= 64/4 ) {
        exit((int)1);
    }


    // delay some constant time amount, while LED discharges
    _delay_us(MEASURE_GAP_DELAY);

    ADCSRA |= (1 << ADSC);        // start single conversion
    while (ADCSRA & (1 << ADSC)); // wait until conversion is done

    adcl = ADCL;
    adch = ADCH;
    second_measure = adch << 8 | adcl;

    // turn off ADC
    ADCSRA &= ~(1<<ADEN);

    // calculate the diff between the two -- the amount that the capacitor has discharged
    // bright shining on the LED makes it discharge quicker
    // so the second measure will be lower
    // causing the displayed number (diff) to be higher
    uint16_t diff;
    if ( second_measure > first_measure )
        diff = 0;
    else
        diff = first_measure - second_measure;

    static uint8_t write_address = 0;
//     eeprom_update_word((uint16_t *)(write_address), first_measure);
//     write_address += 2;
// 
//     eeprom_update_word((uint16_t *)(write_address), second_measure);
//     write_address += 2;

    eeprom_update_word((uint16_t *)(write_address), diff);
    write_address += 2;

    // smaller is darker; function returns true for dark
    return (diff < 0x0300) ? 1 : 0;
}



int main(void)
{

    uint8_t c;
    for ( c = 0; c < 64/2; c++ ) { // writing four bytes each call it light_is_low_enough()
        light_is_low_enough();
        _delay_ms(100);
    }

    // set output pins
    DDRB  = (1<<DDB3) | (1<<DDB4) | (1<<DDB1);
    // turn on light
    PORTB = 1<<FF1_MALE;
    _delay_ms(250);


    return 0;
}

