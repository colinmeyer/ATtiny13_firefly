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

#define F_CPU 9.6E6L/8 // CPU Freq. Must come before delay.h include. 9.6MHz
                       // by default, sysclock is /8
#include <util/delay.h>

#define POWER_DELAY         1
#define MEASURE_GAP_DELAY 100

#define FF1_ITERATIONS 33

#define FF1_MALE   PB3   // pin 2
#define FF1_MALE_  PB4   // pin 3
#define FF1_FEMALE PB1   // pin 6



// returns true when it's dark
uint8_t dark_out() {
    // read ADC2 input
    ADMUX = 2               // Select channel ADC = 2
        | (1<<ADLAR);       // Left shift 8 significant bits to ADCH
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

    // count loop iterations until voltage read from "capacitor" LED
    // is less than threshold
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

    // turn off ADC
    ADCSRA &= ~(1<<ADEN);

//  count < 30 means light out
    if (c > 30)
        return 0;
    else
        return 1;
}


uint16_t c=0;      // 16 bit
uint8_t  mode = 0; // 0 = light meter
                   // 1 = firefly
ISR(WDT_vect) {
    if (mode==0) {
        // signal mode 0, FF1_FEMALE light on
        DDRB  |= 1<<DDB1;
        PORTB |= 1<<FF1_FEMALE;

        if (dark_out()) {
            // switch to firefly
            mode=1;

            // adjust WDT prescaler
            WDTCR = (WDTCR & 0xd8)     // 0xd8 == 0b11011000 => mask out prescaler bits
                | (1<<WDP2);       // 32k ~.25 8.5.2 p.43
        }
    }

    if (mode==1) {
        // set output pins
        DDRB  = (1<<DDB3) | (1<<DDB4) | (1<<DDB1);

        // turn off mode0 light
        PORTB &= ~(1<<FF1_FEMALE);

        // set PORT B to whatever the lookup table says
        PORTB |= 1<<FF1_MALE;

        if (++c==FF1_ITERATIONS) {
            // change to light meter
            mode=0;
            c=0;

            PORTB = 0; // shut off all lights
            DDRB  = 0; // shut off outputs

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
    //  - not necessary; off by default
    //
    // turn off Internal Voltage Reference
    //  - not necessary; ADC is off & BOD is off
    //
    // turn off all digital inputs Port Pins
    DIDR0 = 
        (1<<AIN0D) |
        (1<<AIN1D) |
        (1<<ADC0D) |
        (1<<ADC1D) |
        (1<<ADC2D) |
        (1<<ADC3D);

    // set wdt prescaler
    WDTCR = (1<<WDP2)|(1<<WDP1); // 1s //(1<<WDP3);   // 10M ~8s  8.5.2 p.43

    // Enable watchdog timer interrupts
    WDTCR |= (1<<WDTIE);

    sei(); // Enable global interrupts 

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    while (1) {
        sleep_mode();
    }

    return 0;
}

