#include "connectfourrulesengine.h"

using namespace std;

bool testC4RulesEngine()
{
    bool didPass = true;

    ConnectFourRulesEngine *re = new ConnectFourRulesEngine();

    //Test grid creation.
    Grid *currentState = new ConnectFourGrid();

    Grid *nextState = new ConnectFourGrid();
    *nextState = *currentState;

    double *move = new double[1];
    //Bottom of board.
    *move = 3.0;
    re->updateGrid(nextState, move, Elements::PLAYER_1);
    //Test isValidMove.

    cout << "Testing move: " << *currentState << "\nto\t : " << *nextState << "\n";
    if(!re->isValidMove(currentState, nextState, Elements::PLAYER_1))
    {
        cout << "Expected invalid, returned valid\n";
        didPass = false;
    }

    *currentState = *nextState;
    //The square in the top of the same row.
    *move = 3.0;
    //Invalid move.  Player 1 went twice.
    re->updateGrid(nextState, move, Elements::PLAYER_1);

    cout << "Testing move: " << *currentState << "\nto\t : " << *nextState << "\n";
    if(re->isValidMove(currentState, nextState, Elements::PLAYER_2))
    {
        cout << "Expected invalid, returned valid\n";
        didPass = false;
    }

    //Test isAWin()
    Elements::GenericPieceType pieces[4];

    pieces[0] = pieces[1] = pieces[2] = pieces[3] = Elements::TYPE1;
    if(!re->isAWin(pieces[0], pieces[1], pieces[2], pieces[3]))
    {
        cout << "expected a win, got false\n";
        didPass = false;
    }

    pieces[1] = Elements::EMPTY;
    if(re->isAWin(pieces[0], pieces[1], pieces[2], pieces[3]))
    {
        cout << "expected false, got a win\n";
        didPass = false;
    }

    pieces[0] = Elements::EMPTY;
    if(re->isAWin(pieces[0], pieces[1], pieces[2], pieces[3]))
    {
        cout << "expected false, got a win\n";
        didPass = false;
    }

    return didPass;
}
