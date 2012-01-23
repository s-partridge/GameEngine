#ifndef DATATYPES_H
#define DATATYPES_H

//Used for tic tac toe and connect four, these values are for conversions
//between enumerated data and double precision inputs for a neural net.
#define EMPTY_SQUARE        0.5
#define FRIENDLY_PIECE      1.0
#define OPPONENT_PIECE      0.0

#define EMPTY_LOWER_BOUND   0.33
#define EMPTY_UPPER_BOUND   0.66

#define NEURAL NET VERSON   3
namespace NeuralNetValues
{
    #define MOMENTUM        0.9
    #define LEARN_RATE      0.3
    #define NUM_LAYERS      2

    #define NUM_INPUTS      9
    #define NUM_OUTPUTS     9
}

namespace Elements
{
    //These values are common across different board games.
    //There is no reason to different ones for different rule sets.
    enum PlayerType { NONE, PLAYER_1, PLAYER_2 };
    enum GameState { NORMAL, P1WIN, P2WIN, DRAW };

    //This can be used in any base class where the game type is not yet known.
    //Since enumerated types are simply integer masks, an actual piece type can be cast
    //to and from these generic values.  The reason for using this is to provide more clarity
    //than simply using integers in base classes.  It should make it easier to tell
    //how a particular piece of data is used.
    enum GenericPieceType { EMPTY, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8, TYPE9, TYPE10, TYPE11, TYPE12 };
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
