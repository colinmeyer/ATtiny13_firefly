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

