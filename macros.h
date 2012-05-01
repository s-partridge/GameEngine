#ifndef MACROS_H
#define MACROS_H

#include <cmath>
#include <iostream>

//DEBUGGING CONSTANTS
//#define DEBUG_RULESENGINE
//#define DEBUG_BOARDSTATE
//#define DEBUG_STATEWORTH
//#define DEBUG_GRID
//#define DEBUG_GAMEDATA
//#define DEBUG_VIEW
#define DEBUG_TRAINER
//#define DEBUG_NN_ACCBLAMES
//#define DEBUG_NEURON
//#define DEBUG_NEURON_SUMMATION
//#define DEBUG_NEURONLAYER
//#define DEBUG_NEURONLAYER_MOMENTUM
//#define DEBUG_NEURALNETWORK
//#define DEBUG_NNOUTPUTS
//#define DEBUG_NNRESULTS
//#define DEBUG_NNDERIVATIVE
//#define DEBUG_SAVENNP
//#define DEBUG_GAMECONTROLLER
//#define DEBUG_C4RE
//#define DEBUG_C4GENNEXTMOVES
//#define DEBUG_TTTGENNEXTMOVES
//#define DEBUG_MEMORY
//#define DEBUG_C4VIEW
//#define DEBUG_C4GAMEDATA

//#define DEBUG_DATABASE_FILE_IO

//#define DEBUG_TDNEURALNET

//Math macros
//Floor reduces a floating point number to its integer component.
//By adding 0.5, we ensure that a number with a decimal part >= 0.5 will be rounded up.
#define round(x)                    (floor(x + 0.5))
//Absolute value macro for floating point numbers.
#define absd(x) (x < 0 ? x * -1 : x)

//Printing macros used in testing.
#define print(x)                    std::cout << x
#define printLine(x)                std::cout << x << std::endl
#define print2(y, z)                std::cout << y << z
#define printLine2(y, z)            std::cout << y << z << std::endl
#define print3(x, y, z)             std::cout << x << y << z
#define printLine3(x, y, z)         std::cout << x << y << z << std::endl
#define print4(w, x , y, z)         std::cout << w << x << y << z
#define printLine4(w, x , y, z)     std::cout << w << x << y << z << std::endl
#define print5(v, w, x, y, z)       std::cout << v << w << x << y << z
#define printLine5(v, w, x, y, z)   std::cout << v << w << x << y << z << std::endl

#endif // MACROS_H
