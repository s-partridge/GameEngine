
#ifndef TICTACTOERULESENGINE_H
#define TICTACTOERULESENGINE_H

#include "rulesengine.h"
#include "tictactoegrid.h"

class TicTacToeRulesEngine : public RulesEngine
{
    //Local using statements.  Allows only class-level access to
    //these namespaces without the scope resolution operator.
    //...these don't work.  I have no idea why.
    //using namespace Elements;
    //using namespace TicTacToeElements;

public:
    TicTacToeRulesEngine() {}

    //Test a board state for end game conditions.
    Elements::GameState testBoard(const Grid *boardState) const;

    //Check whether a state is a valid move from currentState.
    bool isValidMove(const Grid *currentState, const Grid *nextState, Elements::PlayerType currentPlayer) const;

    //Find the value of a board state.
    //That is, find whether it represents a good or bad position for the current player.
    //Values range from 0 (loss) to 1 (win).   0.5 is a neutral state (empty board, cat's game, etc).
    float worthOfState(const Grid *boardState, Elements::PlayerType currentPlayer, Elements::GameState currentState) const;

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

    Grid *createGameSpecificGrid() const;

private:
    //Returns the worth of a game, assuming that the state passed in is not 'NORMAL'.
    //Used in worthOfState to quickly handle end game states.
    float endStateWorth(Elements::PlayerType currentPlayer, Elements::GameState currentState) const;
    float normalStateWorth(const Grid *boardState, Elements::PlayerType currentPlayer) const;

    //The first and second moves are relatively easy to calculate.
    float firstMoveWorth(const Grid *boardState, Elements::PlayerType currentPlayer) const;
    float secondMoveWorth(const Grid *boardState, Elements::PlayerType currentPlayer) const;

    //Any moves after than will be different.
    float laterMoveWorth(const Grid *boardState, Elements::PlayerType currentPlayer) const;

    //Count the number of two-in-a-row for X or O that are either open or blocked.
    //If countBlocked is true, the funtion will return the number of blocked sets it finds (E.G. XOX, where O is blocking)
    //If false, the function will return the number of open sets it finds (E.G X_X, where the center is open)
    int countDoubleSpace(const Grid *boardState, Elements::GenericPieceType toCount, bool countBlocked) const;
};

#endif // TICTACTOERULESENGINE_H
