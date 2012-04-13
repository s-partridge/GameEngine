#include "tictactoerulesengine.h"

bool TicTacToeRulesEngine::isValidMove(const Grid *currentState, const Grid *nextState, Elements::PlayerType currentPlayer) const
{
    //If exactly one less square is Elements::EMPTY.
    if(currentState->numPiecesOfType(Elements::EMPTY) != nextState->numPiecesOfType(Elements::EMPTY) + 1)
    {
        return false;
    }
    //and there is only one difference between the boards.
    else if(currentState->numDifferences(nextState) != 1)
    {
        return false;
    }
    //Then nextState is valid.
    return true;
}

void TicTacToeRulesEngine::genNextMoves(const Grid *current, Grid **&nextMoves, int *&lastMoves, Elements::PlayerType currentPlayer, int &numNextStates) const
{
    numNextStates = current->numPiecesOfType(Elements::EMPTY);

    if(testBoard(current) != Elements::NORMAL)
    {
        numNextStates = 0;
        return;
    }

#ifdef DEBUG_C4GENNEXTMOVES
    printLine3("There are ", numNextStates, " legal moves from this state.");
#endif

    nextMoves = new Grid*[numNextStates];
    lastMoves = new int[numNextStates];

    for(int x = 0; x < numNextStates; ++x)
    {
        nextMoves[x] = new TicTacToeGrid();
        //Deep copy of the current grid to each new grid.
        *nextMoves[x] = *current;
    }

    int stateCount = 0;
    int moveIndex = 0;

    for(int x = 0; x < 3; ++x)
    {
        for(int y = 0; y < 3; ++y)
        {
            //Keep looping through the squares.  There should be one possible move for each Elements::EMPTY
            //space.  StateCount will only increment when an Elements::EMPTY space is filled.  Once there are
            //no more spaces, the loop will break.
            if(stateCount >= numNextStates)
                return;

            if(nextMoves[stateCount]->squares[x][y] == Elements::EMPTY)
            {
                nextMoves[stateCount]->squares[x][y] = (Elements::GenericPieceType)currentPlayer;

                //Store the index of the most recently used square.
                lastMoves[stateCount] = moveIndex;
                ++stateCount;
            }

            //A simple counter to keep track of the current square.
            ++moveIndex;
        }
    }

#ifdef DEBUG_TTTGENNEXTMOVES
    //Display the board.
    for(int y = 0; y < 3; ++y)
    {
        int nextStateIndex = 0;
        while(nextStateIndex < numNextStates)
        {
            for(int x = 0; x < 3; ++x)
            {
                print2(nextMoves[nextStateIndex]->squares[x][y], " ");
            }
            print("\t");
            ++nextStateIndex;
        }
        print("\n");
    }
    printLine("\n");
#endif

    return;
}

//Change grid so it matches the move stored in outputs.
//Outputs is expected to be a move pulled from neuralNetwork.
void TicTacToeRulesEngine::updateGrid(Grid *grid, const double *outputs, Elements::PlayerType player) const
{
    //Get the integer value of the output.
    int out = round(*outputs);

    //For tic tac toe, outputs is expected to only be one value long.
    //Its value should represent the grid square to be replaced by currentPlayer.
    //Since outputs represents one of nine squares,
    if(*outputs >= 0.0 && *outputs <= 9.0)
    {
        grid->squares[out / 3][out % 3] = (Elements::GenericPieceType)player;
    }
}

Elements::GameState TicTacToeRulesEngine::testBoard(const Grid *boardState) const
{
    int x, y;

    //Check from the middle of the board out.
    if(boardState->squares[1][1] != Elements::EMPTY)
    {
        //Check the center horizontal row.
        if(boardState->squares[0][1] == boardState->squares[1][1] && boardState->squares[1][1] == boardState->squares[2][1])
            return (Elements::GameState)boardState->squares[1][1];
        //Check diagonal lines and vertical center.
        for(x = 0; x < 3; ++x)
        {
            if(boardState->squares[x][0] == boardState->squares[1][1] && boardState->squares[2 - x][2] == boardState->squares[1][1])
                //Cast the center square to Elements::GameState to simplify algorithm.
                return (Elements::GameState)boardState->squares[1][1];
        }
    }

    //Check the squares connected to the top left corner.
    if(boardState->squares[0][0] != Elements::EMPTY)
    {
        if(boardState->squares[0][1] == boardState->squares[0][0] && boardState->squares[0][2] == boardState->squares[0][0] ||
                boardState->squares[1][0] == boardState->squares[0][0] && boardState->squares[2][0] == boardState->squares[0][0])
        {
            return (Elements::GameState)boardState->squares[0][0];
        }
    }

    //Check the squares connected to the bottom right corner.
    if(boardState->squares[2][2] != Elements::EMPTY)
    {
        if(boardState->squares[2][0] == boardState->squares[2][2] && boardState->squares[2][1] == boardState->squares[2][2] ||
                boardState->squares[0][2] == boardState->squares[2][2] && boardState->squares[1][2] == boardState->squares[2][2])
        {
            return (Elements::GameState)boardState->squares[2][2];
        }
    }

    //Check for cat's game.
    for(x = 0; x < 3; ++x)
    {
        for(y = 0; y < 3; ++y)
        {
            //This is true because if a player had three in a row, it would have been caught before this loop ran.
            if(boardState->squares[x][y] == Elements::EMPTY)
                return Elements::NORMAL;
        }
    }

    //This will only occur if all the above checks fail.
    return Elements::DRAW;
}

float TicTacToeRulesEngine::worthOfState(const Grid *boardState, Elements::PlayerType currentPlayer, Elements::GameState currentState) const
{
    if(currentState != Elements::NORMAL)
    {
        //Board state is an end game condition.
        //Find whether the player won, lost, or tied.
        return endStateWorth(currentPlayer, currentState);
    }
    else
    {
        //Perform a series of checks for specific patterns in the board state.
        return normalStateWorth(boardState, currentPlayer);
    }
}

float TicTacToeRulesEngine::endStateWorth(Elements::PlayerType currentPlayer, Elements::GameState currentState) const
{
    //This is a winning state.
    if(currentState == (Elements::GameState)currentPlayer)
    {
        //A good move.
        return 1.0;
    }
    //This is a losing state.
    else if(currentState != (Elements::GameState)currentPlayer && currentState != Elements::DRAW)
    {
        //A terrible move.
        return 0.0;
    }
    //Cat's game
    else
    {
        //Nobody wins.  It's a neutral move.
        return 0.5;
    }
}

//Find the worth of a state that is not an end game condition.
float TicTacToeRulesEngine::normalStateWorth(const Grid *boardState, Elements::PlayerType currentPlayer) const
{
    float stateWorth;
    //Store the number of empty spaces.
    int empty = boardState->numPiecesOfType(Elements::EMPTY);

    //Elements::EMPTY board
    if(empty == 9)
    {
        stateWorth = 0.5;
    }
    //First move
    else if(empty == 8)
    {
        stateWorth = firstMoveWorth(boardState, currentPlayer);
    }
    //Second move
    else if(empty >= 7)
    {
        stateWorth = secondMoveWorth(boardState, currentPlayer);
    }
    //Third or later.
    else
    {
        stateWorth = laterMoveWorth(boardState, currentPlayer);
    }

    return stateWorth;
}

//Find the worth of a player's first move.
float TicTacToeRulesEngine::firstMoveWorth(const Grid *boardState, Elements::PlayerType currentPlayer) const
{
    //X, Y
    int x, y;
    for(x = 0; x < 3; ++x)
    {
        for(y = 0; y < 3; ++y)
        {
            if(boardState->squares[x][y] == (Elements::GenericPieceType)currentPlayer)
            {
                //Store the x-y coord of the move.
                //this will only break the inner for loop.
                //I can either store the x and y values,
                //flip a boolean, or use a goto statement
                //to get out of both loops.

                //A goto statement would be the most efficient
                //option in this case, as it requires no extra
                //memory and assembles down into a single operation.
                //There would also be no stack corruption because It's
                //not leaving this function.
                goto LABEL_FINISH_XY_LOOP;
            }
        }
    }

    LABEL_FINISH_XY_LOOP:

    //Center square.
    if(x == 1 && y == 1)
    {
        //Very good first move.
        return 0.9;
    }
    //Not corner or center.
    else if((x == 1 && y != 1) || (x != 1 && y == 1))
    {
        //Terrible first move.
        //Any side is equally bad.
        return 0.1;
    }
    //One of the corner squares
    else
    {
        //Not a great move, but not too bad, either.
        //Any corner is equally neutral.
        return 0.5;
    }
}

//Find the worth of a player's second move.
float TicTacToeRulesEngine::secondMoveWorth(const Grid *boardState, Elements::PlayerType currentPlayer) const
{
    //x1 and y1 are used for the opponent.
    //x2 and y2 are used for the player.
    int x1, x2, y1, y2;

    //Get opponent's position.
    for(x1 = 0; x1 < 3; ++x1)
    {
        for(y1 = 0; y1 < 3; ++y1)
        {
            //Break if the opponent's spot is found.
            //This will keep the current x and y coords.
            if(boardState->squares[x1][y1] != Elements::EMPTY &&
               boardState->squares[x1][y1] != (Elements::GenericPieceType)currentPlayer)
            {
                goto LABEL_FINISH_OPPONENT_LOOP;
            }
        }
    }

    LABEL_FINISH_OPPONENT_LOOP:

    //Get player's position.
    for(x2 = 0; x2 < 3; ++x2)
    {
        for(y2 = 0; y2 < 3; ++y2)
        {
            //Break if the player's spot is found.
            //This will keep the current x and y coords.
            if(boardState->squares[x2][y2] == (Elements::GenericPieceType)currentPlayer)
            {
                goto LABEL_FINISH_PLAYER_LOOP;
            }
        }
    }

    LABEL_FINISH_PLAYER_LOOP:

    //Enemy took center.
    if(x1 == 1 && y1 == 1)
    {
        //Terrible move.
        //All side centers are equal.
        if((x2 == 1 && y2 != 1) || (x2 != 1 && y2 == 1))
        {
            return 0.1;
        }
        //Best move you can make.
        //All corners are equal.
        else
        {
            //Good, but not as good as being able to take center.
            return 0.8;
        }
    }
    //Otherwise, the player should take center.
    else
    {
        if(x2 == 1 && y2 == 1)
        {
            return 0.9;
        }
        //Took a center side.
        else if((x2 == 1 && y2 != 1) || (x2 != 1 && y2 == 1))
        {
            //Same side as opponent.  A potential block.
            if(x2 == x1 || y2 == y1)
            {
                //Better than nothing.
                return 0.6;
            }
            else
            {
                //If they're on the opposite side,
                //a terrible move.
                return 0.1;
            }
        }
        //Took a corner
        else
        {
            //Same side.  A potential block and better than a center side.
            if(x2 == x1 || y2 == y1)
            {
                return 0.7;
            }
            //Opposite side.  Not very good, but better than opposite center.
            else
            {
                return 0.3;
            }
        }
    }
}

//Find the worth of a player's move at third or later.
float TicTacToeRulesEngine::laterMoveWorth(const Grid *boardState, Elements::PlayerType currentPlayer) const
{
    Elements::PlayerType opponent;
    if(currentPlayer == Elements::PLAYER_1)
    {
        opponent = Elements::PLAYER_2;
    }
    else
    {
        opponent = Elements::PLAYER_1;
    }

    int opponentOpenCount = countDoubleSpace(boardState, (Elements::GenericPieceType)opponent, false);
    int playerOpenCount = countDoubleSpace(boardState, (Elements::GenericPieceType)currentPlayer, false);
    int opponentBlockedCount = countDoubleSpace(boardState, (Elements::GenericPieceType)opponent, true);
    int playerBlockedCount = countDoubleSpace(boardState, (Elements::GenericPieceType)currentPlayer, true);
    //The player should not allow this to happen.
    //If the turn ends with an opponent's open count > 0, the game is over.
    if(opponentOpenCount > 0)
    {
        //Return a value based on how many sets of two the opponent has managed.
        //It is not possible for this value to be more than 4.
        return 0.3 - opponentOpenCount / 10.0f;
    }
    else if(playerOpenCount > 0)
    {
        //Return a value based on how many sets of two the player has managed.
        //It is not possible for this value to be more than 4.
        return 0.7 + playerOpenCount / 10.0f;
    }
    else
    {
        //Return a value based on the number of moves the player has blocked.
        //Subtract the number of places the opponent has blocked from that value.
        return 0.5 + (opponentBlockedCount - playerBlockedCount) / 10.0f;
    }
}


//Count two-in-a-row for the given player type.
int TicTacToeRulesEngine::countDoubleSpace(const Grid *boardState, Elements::GenericPieceType toCount, bool countBlocked) const
{
    int x, y;
    int count = 0;
    //Check for diagonal and vertical first.
    for(x = 0; x < 3; ++x)
    {
        //Don't do this for the top center.  Avoid doing the vertical calculation twice.
        if(x != 1)
        {
            if((boardState->squares[x][0] == toCount && boardState->squares[x][1] == toCount && boardState->squares[x][2] != toCount) ||
               (boardState->squares[x][0] == toCount && boardState->squares[x][1] != toCount && boardState->squares[x][2] == toCount) ||
               (boardState->squares[x][0] != toCount && boardState->squares[x][1] == toCount && boardState->squares[x][2] == toCount) )
            {
                if(countBlocked)
                    //Increment number of sets only if one of these is not Elements::EMPTY and is not the type to count.
                    if((boardState->squares[x][0] != Elements::EMPTY && boardState->squares[x][0] != toCount) ||
                       (boardState->squares[x][1] != Elements::EMPTY && boardState->squares[x][1] != toCount) ||
                       (boardState->squares[x][2] != Elements::EMPTY && boardState->squares[x][2] != toCount) )
                        ++count;
                else
                {
                    //If counting open sets, only increment if one of these is Elements::EMPTY.
                    if(boardState->squares[x][0] == Elements::EMPTY || boardState->squares[x][1] == Elements::EMPTY || boardState->squares[x][2] == Elements::EMPTY)
                        //Increment number of sets.
                        ++count;
                }
            }
        }
        //If top == mid & bottom is different or
        //if top == bottom & mid is different or
        //if mid == bottom & top is different
        if( (boardState->squares[x][0] == toCount && boardState->squares[1][1] == toCount && boardState->squares[2 - x][2] != toCount) ||
            (boardState->squares[x][0] == toCount && boardState->squares[1][1] != toCount && boardState->squares[2 - x][2] == toCount) ||
            (boardState->squares[x][0] != toCount && boardState->squares[1][1] == toCount && boardState->squares[2 - x][2] == toCount) )
        {
            if(countBlocked)
                //Increment number of sets.
                if((boardState->squares[x][0] != Elements::EMPTY && boardState->squares[x][0] != toCount ) ||
                   (boardState->squares[1][1] != Elements::EMPTY && boardState->squares[1][1] != toCount ) ||
                   (boardState->squares[2 - x][2] != Elements::EMPTY && boardState->squares[2 - x][2] != toCount ) )
                    ++count;
            else
            {
                //If counting open sets, only increment if one of these is Elements::EMPTY.
                if(boardState->squares[x][0] == Elements::EMPTY || boardState->squares[2 - x][2] == Elements::EMPTY || boardState->squares[1][1] == Elements::EMPTY)
                    //Increment number of sets.
                    ++count;
            }
        }
    }

    //Next, check for horizontal sets.
    for(y = 0; y < 3; ++y)
    {
        if((boardState->squares[0][y] == toCount && boardState->squares[1][y] == toCount && boardState->squares[2][y] != toCount) ||
           (boardState->squares[0][y] == toCount && boardState->squares[1][y] != toCount && boardState->squares[2][y] == toCount) ||
           (boardState->squares[0][y] != toCount && boardState->squares[1][y] == toCount && boardState->squares[2][y] == toCount) )
        {
            if(countBlocked)
            {
                //Increment number of sets.
                if(boardState->squares[0][y] == Elements::EMPTY || boardState->squares[1][y] == Elements::EMPTY || boardState->squares[2][y] == Elements::EMPTY)
                    ++count;
            }
            else if(!countBlocked)
            {
                //If counting open sets, only increment if one of these is Elements::EMPTY.
                if(boardState->squares[0][y] == Elements::EMPTY || boardState->squares[1][y] == Elements::EMPTY || boardState->squares[2][y] == Elements::EMPTY)
                    //Increment number of sets.
                    ++count;
            }
        }
    }

    return count;
}

bool TicTacToeRulesEngine::gridToDoubleArray(const Grid *grid, double *&array, Elements::PlayerType player, int index, const int numElements) const
{
    //Iterate over each space, assuming there is enough space in the receiving array.
    if(9 <= numElements - index)
    {
        //It's tic-tac-toe.  These numbers will never change.
        for(int x = 0; x < 3; ++x)
        {
            for(int y = 0; y < 3; ++y)
            {
                //Convert the square to a double precision value.
                array[index] = pieceTypeToDouble(player, grid->squares[x][y]);
                ++index;
            }
        }
        //Copy succeeded.
        return true;
    }
    else
    {
        //Copy failed.
        return false;
    }
}

bool TicTacToeRulesEngine::doubleArrayToGrid(Grid *&grid, const double *array, Elements::PlayerType player, int index, const int numElements) const
{
    //Make sure the array is large enough.
    if(numElements - index > 9)
    {
        for(int x = 0; x < 3; ++x)
        {
            for(int y = 0; y < 3; ++y)
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
}

//Convert a piece type to its double precision equivalent, relative to the current player.
double TicTacToeRulesEngine::pieceTypeToDouble(Elements::PlayerType player, Elements::GenericPieceType piece) const
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
Elements::GenericPieceType TicTacToeRulesEngine::doubleToPieceType(Elements::PlayerType player, double value) const
{
    //If input is too low to be an Elements::EMPTY value,
    //It's for an opponent.
    if(value < EMPTY_LOWER_BOUND)
    {
        if(player == Elements::PLAYER_1)
            return (Elements::GenericPieceType)TicTacToeElements::O_PIECE;
        else
            return (Elements::GenericPieceType)TicTacToeElements::X_PIECE;
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
        if(player == Elements::PLAYER_1)
            return (Elements::GenericPieceType)TicTacToeElements::X_PIECE;
        else
            return (Elements::GenericPieceType)TicTacToeElements::O_PIECE;
    }
}

Grid *TicTacToeRulesEngine::createGameSpecificGrid() const
{
    //Create a new grid.
    TicTacToeGrid *newGrid = new TicTacToeGrid();
    //Initialize it as Elements::EMPTY.
    newGrid->init();

    return newGrid;
}
