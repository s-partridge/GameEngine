#ifndef CONNECTFOURRULESENGINE_H
#define CONNECTFOURRULESENGINE_H

#include "connectfourgrid.h"
#include "rulesengine.h"

class ConnectFourRulesEngine : public RulesEngine
{
public:
    ConnectFourRulesEngine() {}

    //Test a board state for end game conditions.
    Elements::GameState testBoard(const Grid *boardState) const;

    //Check whether a state is a valid move from currentState.
    bool isValidMove(const Grid *currentState, const Grid *nextState, Elements::PlayerType currentPlayer) const;

    //DEPRECATED!!!
    //This function no longer has a use.  Therefore, it will return a value of -1.
    float worthOfState(const Grid *boardState, Elements::PlayerType currentPlayer, Elements::GameState currentState) const { return -1.0; }

    //Generate grids for possible moves from the current state.
    void genNextMoves(const Grid *current, Grid **&nextMoves, int *&lastMoves, Elements::PlayerType currentPlayer, int &numNextStates) const;

    //Converts a grid object into an array of doubles, given a large enough array.
    //Return value is true or false, depending on whether or not the copy was successful.
    bool gridToDoubleArray(const Grid *grid, double *&array, Elements::PlayerType player, int index, const int numElements) const;

    //Change grid so it matches the move stored in outputs.
    //Outputs is expected to be a move pulled from neuralNetwork.
    void updateGrid(Grid *grid, const double *outputs, Elements::PlayerType player) const;

    //Converts an array of double to a grid object, given a large enough array.
    //Return value is true or false, depending on whether or not the copy was successful.
    bool doubleArrayToGrid(Grid *&grid, const double *array, Elements::PlayerType player, int index, const int numElements) const;

    //Convert a piece type to its double precision equivalent, relative to the current player.
    double pieceTypeToDouble(Elements::PlayerType player, Elements::GenericPieceType piece) const;

    //Convert a double precision value to its piece type equivalent, relative to the current player.
    Elements::GenericPieceType doubleToPieceType(Elements::PlayerType player, double value) const;

    Grid* createGameSpecificGrid() const;

private:
    int getFirstOpenSpace(const Grid *grid, int column) const;
    void updateGrid(Grid *grid, int output, Elements::PlayerType player) const;

public:
    bool isAWin(Elements::GenericPieceType sq1, Elements::GenericPieceType sq2, Elements::GenericPieceType sq3, Elements::GenericPieceType sq4) const;
};

#endif // CONNECTFOURRULESENGINE_H
