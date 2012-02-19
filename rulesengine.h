#ifndef RULESENGINE_H
#define RULESENGINE_H

#include "grid.h"
#include "datatypes.h"
#include "macros.h"

class RulesEngine
{
public:
    RulesEngine() {}

    //Check whether nextState is a valid move from currentState.
    virtual bool isValidMove(const Grid *currentState, const Grid *nextState, Elements::PlayerType currentPlayer) const = 0;
    //Test a board state for end game conditions.
    virtual Elements::GameState testBoard(const Grid *boardState) const = 0;
    //Find the value of a board state.
    //That is, find whether it represents a good or bad position for the current player.
    //Values range from 0 (loss) to 1 (win).   0.5 is a neutral state (empty board, cat's game, etc).
    virtual float worthOfState(const Grid *boardState, Elements::PlayerType currentPlayer, Elements::GameState currentState) const = 0;

    //Generate grids for moves from the current state.
    virtual void genNextMoves(const Grid *current, Grid **&nextMoves, Elements::PlayerType currentPlayer, int &numNextStates) const = 0;
    /*
    Convert a given grid to an array of doubles.
      grid is the Grid to copy from.
      array is the double array to store grid spaces in.  Array should be large enough to contain (GRID_WIDTH * GRID_HEIGHT) elements.
      player contains the current player, used for conversion to PlayerType objects in the resulting Grid.
      index is the index of the first element to read from.  Allows a larger array to contain multiple grids.
      numElements is the size of array.

    Return value is true or false, depending on whether or not the copy was successful.*/
    virtual bool gridToDoubleArray(const Grid *grid, double *&array, Elements::PlayerType player, const int index, const int numElements) const = 0;

    //Change grid so it matches the move stored in outputs.
    //Outputs is expected to be a move pulled from neuralNetwork.
    virtual void updateGrid(Grid *grid, const double *outputs, Elements::PlayerType player) const = 0;

    /*
    Convert an array of doubles to a grid.
      grid is the Grid to store the results in.
      array contains the array to copy from.
      player contains the current player, used for conversion to PlayerType objects in the resulting Grid.
      index is the index of the first element to read from.  Allows a larger array to contain multiple grids.
      numElements is the size of array.
      The array should be large enough to contain 9 elements between index and numElements.
        If there are not enough spaces in the array, a grid of 9 XPLAYER spaces will be returned.

    Return value is true or false, depending on whether or not the copy was successful.*/
    virtual bool doubleArrayToGrid(Grid *&grid, const double *array, Elements::PlayerType player, const int index, const int numElements) const = 0;

    //Convert a piece type to its double precision equivalent, relative to the current player.
    virtual double pieceTypeToDouble(Elements::PlayerType player, Elements::GenericPieceType piece) const = 0;

    //Convert a double precision value to its piece type equivalent, relative to the current player.
    virtual Elements::GenericPieceType doubleToPieceType(Elements::PlayerType player, double value) const = 0;

    //generates a new instance of a grid with specific values for the concrete game type.
    virtual Grid *createGameSpecificGrid() const = 0;
};

#endif // RULESENGINE_H
