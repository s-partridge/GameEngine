#include "tictactoerulesengine.h"

#include "TicTacToeRulesEngine.h"

GameState TicTacToeRulesEngine::testBoard(const Grid &boardState)
{
    int x, y;

    //Check from the middle of the board out.
    if(boardState.squares[1][1] != EMPTY)
    {
        //Check the center horizontal row.
        if(boardState.squares[0][1] == boardState.squares[1][1] && boardState.squares[1][1] == boardState.squares[2][1])
            return (GameState)boardState.squares[1][1];
        //Check diagonal lines and vertical center.
        for(x = 0; x < 3; ++x)
        {
            if(boardState.squares[x][0] == boardState.squares[1][1] && boardState.squares[2 - x][2] == boardState.squares[1][1])
                //Cast the center square to GameState to simplify algorithm.
                return (GameState)boardState.squares[1][1];
        }
    }

    //Check the squares connected to the top left corner.
    if(boardState.squares[0][0] != EMPTY)
    {
        if(boardState.squares[0][1] == boardState.squares[0][0] && boardState.squares[0][2] == boardState.squares[0][0] ||
                boardState.squares[1][0] == boardState.squares[0][0] && boardState.squares[2][0] == boardState.squares[0][0])
        {
            return (GameState)boardState.squares[0][0];
        }
    }

    //Check the squares connected to the bottom right corner.
    if(boardState.squares[2][2] != EMPTY)
    {
        if(boardState.squares[2][0] == boardState.squares[2][2] && boardState.squares[2][1] == boardState.squares[2][2] ||
                boardState.squares[0][2] == boardState.squares[2][2] && boardState.squares[1][2] == boardState.squares[2][2])
        {
            return (GameState)boardState.squares[0][0];
        }
    }

    //Check for cat's game.
    for(x = 0; x < 3; ++x)
    {
        for(y = 0; y < 3; ++y)
        {
            //This is true because if a player had three in a row, it would have been caught before this loop ran.
            if(boardState.squares[x][y] == EMPTY)
                return NORMAL;
        }
    }

    //This will only occur if all the above checks fail.
    return CATSGAME;
}

float TicTacToeRulesEngine::worthOfState(const Grid &boardState, PlayerType currentPlayer, GameState currentState)
{
    if(currentState != NORMAL)
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

float TicTacToeRulesEngine::endStateWorth(PlayerType currentPlayer, GameState currentState)
{
    //This is a winning state.
    if(currentState == (GameState)currentPlayer)
    {
        //A good move.
        return 1.0;
    }
    //This is a losing state.
    else if(currentState != (GameState)currentPlayer && currentState != CATSGAME)
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
float TicTacToeRulesEngine::normalStateWorth(const Grid &boardState, PlayerType currentPlayer)
{
    float stateWorth;
    //Store the number of empty spaces.
    int numEmpty = boardState.numSquaresOfType(EMPTY);

    //Empty board
    if(numEmpty == 9)
    {
        stateWorth = 0.5;
    }
    //First move
    else if(numEmpty == 8)
    {
        stateWorth = firstMoveWorth(boardState, currentPlayer);
    }
    //Second move
    else if(numEmpty >= 7)
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
float TicTacToeRulesEngine::firstMoveWorth(const Grid &boardState, PlayerType currentPlayer)
{
    //X, Y
    int x, y;
    for(x = 0; x < 3; ++x)
    {
        for(y = 0; y < 3; ++y)
        {
            if(boardState.squares[x][y] == currentPlayer)
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
float TicTacToeRulesEngine::secondMoveWorth(const Grid &boardState, PlayerType currentPlayer)
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
            if(boardState.squares[x1][y1] != EMPTY && boardState.squares[x1][y1] != currentPlayer)
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
            if(boardState.squares[x2][y2] == currentPlayer)
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
float TicTacToeRulesEngine::laterMoveWorth(const Grid &boardState, PlayerType currentPlayer)
{
    PlayerType opponent;
    if(currentPlayer == XPLAYER)
    {
        opponent = OPLAYER;
    }
    else
    {
        opponent = XPLAYER;
    }

    int opponentOpenCount = countDoubleSpace(boardState, opponent, false);
    int playerOpenCount = countDoubleSpace(boardState, currentPlayer, false);
    int opponentBlockedCount = countDoubleSpace(boardState, opponent, true);
    int playerBlockedCount = countDoubleSpace(boardState, currentPlayer, true);
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
int TicTacToeRulesEngine::countDoubleSpace(const Grid &boardState, PlayerType toCount, bool countBlocked)
{
    int x, y;
    int count = 0;
    //Check for diagonal and vertical first.
    for(x = 0; x < 3; ++x)
    {
        //Don't do this for the top center.  Avoid doing the vertical calculation twice.
        if(x != 1)
        {
            if((boardState.squares[x][0] == toCount && boardState.squares[x][1] == toCount && boardState.squares[x][2] != toCount) ||
               (boardState.squares[x][0] == toCount && boardState.squares[x][1] != toCount && boardState.squares[x][2] == toCount) ||
               (boardState.squares[x][0] != toCount && boardState.squares[x][1] == toCount && boardState.squares[x][2] == toCount) )
            {
                if(countBlocked)
                    //Increment number of sets only if one of these is not empty and is not the type to count.
                    if((boardState.squares[x][0] != EMPTY && boardState.squares[x][0] != toCount) ||
                       (boardState.squares[x][1] != EMPTY && boardState.squares[x][1] != toCount) ||
                       (boardState.squares[x][2] != EMPTY && boardState.squares[x][2] != toCount) )
                        ++count;
                else
                {
                    //If counting open sets, only increment if one of these is empty.
                    if(boardState.squares[x][0] == EMPTY || boardState.squares[x][1] == EMPTY || boardState.squares[x][2] == EMPTY)
                        //Increment number of sets.
                        ++count;
                }
            }
        }
        //If top == mid & bottom is different or
        //if top == bottom & mid is different or
        //if mid == bottom & top is different
        if( (boardState.squares[x][0] == toCount && boardState.squares[1][1] == toCount && boardState.squares[2 - x][2] != toCount) ||
            (boardState.squares[x][0] == toCount && boardState.squares[1][1] != toCount && boardState.squares[2 - x][2] == toCount) ||
            (boardState.squares[x][0] != toCount && boardState.squares[1][1] == toCount && boardState.squares[2 - x][2] == toCount) )
        {
            if(countBlocked)
                //Increment number of sets.
                if((boardState.squares[x][0] != EMPTY && boardState.squares[x][0] != toCount ) ||
                   (boardState.squares[1][1] != EMPTY && boardState.squares[1][1] != toCount ) ||
                   (boardState.squares[2 - x][2] != EMPTY && boardState.squares[2 - x][2] != toCount ) )
                    ++count;
            else
            {
                //If counting open sets, only increment if one of these is empty.
                if(boardState.squares[x][0] == EMPTY || boardState.squares[2 - x][2] == EMPTY || boardState.squares[1][1] == EMPTY)
                    //Increment number of sets.
                    ++count;
            }
        }
    }

    //Next, check for horizontal sets.
    for(y = 0; y < 3; ++y)
    {
        if((boardState.squares[0][y] == toCount && boardState.squares[1][y] == toCount && boardState.squares[2][y] != toCount) ||
           (boardState.squares[0][y] == toCount && boardState.squares[1][y] != toCount && boardState.squares[2][y] == toCount) ||
           (boardState.squares[0][y] != toCount && boardState.squares[1][y] == toCount && boardState.squares[2][y] == toCount) )
        {
            if(countBlocked)
            {
                //Increment number of sets.
                if(boardState.squares[0][y] == EMPTY || boardState.squares[1][y] == EMPTY || boardState.squares[2][y] == EMPTY)
                    ++count;
            }
            else if(!countBlocked)
            {
                //If counting open sets, only increment if one of these is empty.
                if(boardState.squares[0][y] == EMPTY || boardState.squares[1][y] == EMPTY || boardState.squares[2][y] == EMPTY)
                    //Increment number of sets.
                    ++count;
            }
        }
    }

    return count;
}

