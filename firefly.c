//
// ATtiny13a LED Firefly
//
// cf.
//
//     Atmel ATtiny13 manual 
//     http://www.atmel.com/Images/doc8126.pdf 
//
//     Project of initial inspiration
//     http://www.seanet.com/~karllunt/fireflyLED.html
//
//     Very useful tutorial
//     http://brownsofa.org/blog/archives/191
//

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

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

    uint8_t measure  = 0;  // 8 bit

    ADCSRA |= (1 << ADSC);        // start single conversion
    while (ADCSRA & (1 << ADSC)); // wait until conversion is done

    measure = ADCH;

    // turn off ADC
    ADCSRA &= ~(1<<ADEN);

    // turn off power to light detector
    PORTB &= ~(LM_POWER);
    DDRB  &= ~(LM_POWER_DD);

    // !( measure & 0x80 ) means dark
    // i.e. measure < 0x80
    // i.e. just look at the most significant bit.
    //      1 == light ; 0 == dark
    return !(measure & 0x80);
}


// runs current state
// returns true if ready to sleep
uint8_t runstate() {
    static uint8_t  state = 0;
    static uint8_t  counter = 0;
    uint8_t ready_to_sleep  = 1;

    switch(state) {
        case 0: // light meter mode
            // set sleep to 1s
            WDTCR = (WDTCR & 0xd8)     // 0xd8 == 0b11011000 => mask out prescaler bits
                | (1<<WDP2)|(1<<WDP1); // 1s - p.43  8.5.2

            if ( dark_out() ) {
                state = 1;
                counter = 4 + (rand() % 6); // random 4-9
                ready_to_sleep = 0;
            }
            break;
        case 1: // male on
            // set sleep to 0.25s
            WDTCR = (WDTCR & 0xd8) | (1<<WDP2);

            // turn on light
            DDRB  |= FF1_MALE_DD;
            PORTB |= FF1_MALE;

            state = 2;
            break;
        case 2: // male off
            // turn light off
            DDRB  &= ~(FF1_MALE_DD);
            PORTB &= ~(FF1_MALE);

            if( --counter )
                state = 1;
            else
                state = 3;
            break;
        case 3: // pause between male and female
            // set sleep to 3s XXX 2s for now
            WDTCR = (WDTCR & 0xd8) | (1<<WDP2)|(1<<WDP1)|(1<<WDP0);

            state = 4;
            counter = 1 + (rand() % 4); // random 1-4
            break;
        case 4: // female on
            // set sleep to 0.5s
            WDTCR = (WDTCR & 0xd8) | (1<<WDP2)|(1<<WDP0);

            // turn on light
            DDRB  |= FF1_FEMALE_DD;
            PORTB |= FF1_FEMALE;

            state = 5;
            break;
        case 5: // female off
            // set sleep to 0.25s
            WDTCR = (WDTCR & 0xd8) | (1<<WDP2);

            // turn light off
            DDRB  &= ~(FF1_FEMALE_DD);
            PORTB &= ~(FF1_FEMALE);

            if( --counter )
                state = 4;
            else
                state = 6;
            break;
        case 6: // pause before starting over
            // set sleep to 5s XXX 4s for now
            WDTCR = (WDTCR & 0xd8) | (1<<WDP3);

            state = 0;
            break;
    }

    return ready_to_sleep;
}

ISR(WDT_vect) {
    // just wake up and return to the main loop
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

    // Set up Port B data to be all low
    PORTB = 0;  

    // set wdt prescaler
    WDTCR = (1<<WDP2);   // 32k ~.25s  8.5.2 p.43

    // Enable watchdog timer interrupts
    WDTCR |= (1<<WDTIE);

    sei(); // Enable global interrupts 

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    while (1) {
        // runstate() runs the current state,
        // and returns true if we should sleep
        if ( runstate() )
            sleep_mode();
    }

    return 0;
}

