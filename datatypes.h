#ifndef DATATYPES_H
#define DATATYPES_H

#define e                   2.71828183

//Used to offset values for moves when converting to and from a string.
//Specifically used with toString() and fromString() in BoardState
#define MOVE_OFFSET '0'

//Used for tic tac toe and connect four, these values are for conversions
//between enumerated data and double precision inputs for a neural net.
#define EMPTY_SQUARE        0.5
#define FRIENDLY_PIECE      1.0
#define OPPONENT_PIECE      0.0

//Used to convert between grid squares and set values in two-piece games.
#define EMPTY_LOWER_BOUND   0.33
#define EMPTY_UPPER_BOUND   0.66

//Current version of the neural net architecture.
#define NEURAL_NET_VERSION  3

//Constants for data type sizes.
#define BYTE_SIZE           1
#define INT_SIZE            4
#define DOUBLE_SIZE         8

//Number of iterations to train a neural network in one set.
#define NUM_TRAINING_ITERATIONS 80000
namespace NeuralNetValues
{
    #define MAX_WEIGHT_VAL  5.0
    #define MIN_WEIGHT_VAL  -5.0

    #define MOMENTUM        0.9
    #define LEARN_RATE      0.03
    #define NUM_LAYERS      2

    #define TTT_NUM_HIDDEN_0 27
    #define TTT_NUM_HIDDEN_1 27

    #define NUM_INPUTS      9
    #define NUM_OUTPUTS     1
}

namespace Elements
{
    //These values are common across different board games.
    //There is no reason to different ones for different rule sets.
    enum PlayerType { NONE = 0, PLAYER_1 = 1, PLAYER_2 = 2 };
    enum GameState { NORMAL = 0, P1WIN = 1, P2WIN = 2, DRAW = 3 };

    //This can be used in any base class where the game type is not yet known.
    //Since enumerated types are simply integer masks, an actual piece type can be cast
    //to and from these generic values.  The reason for using this is to provide more clarity
    //than simply using integers in base classes.  It should make it easier to tell
    //how a particular piece of data is used.
    enum GenericPieceType { EMPTY = 0, TYPE1 = 1, TYPE2 = 2, TYPE3 = 3,
                            TYPE4 = 4, TYPE5 = 5, TYPE6 = 6, TYPE7 = 7,
                            TYPE8 = 8, TYPE9 = 9, TYPE10 = 10, TYPE11 = 11, TYPE12 = 12 };
}

//Since these two games are closely related, I included both the
//enumerated types here.  Later games should be added somewhere else.
namespace TicTacToeElements
{
    enum TicTacToePiece { EMPTY, X_PIECE, O_PIECE };
}

namespace ConnectFourElements
{
    enum ConnectFourPiece { EMPTY, RED_PIECE, BLACK_PIECE };
}
#endif // DATATYPES_H
