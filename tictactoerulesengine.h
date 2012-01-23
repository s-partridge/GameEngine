#ifndef TICTACTOERULESENGINE_H
#define TICTACTOERULESENGINE_H

#include "rulesengine.h"
#include "tictactoegrid.h"

class TicTacToeRulesEngine : public RulesEngine
{
    //Local using statements.  Allows only class-level access to
    //these namespaces without the scope resolution operator.
    using namespace Elements;
    using namespace TicTacToeElements;

public:
    TicTacToeRulesEngine() {}

    //Test a board state for end game conditions.
    GameState testBoard(const Grid *boardState);

    //Check whether a state is a valid move from currentState.
    bool isValidMove(const Grid *currentState, const Grid *nextState, Elements::PlayerType currentPlayer);

    //Find the value of a board state.
    //That is, find whether it represents a good or bad position for the current player.
    //Values range from 0 (loss) to 1 (win).   0.5 is a neutral state (empty board, cat's game, etc).
    float worthOfState(const Grid *boardState, PlayerType currentPlayer, GameState currentState);

    //Generate grids for possible moves from the current state.
    void genNextMoves(const Grid *current, Grid **nextMoves, Elements::PlayerType currentPlayer, int &numNextStates);

    //Return value is true or false, depending on whether or not the copy was successful.
    bool gridToDoubleArray(const Grid *grid, double *array, PlayerType player, const int index, const int numElements);

    //Return value is true or false, depending on whether or not the copy was successful.
    bool doubleArrayToGrid(Grid *grid, const double *array, PlayerType player, const int index, const int numElements);

    //Convert a piece type to its double precision equivalent, relative to the current player.
    double pieceTypeToDouble(PlayerType player, GenericPieceType piece);

    //Convert a double precision value to its piece type equivalent, relative to the current player.
    GenericPieceType doubleToPieceType(PlayerType player, double value);

private:
//    GameState m_gameState;
    //Returns the worth of a game, assuming that the state passed in is not 'NORMAL'.
    //Used in worthOfState to quickly handle end game states.
    float endStateWorth(PlayerType currentPlayer, GameState currentState);
    float normalStateWorth(const Grid *boardState, PlayerType currentPlayer);

    //The first and second moves are relatively easy to calculate.
    float firstMoveWorth(const Grid *boardState, PlayerType currentPlayer);
    float secondMoveWorth(const Grid *boardState, PlayerType currentPlayer);

    //Any moves after than will be different.
    float laterMoveWorth(const Grid *boardState, PlayerType currentPlayer);

    //Count the number of two-in-a-row for X or O that are either open or blocked.
    //If countBlocked is true, the funtion will return the number of blocked sets it finds (E.G. XOX, where O is blocking)
    //If false, the function will return the number of open sets it finds (E.G X_X, where the center is open)
    int countDoubleSpace(const Grid *boardState, PlayerType toCount, bool countBlocked);
};

#endif // TICTACTOERULESENGINE_H
