//
// ATtiny13a LED Firefly
//
// cf.
//
//     Atmel ATtiny13 manual 
//     http://www.atmel.com/Images/doc8126.pdf 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define F_CPU 9.6E6L/8 // CPU Freq. Must come before delay.h include. 9.6MHz
                       // by default, sysclock is /8
#include <util/delay.h>

#define POWER_DELAY         1
#define MEASURE_GAP_DELAY 100

#define FF1_ITERATIONS 33

#define FF1_MALE      1<<PB0   // pin 5
#define FF1_MALE_DD   1<<DDB0  // pin 5
#define FF1_FEMALE    1<<PB1   // pin 6
#define FF1_FEMALE_DD 1<<DDB1  // pin 6

#define LM_POWER      1<<PB3   // pin 2
#define LM_POWER_DD   1<<DDB3  // pin 2

// returns true when it's dark
uint8_t dark_out() {
    // turn on power to light detector
    DDRB  |= LM_POWER_DD;
    PORTB |= LM_POWER;

    // read ADC2 input (pin 3)
    ADMUX = 2               // Select channel ADC = 2
        | (1<<ADLAR);       // Left shift 8 significant bits to ADCH
    ADCSRA |= (1<<ADEN);    // turn on ADC

    // throw away
    ADCSRA |= (1 << ADSC);        // start single conversion
    while (ADCSRA & (1 << ADSC)); // wait until conversion is done

    unsigned char  measure  = 0;  // 8 bit

    ADCSRA |= (1 << ADSC);        // start single conversion
    while (ADCSRA & (1 << ADSC)); // wait until conversion is done

    measure = ADCH;

    // turn off ADC
    ADCSRA &= ~(1<<ADEN);

    // turn off power to light detector
    PORTB &= ~(LM_POWER);
    DDRB  &= ~(LM_POWER_DD);

    // ~( measure & 0x80 ) means dark
    // i.e. measure < 0x80
    // i.e. just look at the most significant bit.
    //      1 == light ; 0 == dark
    return !(measure & 0x80);
}


uint16_t c=0;      // 16 bit
uint8_t  mode = 0; // 0 = light meter
                   // 1 = firefly
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

