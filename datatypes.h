#ifndef DATATYPES_H
#define DATATYPES_H

namespace NeuralNetValues
{
    #define MOMENTUM        0.9
    #define LEARN_RATE      0.3
    #define NUM_LAYERS      2
}
namespace Elements
{
    #define EMPTY           0

    #define PLAYER_ONE      1
    #define PLAYER_TWO      2

    #define PIECE_X         3
    #define PIECE_O         4

    #define PIECE_RED       3
    #define PIECE_BLACK     4

    enum GameState { NORMAL, P1WIN, P2WIN, DRAW };
}

#endif // DATATYPES_H
