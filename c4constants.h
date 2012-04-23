#ifndef C4CONSTANTS_H
#define C4CONSTANTS_H

//C4 constants
#define C4_NUM_TRAINING_ITERATIONS  20000
//Board size constants
#define C4_WIDTH            7
#define C4_HEIGHT           6

//C4 Network constants
#define C4_NUM_INPUTS       C4_WIDTH * C4_HEIGHT
#define C4_NUM_OUTPUTS      1

#define C4_NUM_LAYERS       2
#define C4_NUM_NEURONS_1    21
#define C4_NUM_NEURONS_2    21

#define C4_OUTPUT_STRETCH   5

#define C4_MOMENTUM         0.9
#define C4_LEARN_RATE       0.01

#endif // C4CONSTANTS_H
