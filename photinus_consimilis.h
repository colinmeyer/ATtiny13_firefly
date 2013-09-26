#define COMBINED_ITERATIONS 1155     // number of iterations in a cycle 
#define FF1_ITERATIONS 33
#define FF2_ITERATIONS 35

#define FF2_MALE 0
#define FF2_FEMALE 0

#define FF1_MASK (1<<FF1_MALE | 1<<FF1_FEMALE)
#define FF2_MASK (1<<FF2_MALE | 1<<FF2_FEMALE)


// lookup table for lights status at each iteration
const unsigned char lights[] = {
    1<<FF1_MALE | 0<<FF1_FEMALE, // | 1<<FF2_MALE | 0<<FF2_FEMALE ,  //  0
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  1
    1<<FF1_MALE | 0<<FF1_FEMALE, // | 1<<FF2_MALE | 0<<FF2_FEMALE ,  //  2
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  3
    1<<FF1_MALE | 0<<FF1_FEMALE, // | 1<<FF2_MALE | 0<<FF2_FEMALE ,  //  4
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  5
    1<<FF1_MALE | 0<<FF1_FEMALE, // | 1<<FF2_MALE | 0<<FF2_FEMALE ,  //  6
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  7
    1<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  8
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  9
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  10
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  11
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  12
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  13
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  14
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  15
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  16
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  17
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 1<<FF2_FEMALE ,  //  18
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  19
    0<<FF1_MALE | 1<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  20
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  21
    0<<FF1_MALE | 1<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  22
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  23
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  24
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  25
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  26
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  27
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  28
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  29
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  30
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  31
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  32
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  33
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  34
    0<<FF1_MALE | 0<<FF1_FEMALE, // | 0<<FF2_MALE | 0<<FF2_FEMALE ,  //  35
};
