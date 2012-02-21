#include "connectfourrulesengine.h"

Elements::GameState ConnectFourRulesEngine::testBoard(const Grid *boardState) const
{
    //Test for empty in the top row.
    //Test for diagonal and vertical wins if top row is not empty.

    //First check the left side of the board.  It will only support left-facing and vertical wins.
    for(int x = 0; x < 3; ++x)
    {
        if(boardState->squares[x][0] != Elements::EMPTY)
        {
            //Vertical
            if(isAWin(boardState->squares[x][0], boardState->squares[x][1], boardState->squares[x][2], boardState->squares[x][3]) ||
               /* angled */ isAWin(boardState->squares[x][0], boardState->squares[x + 1][1], boardState->squares[x + 2][2], boardState->squares[x + 3][3]))
            {
                //Return the value of the winning square.
                return (Elements::GameState)boardState->squares[x][0];
            }
        }
        //Test for horizontal wins.
            //Test each set of four horizontal spaces simultaneously.
            //Move down one row and try again.
            //Repeat.
        //No need to test beyond the third square, as with the diagonals.
        for(int y = 0; y < 3; ++y)
        {
            if(isAWin(boardState->squares[x][y], boardState->squares[x + 1][y], boardState->squares[x + 2][y], boardState->squares[x + 3][y]))
            {
                return (Elements::GameState)boardState->squares[x][y];
            }
        }
    }

    //Then check the right side.  It will only support right-facing and vertical wins.
    for(int x = 3; x < 6; ++x)
    {

        if(boardState->squares[x][0] != Elements::EMPTY)
        {
            //Vertical
            if(isAWin(boardState->squares[x][0], boardState->squares[x][1], boardState->squares[x][2], boardState->squares[x][3]) ||
               /* angled */ isAWin(boardState->squares[x][0], boardState->squares[x - 1][1], boardState->squares[x - 2][2], boardState->squares[x - 3][3]))
            {
                //Return the value of the winning square.
                return (Elements::GameState)boardState->squares[x][0];
            }
        }

    }

    //if no win was found and there are no empty spaces, the game is over.
    if(boardState->numPiecesOfType(Elements::EMPTY) == 0)
        return Elements::DRAW;

    return Elements::NORMAL;
}

bool ConnectFourRulesEngine::isAWin(Elements::GenericPieceType sq1, Elements::GenericPieceType sq2, Elements::GenericPieceType sq3, Elements::GenericPieceType sq4) const
{
    return (sq1 != Elements::EMPTY && sq1 == sq2 && sq1 == sq3 && sq1 == sq4) ? true : false;
}

bool ConnectFourRulesEngine::isValidMove(const Grid *currentState, const Grid *nextState, Elements::PlayerType currentPlayer) const
{
    std::cout << currentState->numPiecesOfType(Elements::EMPTY) << " " << nextState->numPiecesOfType(Elements::EMPTY) << std::endl;
    if(currentState->numPiecesOfType(Elements::EMPTY) != nextState->numPiecesOfType(Elements::EMPTY) + 1)
        return false;

    //Player 1 always goes first.  That means that at any time in the game, player 1 will have either the same number of pieces or one more than player two.
    //Implement this if the issue comes up; the current interface doesn't allow one player to move twice in a row.
    //For now, it's not worth the clock cycles.
    /*if(nextState->numPiecesOfType(Elements::PLAYER_1) == next)*/

    //Check each row to see if the move made sense.
    //connect four is gravity based, meaning that pieces will fall to the bottom of each stack.
    //Should not take for granted that this will happen with the inputs.
    //Chech each column and the squares within it.
    for(int x = 0; x < currentState->width; ++x)
    {
        bool isEmpty = false;
        //Start at the base of the column and go up.  The grid is represented with 0 as the top.
        for(int y = currentState->height - 1; y >= 0; --y)
        {
            //If there is an empty square at any point, mark that column as empty.
            if(nextState->squares[x][y] == Elements::EMPTY)
            {
                isEmpty = true;
            }
            //If a square is later found to be not empty, this column is invalid.
            //Therefore, the entire state is invalid.
            else if(nextState->squares[x][y] != Elements::EMPTY && isEmpty)
            {
                return false;
            }
        }
    }
    return true;
}

void ConnectFourRulesEngine::genNextMoves(const Grid *current, Grid **&nextMoves, Elements::PlayerType currentPlayer, int &numNextStates) const
{
    numNextStates = 0;

    int nextStateIndex = 0;

    //If the first square in any column is empty, that column can be moved to.
    for(int x = 0; x < 6; ++x)
    {
        if(current->squares[x][0] == Elements::EMPTY)
            ++numNextStates;
    }

    #ifdef DEBUG_C4GENNEXTMOVES
    printLine3("There are ", numNextStates, " legal moves from this state.");
    #endif
    //Initialize the array of next possible states.
    nextMoves = new Grid*[numNextStates];

    for(int x = 0; x < numNextStates; ++x)
    {
        nextMoves[x] = new ConnectFourGrid();

        *nextMoves[x] = *current;
    }

    int x = 0;
    while(nextStateIndex < numNextStates)
    {
        //If the column is not full, update that grid with the current column number.
        if(nextMoves[nextStateIndex]->squares[x][0] == Elements::EMPTY)
        {
            //Update the grid.
            updateGrid(nextMoves[nextStateIndex], x, currentPlayer);
            //Increment the state index so the next state in the array can be modified.

            ++nextStateIndex;
        }
        ++x;
    }

#ifdef DEBUG_C4GENNEXTMOVES
    //Display the board.
    for(int y = 0; y < 4; ++y)
    {
        nextStateIndex = 0;
        while(nextStateIndex < numNextStates)
        {
            for(int x = 0; x < 6; ++x)
            {
                print2(nextMoves[nextStateIndex]->squares[x][y], " ");
            }
            print("\t");
            ++nextStateIndex;
        }
        print("\n");
    }
    print("\n\n");
#endif
}

void ConnectFourRulesEngine::updateGrid(Grid *grid, const double *outputs, Elements::PlayerType player) const
{
    //Outputs is expected to contain a column value.
    int move = round(*outputs);

    if(move >= 0 && move <= 5)
    {
        //Find the lowest open space in the grid.
        int row = getFirstOpenSpace(grid, move);
#ifdef DEBUG_C4RE
        for(int y = 0; y < 4; ++y)
            printLine5(move, ", ", y, " = ", grid->squares[move][y]);
#endif

        //This column must not already be full.
        if(row != -1)
            grid->squares[move][row] = (Elements::GenericPieceType)player;
    }
}

void ConnectFourRulesEngine::updateGrid(Grid *grid, int output, Elements::PlayerType player) const
{

    if(output >= 0 && output <= 5)
    {
        //Find the lowest open space in the grid.
        int row = getFirstOpenSpace(grid, output);
#ifdef DEBUG_C4RE
        for(int y = 0; y < 4; ++y)
            printLine5(output, ", ", y, " = ", grid->squares[move][y]);
#endif

        //This column must not already be full.
        if(row != -1)
            grid->squares[output][row] = (Elements::GenericPieceType)player;
    }
}

int ConnectFourRulesEngine::getFirstOpenSpace(const Grid *grid, int column) const
{
    for(int y = 3; y >=0; --y)
    {
        if(grid->squares[column][y] == Elements::EMPTY)
            return y;
    }

    //Did not find a row.
    return -1;
}

bool ConnectFourRulesEngine::gridToDoubleArray(const Grid *grid, double *&array, Elements::PlayerType player, int index, const int numElements) const
{
    //Iterate over each space, assuming there is enough space in the receiving array.
    if(numElements - index >= 24)
    {
        for(int x = 0; x < 6; ++x)
        {
            for(int y = 0; y < 3; ++y)
            {
                //Convert the square to a double and copy it over.
                array[index] = pieceTypeToDouble(player, grid->squares[x][y]);
                ++index;
            }
        }
        //Copy succeeded
        return true;
    }
    else
    {
        //Copy failed
        return false;
    }
}

bool ConnectFourRulesEngine::doubleArrayToGrid(Grid *&grid, const double *array, Elements::PlayerType player, int index, const int numElements) const
{
    //Make sure the array is large enough.
    if(numElements - index >= 24)
    {
        for(int x = 0; x < 6; ++x)
        {
            for(int y = 0; y < 4; ++y)
            {
                //Convert each value to a piece type.
                grid->squares[x][y] = doubleToPieceType(player, array[index]);
                //Increment the array index.
                ++index;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
    return true;
}

//Convert a piece type to its double precision equivalent, relative to the current player.
double ConnectFourRulesEngine::pieceTypeToDouble(Elements::PlayerType player, Elements::GenericPieceType piece) const
{
    //Blank space.
    if(piece == Elements::EMPTY)
        return EMPTY_SQUARE;
    //Player's square.
    else if(piece == (Elements::GenericPieceType)player)
        return FRIENDLY_PIECE;
    //Opponent's square.
    else
        return OPPONENT_PIECE;
}

//Convert a double precision value to its piece type equivalent, relative to the current player.
Elements::GenericPieceType ConnectFourRulesEngine::doubleToPieceType(Elements::PlayerType player, double value) const
{
    //If input is too low to be an Elements::EMPTY value,
    //It's for an opponent.
    if(value < EMPTY_LOWER_BOUND)
    {
        if(player == Elements::PLAYER_1)
            return Elements::TYPE2;
        else
            return Elements::TYPE1;
    }
    //If it's lower than the value for a friendly space,
    //It's Elements::EMPTY.
    else if(player <= EMPTY_UPPER_BOUND)
    {
        return Elements::EMPTY;
    }
    //Otherwise, it's a friendly space.
    else
    {
        return (Elements::GenericPieceType)player;
    }
}

Grid* ConnectFourRulesEngine::createGameSpecificGrid() const
{
    return new ConnectFourGrid();
}
