#include "connectfourrulesengine.h"
#include "connectfourtrainer.h"

#define C4_NUM_MOVE_BLOCKER_TEST_STATES 12
#define DEBUG_UNIT_TESTS
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

static void createBoardStateList(Grid **&states, Grid **&expected)
{
    states = new Grid*[C4_NUM_MOVE_BLOCKER_TEST_STATES];
    expected = new Grid*[C4_NUM_MOVE_BLOCKER_TEST_STATES];

    //Create empty grids.
    for(int x = 0; x < C4_NUM_MOVE_BLOCKER_TEST_STATES; ++x)
    {
        states[x] = new ConnectFourGrid();
        expected[x] = new ConnectFourGrid();
    }

    //First grid
    // - - - - - - -
    // - - - - - - -
    // - - - - - - -
    // O - - - - - -
    // O X - - - - -
    // O X X - - - -
    states[0]->squares[0][5] = Elements::TYPE1;
    states[0]->squares[0][4] = Elements::TYPE1;
    states[0]->squares[0][3] = Elements::TYPE1;
    states[0]->squares[1][5] = Elements::TYPE2;
    states[0]->squares[1][4] = Elements::TYPE2;
    states[0]->squares[2][5] = Elements::TYPE2;

    //Expected move
    // Column 0
    *expected[0] = *states[0];
    expected[0]->squares[0][2] = Elements::TYPE2;

    //Second grid
    // - - - - - - -
    // - O - - - - -
    // - O - - - - -
    // X O - - - - -
    // X X - - - - -
    // X O X - - - -
    states[1]->squares[0][5] = Elements::TYPE2;
    states[1]->squares[0][4] = Elements::TYPE2;
    states[1]->squares[0][3] = Elements::TYPE2;
    states[1]->squares[1][5] = Elements::TYPE1;
    states[1]->squares[1][4] = Elements::TYPE2;
    states[1]->squares[1][3] = Elements::TYPE1;
    states[1]->squares[1][2] = Elements::TYPE1;
    states[1]->squares[1][1] = Elements::TYPE1;
    states[1]->squares[2][5] = Elements::TYPE2;

    //Expected move
    // Column 1
    *expected[1] = *states[1];
    expected[1]->squares[1][0] = Elements::TYPE2;

    //Third grid
    // - - - - - - -
    // - - - - - - -
    // - - - - - - O
    // - - - - - - O
    // - - - - - - O
    // - - x - - x x
    states[2]->squares[2][5] = Elements::TYPE2;
    states[2]->squares[5][5] = Elements::TYPE2;
    states[2]->squares[6][5] = Elements::TYPE2;
    states[2]->squares[6][4] = Elements::TYPE1;
    states[2]->squares[6][3] = Elements::TYPE1;
    states[2]->squares[6][2] = Elements::TYPE1;

    //Expected move
    // Column 6
    *expected[2] = *states[2];
    expected[2]->squares[6][1] = Elements::TYPE2;

    //Fourth grid
    // - - - - - - -
    // - - - - - - -
    // - - - - - - -
    // X - - - - - -
    // X X - - - - -
    // O O O - - - -

    states[3]->squares[0][5] = Elements::TYPE1;
    states[3]->squares[0][4] = Elements::TYPE2;
    states[3]->squares[0][3] = Elements::TYPE2;
    states[3]->squares[1][5] = Elements::TYPE1;
    states[3]->squares[1][4] = Elements::TYPE2;
    states[3]->squares[2][5] = Elements::TYPE1;

    //Expected move
    // Column 3
    *expected[3] = *states[3];
    expected[3]->squares[3][5] = Elements::TYPE2;

    //Fifth grid
    // - - - - - - -
    // - - - - - - -
    // - - - - - - -
    // - - - - - - -
    // - X X - - - -
    // - O O - - - -

    states[4]->squares[1][5] = Elements::TYPE1;
    states[4]->squares[1][4] = Elements::TYPE2;
    states[4]->squares[2][5] = Elements::TYPE1;
    states[4]->squares[2][4] = Elements::TYPE2;

    //Expected move
    // Column 0
    *expected[4] = *states[4];
    expected[4]->squares[0][5] = Elements::TYPE2;

    //Sixth grid
    // - - - - - - -
    // - - - - - - -
    // - - - - - - -
    // - - - - - - -
    // - X X - - - -
    // X O O O - - -

    states[5]->squares[0][5] = Elements::TYPE2;
    states[5]->squares[1][5] = Elements::TYPE1;
    states[5]->squares[1][4] = Elements::TYPE2;
    states[5]->squares[2][5] = Elements::TYPE1;
    states[5]->squares[2][4] = Elements::TYPE2;
    states[5]->squares[3][5] = Elements::TYPE1;

    //Expected move
    // Column 4
    *expected[5] = *states[5];
    expected[5]->squares[4][5] = Elements::TYPE2;

    //Seventh grid
    // - - - - - - -
    // - - - - - - -
    // - - - - - - -
    // - - X - O O -
    // - X X O X O -
    // X O O O X X -

    states[6]->squares[0][5] = Elements::TYPE2;
    states[6]->squares[1][5] = Elements::TYPE1;
    states[6]->squares[1][4] = Elements::TYPE2;
    states[6]->squares[2][5] = Elements::TYPE1;
    states[6]->squares[2][4] = Elements::TYPE2;
    states[6]->squares[2][3] = Elements::TYPE2;
    states[6]->squares[3][5] = Elements::TYPE1;
    states[6]->squares[3][4] = Elements::TYPE1;
    states[6]->squares[4][5] = Elements::TYPE2;
    states[6]->squares[4][4] = Elements::TYPE2;
    states[6]->squares[4][3] = Elements::TYPE1;
    states[6]->squares[5][5] = Elements::TYPE2;
    states[6]->squares[5][4] = Elements::TYPE1;
    states[6]->squares[5][3] = Elements::TYPE1;

    //Expected move
    // Column 6
    *expected[6] = *states[6];
    expected[6]->squares[5][2] = Elements::TYPE2;

    //Eighth grid
    // - - X - - X -
    // - X O O X O -
    // - O O O X X -
    // - X O X O X O
    // X O X O X X O
    // X O O O X O X

    states[7]->squares[0][5] = Elements::TYPE2;
    states[7]->squares[0][4] = Elements::TYPE2;
    states[7]->squares[1][5] = Elements::TYPE1;
    states[7]->squares[1][4] = Elements::TYPE1;
    states[7]->squares[1][3] = Elements::TYPE2;
    states[7]->squares[1][2] = Elements::TYPE1;
    states[7]->squares[1][1] = Elements::TYPE2;
    states[7]->squares[2][5] = Elements::TYPE1;
    states[7]->squares[2][4] = Elements::TYPE2;
    states[7]->squares[2][3] = Elements::TYPE1;
    states[7]->squares[2][2] = Elements::TYPE1;
    states[7]->squares[2][1] = Elements::TYPE1;
    states[7]->squares[2][0] = Elements::TYPE2;
    states[7]->squares[3][5] = Elements::TYPE1;
    states[7]->squares[3][4] = Elements::TYPE1;
    states[7]->squares[3][3] = Elements::TYPE2;
    states[7]->squares[3][2] = Elements::TYPE1;
    states[7]->squares[3][1] = Elements::TYPE1;
    states[7]->squares[4][5] = Elements::TYPE2;
    states[7]->squares[4][4] = Elements::TYPE2;
    states[7]->squares[4][3] = Elements::TYPE1;
    states[7]->squares[4][2] = Elements::TYPE2;
    states[7]->squares[4][1] = Elements::TYPE2;
    states[7]->squares[5][5] = Elements::TYPE1;
    states[7]->squares[5][4] = Elements::TYPE2;
    states[7]->squares[5][3] = Elements::TYPE2;
    states[7]->squares[5][2] = Elements::TYPE2;
    states[7]->squares[5][1] = Elements::TYPE1;
    states[7]->squares[5][0] = Elements::TYPE2;
    states[7]->squares[6][5] = Elements::TYPE2;
    states[7]->squares[6][4] = Elements::TYPE1;
    states[7]->squares[6][3] = Elements::TYPE1;

    //Expected move
    // Column 0
    *expected[7] = *states[7];
    expected[7]->squares[0][3] = Elements::TYPE2;

    //Ninth grid
    // O X X - - X -
    // X O O O X O -
    // X O O O X X X
    // O X O X O X O
    // X O X O X X X
    // X O O O X O O
    states[8]->squares[0][5] = Elements::TYPE2;
    states[8]->squares[0][4] = Elements::TYPE2;
    states[8]->squares[0][3] = Elements::TYPE1;
    states[8]->squares[0][2] = Elements::TYPE2;
    states[8]->squares[0][1] = Elements::TYPE2;
    states[8]->squares[0][0] = Elements::TYPE1;
    states[8]->squares[1][5] = Elements::TYPE1;
    states[8]->squares[1][4] = Elements::TYPE1;
    states[8]->squares[1][3] = Elements::TYPE2;
    states[8]->squares[1][2] = Elements::TYPE1;
    states[8]->squares[1][1] = Elements::TYPE1;
    states[8]->squares[1][0] = Elements::TYPE2;
    states[8]->squares[2][5] = Elements::TYPE1;
    states[8]->squares[2][4] = Elements::TYPE2;
    states[8]->squares[2][3] = Elements::TYPE1;
    states[8]->squares[2][2] = Elements::TYPE1;
    states[8]->squares[2][1] = Elements::TYPE1;
    states[8]->squares[2][0] = Elements::TYPE2;
    states[8]->squares[3][5] = Elements::TYPE1;
    states[8]->squares[3][4] = Elements::TYPE1;
    states[8]->squares[3][3] = Elements::TYPE2;
    states[8]->squares[3][2] = Elements::TYPE1;
    states[8]->squares[3][1] = Elements::TYPE1;
    states[8]->squares[4][5] = Elements::TYPE2;
    states[8]->squares[4][4] = Elements::TYPE2;
    states[8]->squares[4][3] = Elements::TYPE1;
    states[8]->squares[4][2] = Elements::TYPE2;
    states[8]->squares[4][1] = Elements::TYPE2;
    states[8]->squares[5][5] = Elements::TYPE1;
    states[8]->squares[5][4] = Elements::TYPE2;
    states[8]->squares[5][3] = Elements::TYPE2;
    states[8]->squares[5][2] = Elements::TYPE2;
    states[8]->squares[5][1] = Elements::TYPE1;
    states[8]->squares[5][0] = Elements::TYPE2;
    states[8]->squares[6][5] = Elements::TYPE1;
    states[8]->squares[6][4] = Elements::TYPE2;
    states[8]->squares[6][3] = Elements::TYPE1;
    states[8]->squares[6][2] = Elements::TYPE2;

    //Expected move
    // Column 3
    *expected[8] = *states[8];
    expected[8]->squares[3][0] = Elements::TYPE2;

    //Tenth grid
    // - X - - X - -
    // - O X O O X -
    // - X X O O O -
    // O X O X O X -
    // O X X O X O X
    // X O X O O O X

    states[9]->squares[6][5] = Elements::TYPE2;
    states[9]->squares[6][4] = Elements::TYPE2;
    states[9]->squares[5][5] = Elements::TYPE1;
    states[9]->squares[5][4] = Elements::TYPE1;
    states[9]->squares[5][3] = Elements::TYPE2;
    states[9]->squares[5][2] = Elements::TYPE1;
    states[9]->squares[5][1] = Elements::TYPE2;
    states[9]->squares[4][5] = Elements::TYPE1;
    states[9]->squares[4][4] = Elements::TYPE2;
    states[9]->squares[4][3] = Elements::TYPE1;
    states[9]->squares[4][2] = Elements::TYPE1;
    states[9]->squares[4][1] = Elements::TYPE1;
    states[9]->squares[4][0] = Elements::TYPE2;
    states[9]->squares[3][5] = Elements::TYPE1;
    states[9]->squares[3][4] = Elements::TYPE1;
    states[9]->squares[3][3] = Elements::TYPE2;
    states[9]->squares[3][2] = Elements::TYPE1;
    states[9]->squares[3][1] = Elements::TYPE1;
    states[9]->squares[2][5] = Elements::TYPE2;
    states[9]->squares[2][4] = Elements::TYPE2;
    states[9]->squares[2][3] = Elements::TYPE1;
    states[9]->squares[2][2] = Elements::TYPE2;
    states[9]->squares[2][1] = Elements::TYPE2;
    states[9]->squares[1][5] = Elements::TYPE1;
    states[9]->squares[1][4] = Elements::TYPE2;
    states[9]->squares[1][3] = Elements::TYPE2;
    states[9]->squares[1][2] = Elements::TYPE2;
    states[9]->squares[1][1] = Elements::TYPE1;
    states[9]->squares[1][0] = Elements::TYPE2;
    states[9]->squares[0][5] = Elements::TYPE2;
    states[9]->squares[0][4] = Elements::TYPE1;
    states[9]->squares[0][3] = Elements::TYPE1;

    //Expected move
    // Column 0
    *expected[9] = *states[9];
    expected[9]->squares[6][3] = Elements::TYPE2;

    //Eleventh grid
    // - X - - X X O
    // - O X O O O X
    // X X X O O O X
    // O X O X O X O
    // X X X O X O X
    // O O X O O O X

    states[10]->squares[6][5] = Elements::TYPE2;
    states[10]->squares[6][4] = Elements::TYPE2;
    states[10]->squares[6][3] = Elements::TYPE1;
    states[10]->squares[6][2] = Elements::TYPE2;
    states[10]->squares[6][1] = Elements::TYPE2;
    states[10]->squares[6][0] = Elements::TYPE1;
    states[10]->squares[5][5] = Elements::TYPE1;
    states[10]->squares[5][4] = Elements::TYPE1;
    states[10]->squares[5][3] = Elements::TYPE2;
    states[10]->squares[5][2] = Elements::TYPE1;
    states[10]->squares[5][1] = Elements::TYPE1;
    states[10]->squares[5][0] = Elements::TYPE2;
    states[10]->squares[4][5] = Elements::TYPE1;
    states[10]->squares[4][4] = Elements::TYPE2;
    states[10]->squares[4][3] = Elements::TYPE1;
    states[10]->squares[4][2] = Elements::TYPE1;
    states[10]->squares[4][1] = Elements::TYPE1;
    states[10]->squares[4][0] = Elements::TYPE2;
    states[10]->squares[3][5] = Elements::TYPE1;
    states[10]->squares[3][4] = Elements::TYPE1;
    states[10]->squares[3][3] = Elements::TYPE2;
    states[10]->squares[3][2] = Elements::TYPE1;
    states[10]->squares[3][1] = Elements::TYPE1;
    states[10]->squares[2][5] = Elements::TYPE2;
    states[10]->squares[2][4] = Elements::TYPE2;
    states[10]->squares[2][3] = Elements::TYPE1;
    states[10]->squares[2][2] = Elements::TYPE2;
    states[10]->squares[2][1] = Elements::TYPE2;
    states[10]->squares[1][5] = Elements::TYPE1;
    states[10]->squares[1][4] = Elements::TYPE2;
    states[10]->squares[1][3] = Elements::TYPE2;
    states[10]->squares[1][2] = Elements::TYPE2;
    states[10]->squares[1][1] = Elements::TYPE1;
    states[10]->squares[1][0] = Elements::TYPE2;
    states[10]->squares[0][5] = Elements::TYPE1;
    states[10]->squares[0][4] = Elements::TYPE2;
    states[10]->squares[0][3] = Elements::TYPE1;
    states[10]->squares[0][2] = Elements::TYPE2;

    //Expected move
    // Column 0
    *expected[10] = *states[10];
    expected[10]->squares[3][0] = Elements::TYPE2;

    //Twelvth grid
    // - - - - - - -
    // - - - - - - -
    // - - - - - - -
    // - - - - - - X
    // - - - - - X X
    // - - - - O O O

    states[11]->squares[6][5] = Elements::TYPE1;
    states[11]->squares[6][4] = Elements::TYPE2;
    states[11]->squares[6][3] = Elements::TYPE2;
    states[11]->squares[5][5] = Elements::TYPE1;
    states[11]->squares[5][4] = Elements::TYPE2;
    states[11]->squares[4][5] = Elements::TYPE1;

    //Expected move
    // Column 3
    *expected[11] = *states[11];
    expected[11]->squares[3][5] = Elements::TYPE2;

#ifdef DEBUG_UNIT_TESTS

    ConnectFourRulesEngine re;
    for(int z = 0; z < C4_NUM_MOVE_BLOCKER_TEST_STATES; ++z)
    {
        cout << "Test state " << z + 1 << endl;
        for(int y = 0; y < C4_HEIGHT; ++y)
        {
            for(int x = 0; x < C4_WIDTH; ++x)
            {
                cout << states[z]->squares[x][y] << ' ';
            }

            cout << '\t';

            for(int x = 0; x < C4_WIDTH; ++x)
            {
                cout << expected[z]->squares[x][y] << ' ';
            }

            cout << endl;
        }
        if(re.testBoard(states[z]) != Elements::NORMAL)
        {
            cout << "This is an end-state.  Fix it.\n";
        }
        cout << endl;
    }


#endif
}

bool testC4MoveBlocker()
{
    bool didPass = true;

    ConnectFourRulesEngine *re = new ConnectFourRulesEngine();
    //ConnectFourTrainer trainer(10, re);

    //Test grid creation.
    Grid **states;
    Grid **expected;

    createBoardStateList(states, expected);
    //Create board states to contain the generated game grids.
    BoardState **boardStates = new BoardState*[C4_NUM_MOVE_BLOCKER_TEST_STATES];
    BoardState **resultStates = new BoardState*[C4_NUM_MOVE_BLOCKER_TEST_STATES];

    for(int x = 0; x < C4_NUM_MOVE_BLOCKER_TEST_STATES; ++x)
    {
        boardStates[x] = new BoardState(states[x], NULL, Elements::PLAYER_2, re, 1);
    }

    for(int x = 0; x < C4_NUM_MOVE_BLOCKER_TEST_STATES; ++x)
    {
        cout << "Testing pattern " << x + 1 << endl;

        resultStates[x] = ConnectFourTrainer::moveBlocker(boardStates[x], Elements::PLAYER_2, Elements::PLAYER_1);

        if(*(resultStates[x]->getCurrentGrid()) != *(expected[x]))
        {
            didPass = false;
            cout << "\tTest failed\n";
        }
        else
        {
            cout << "\tTest passed\n";
        }
    }

#ifdef DEBUG_UNIT_TESTS
    for(int z = 0; z < C4_NUM_MOVE_BLOCKER_TEST_STATES; ++z)
    {
        cout << "Test state " << z + 1 << endl;
        for(int w = 0; w < C4_HEIGHT; ++w)
        {
            for(int v = 0; v < C4_WIDTH; ++v)
            {
                cout << resultStates[z]->getCurrentGrid()->squares[v][w] << ' ';
            }

            cout << '\t';

            for(int v = 0; v < C4_WIDTH; ++v)
            {
                cout << expected[z]->squares[v][w] << ' ';
            }

            cout << endl;
        }
        cout << endl;
    }
#endif


    //Delete locally created data.
    for(int x = 0; x < C4_NUM_MOVE_BLOCKER_TEST_STATES; ++x)
    {
        delete expected[x];
        delete boardStates[x];
    }

    delete [] states;
    delete [] expected;
    delete [] boardStates;
    delete [] resultStates;

    delete re;

    return didPass;
}
