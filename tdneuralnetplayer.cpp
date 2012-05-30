#include "tdneuralnetplayer.h"

PastWeightMatrix::PastWeightMatrix(int numRounds, int numOutputs)
    : previousOutputs(NULL), idealOutputs(NULL)
{
    resetArrays(numRounds, numOutputs);
}

void PastWeightMatrix::purge()
{
    if(previousOutputs != NULL)
    {
        for(int x = 0; x < m_numRounds; ++x)
        {
            if(previousOutputs[x] != NULL)
            {
                delete [] previousOutputs[x];
                previousOutputs[x] = NULL;
            }
        }
    }
    delete [] previousOutputs;
    previousOutputs = NULL;

    if(idealOutputs != NULL)
    {
        for(int x = 0; x < m_numRounds; ++x)
        {
            if(idealOutputs[x] != NULL)
            {
                delete [] idealOutputs[x];
                idealOutputs[x] = NULL;
            }
        }
    }

    delete [] idealOutputs;
    idealOutputs = NULL;

}

void PastWeightMatrix::resetArrays(int numRounds, int numOutputs)
{
    //Clear out any old data for these arrays.
    purge();

    m_numRounds = numRounds;
    m_numOutputs = numOutputs;

    //Create arrays given a number of rounds.
    previousOutputs = new double*[m_numRounds];
    idealOutputs = new double *[m_numRounds];

    //Create arrays given a number of outputs.
    for(int x = 0; x < m_numRounds; ++x)
    {
        previousOutputs[x] = new double[m_numOutputs];
        idealOutputs[x] = new double[m_numOutputs];
    }
}

void PastWeightMatrix::copyArray(double **source, double **dest, int size)
{
    for(int x = 0; x < size; ++x)
    {
        dest[x] = source[x];
    }
}

void PastWeightMatrix::expandArray(double **&arr, int newSize)
{
    //Create a new, larger array.
    double **temp = new double*[newSize];

    //Copy the existing pointers into the new array.
    copyArray(arr, temp, m_numRounds);

    //Fill the empty space in the new array.
    for(int x = m_numRounds; x < newSize; ++x)
    {
        temp[x] = new double[m_numOutputs];
    }

    //Delete the original pointer.
    delete arr;

    arr = temp;
}

void PastWeightMatrix::increaseSize()
{
    //Expand arrays.
    expandArray(previousOutputs, m_numRounds * 2);
    expandArray(idealOutputs, m_numRounds * 2);

    m_numRounds *= 2;
}

TDNeuralNetPlayer::TDNeuralNetPlayer(Elements::PlayerType player, int numExpectedRounds)
    : NeuralNetPlayer(player), m_currentRound(0), m_oldWeights(numExpectedRounds, TD_NUM_OUTPUTS)
{
    if(m_player == Elements::PLAYER_1)
    {
        calcAsMax = true;
        m_currentRound = 0;
    }
    else
    {
        calcAsMax = false;
        m_currentRound = 1;
    }

    m_searchDepth = DLS_SEARCH_DEPTH;
}

double TDNeuralNetPlayer::depthLimitedSearch(const BoardState *currentState, int searchDepth, int &bestIndex, Elements::PlayerType player)
{
    //If an end state is found or we have reached the maximum search depth
    if(searchDepth == 0 || currentState->getNumNextStates() == 0)
    {
        //There is only one output from the network for board evaluations.
        double *results = NULL;

        getResults(currentState->getCurrentGrid(), player, results);

        bestIndex = DLS_EVALUATED_STATE;

        double retVal = results[0];

#ifdef DEBUG_MOVECHOICE
            print2(" ", retVal);
            printLine2("\t for board: ", *currentState->getCurrentGrid());
#endif

        delete [] results;
        return retVal;
    }
    else if(searchDepth > 0)
    {
        Elements::PlayerType nextPlayer;
        double bestResult, nextResult;

        //The current player only matters on a final state.
        if(player == Elements::PLAYER_1)
            nextPlayer = Elements::PLAYER_2;
        else
            nextPlayer = Elements::PLAYER_1;

        //Don't actually care about the best index from child states; only the calling function does.
        int throwaway;

        //Get the first result.
#ifdef DEBUG_MOVECHOICE
        print("Getting values for each next state: \n");
      //  print2(" ", bestResult);
#endif
        bestResult = depthLimitedSearch(currentState->getStateWithIndex(0), searchDepth - 1, throwaway, nextPlayer);

        double resultTotal = bestResult;

        //BestIndex will only matter for the initial call to DLS, but I'm not sure how to optimize it out at this point,
        //except to exclude the initial state from DLS entirely.
        bestIndex = 0;

        //Loop to find the rest of the results.
        for(int x = 1; x < currentState->getNumNextStates(); ++x)
        {
            nextResult  = depthLimitedSearch(currentState->getStateWithIndex(x), searchDepth - 1, throwaway, nextPlayer);
            resultTotal += nextResult;            

            //If calcAsMax is true, look for the highest value.  Otherwise, look for the lowest.
            if(calcAsMax)
            {
                if(nextResult > bestResult)
                {
                    bestResult = nextResult;
                    bestIndex = x;
                }
            }
            else
            {
                if(nextResult < bestResult)
                {
                    bestResult = nextResult;
                    bestIndex = x;
                }
            }
#ifdef DEBUG_MOVECHOICE
           // print2(" ", nextResult);
#endif
        }

        //Add the value of the current state to the calculation.
       // double *results = NULL;
        //getResults(currentState->getCurrentGrid(), player, (double *&)results);
        //resultTotal += results[0];
        //delete [] results;

#ifdef DEBUG_MOVECHOICE
        printLine2("\nLooking for highest valued move.  Found: ", bestResult);
#endif

        //Return the highest or lowest value.

        //return bestResult;
        //Return the summation of all child routes.
        return resultTotal;
    }
    else
    {
        return -1;
    }
}

void TDNeuralNetPlayer::makeMove(const BoardState *currentState, Grid *&nextMove)
{
    if(m_currentRound >= m_oldWeights.numRounds())
    {
        m_oldWeights.increaseSize();
    }

    //Make sure nextMove does not contain other data.
    if(nextMove != NULL)
        delete nextMove;

    nextMove = m_rulesEngine->createGameSpecificGrid();

    //Select a random number between 0 and RANDOM_MOVE_INTERVAL
    //If the result is not zero, perform a normal move action.
    //Otherwise, randomly select a move from nextMoves.
    if((rand() % RANDOM_MOVE_INTERVAL)/* true*/)
    {
        int bestIndex = 0;

#ifdef USE_DLS
        //////////////////////////////
        //Using Depth-limited search//
        //////////////////////////////

        //NOTE
        //Because of the way getResults works with CurrentPlayer, currentPlayer must be inverted
        //before depthLimitedSearch is called.
        Elements::PlayerType nextPlayer;
        if(m_player == Elements::PLAYER_1)
            nextPlayer = Elements::PLAYER_2;
        else
            nextPlayer = Elements::PLAYER_1;

        //DLS returns the value of the best state it found, but we don't really care about that.
        depthLimitedSearch(currentState, m_searchDepth, bestIndex, nextPlayer);

#ifdef DEBUG_MOVECHOICE
        printLine2("Looking for highest valued move.  Found best index: ", bestIndex);
#endif

#else
        /////////////////////////////////////////
        ////Use minimax to determine best state//
        /////////////////////////////////////////
        double *results = NULL;
        //Create an array to house results from separate board states.
        double *allResults = new double[currentState->getNumNextStates()];
        //Create an array for the outputs.  There is only one.

#ifdef DEBUG_MOVECHOICE
            print("Getting values for each next state: \n");
#endif
        //Collect state worth from all next states.
        for(int x = 0; x < currentState->getNumNextStates(); ++x)
        {
            getResults(currentState->getStateWithIndex(x)->getCurrentGrid(), m_player, results);
            allResults[x] = results[0];

#ifdef DEBUG_MOVECHOICE
            print2(" ", allResults[x]);
            printLine2("\tfor board: ", *currentState->getStateWithIndex(x)->getCurrentGrid());
#endif
        }

#ifdef DEBUG_MOVECHOICE
        print("\n");
#endif

        //Player 1 calculates a highest value (Best move for p1, worst for p2)
        //Player 2 calculates a lowest value (Worst move for p1, best for p2)
        if(calcAsMax)
        {
            //Find the state that had the highest perceived reward.
            for(int x = 1; x < currentState->getNumNextStates(); ++x)
            {
                if(allResults[x] > allResults[bestIndex])
                    bestIndex = x;
            }
#ifdef DEBUG_MOVECHOICE
            printLine2("Looking for highest valued move.  Found: ", allResults[bestIndex]);
            printLine2("Looking for highest valued move.  Found best index: ", bestIndex);
#endif
        }
        else
        {
            //Find the state that had the lowest perceived reward.
            for(int x = 1; x < currentState->getNumNextStates(); ++x)
            {
                if(allResults[x] < allResults[bestIndex])
                    bestIndex = x;
            }
#ifdef DEBUG_MOVECHOICE
            printLine2("Looking for lowest valued move.  Found: ", allResults[bestIndex]);
#endif
        }

        //TODO: Show neural network the previous move so that the expected reward value can be stored as well.

        //Store the values from the nerual net's calculation.
        //There can only be one output for TD-learning.  So, only use
        //m_oldWeights.previousOutputs[m_currentRound][0] = allResults[bestIndex];

        delete [] results;
        delete [] allResults;

//End of #ifdef USE_DLS; #else
#endif
        //Return the grid of the best state.
        *nextMove = *(currentState->getStateWithIndex(bestIndex)->getCurrentGrid());
    }
    else
    {
        //Randomly select a move.
        *nextMove = *(currentState->getStateWithIndex(rand() % currentState->getNumNextStates())->getCurrentGrid());
        //Evaluate that move and store the result.
        //getResults(nextMove, m_player, m_oldWeights.previousOutputs[m_currentRound]);
    }

    //Skip the counter to cover opponent's turn.
    m_currentRound += 2;
}

double TDNeuralNetPlayer::endStateReached(BoardState *currentState, Elements::GameState finalState, bool youMovedLast, int numRounds)
{
    //Go through this process only if the player is set to learn.
    if(!train)
        return 0.0;

    double finalStateValue;
    if(finalState == Elements::DRAW)
        finalStateValue = TD_DRAW;
    else if(finalState == Elements::P1WIN)//(Elements::GameState)m_player)
        finalStateValue = TD_WIN;
    else
        finalStateValue = TD_LOSS;

#ifdef DEBUG_TDNEURALNET
    printLine4("Player: ", m_player, "\tWinner: ", finalState);
    printLine4("EndStateReached called for player ", m_player, " with final state value ", finalStateValue);
    printLine4("Moved last? ", youMovedLast, "\nAlso, the current round is ", numRounds);
#endif

    //Since currentRound moves ahead two to accomodate the opponent's turn,
    //It must be backtracked so that it points to the correct piece of data.
    //Allow the calling function to define how many rounds the game lasted.
    //This enables self-training.
    if(numRounds > 0)
    {
        m_currentRound = numRounds;
    }
    else
    {
        if(youMovedLast)
        {
             m_currentRound -= 1;
        }
        else
        {
            //m_currentRound += 1;
        }
    }
    //Recursively gather ideal state values.

    //Reward is mentioned in every single algorithm, but never explained.
    double reward = 0.0;

    //Store all responses from neural network that it did not calculate during
    //the forward phase of the game (opponent moves).
    //For now, just recalculate all.  This can be optimized later.
    //PastWeightMatrix m_oldWeights(numRounds + 1, TD_NUM_OUTPUTS);
    BoardState *testState = currentState;

    int z = m_currentRound;
    getResults(testState, m_oldWeights.previousOutputs[z]);
    --z;
    //May be able to eliminate this step by passing storing each state the player makes during a game
    //and showing it the opponent's states as well.

    //Iterate through each board state. Store the results in the outputs array.
    while(testState->getParent() != NULL)
    {
        testState = testState->getParent();
        getResults(testState, m_oldWeights.previousOutputs[z]);

/*#ifdef DEBUG_TDNEURALNET
        printLine(*(testState->getCurrentGrid()));
        printLine(m_oldWeights.previousOutputs[z][0]);
#endif*/

        --z;
    }

    //Get "expected" values for each state.
    m_oldWeights.idealOutputs[m_currentRound][0] = finalStateValue;

    for(int x = m_currentRound - 1; x >= 0; --x)
    {
        m_oldWeights.idealOutputs[x][0] = LAMBDA * m_oldWeights.idealOutputs[x + 1][0] + TD_LEARN_RATE * ((1 - LAMBDA) * (reward + GAMMA * m_oldWeights.previousOutputs[x + 1][0] - m_oldWeights.previousOutputs[x][0]));
    }
    //Accumulate changes to neuron weights over the course of the game.
    //Must re-run getResults to properly prepare the network for training.
    testState = currentState;

    z = m_currentRound;
    getResults(testState, m_oldWeights.previousOutputs[z]);

    --z;
    //Iterate through each board state. Store the results in the outputs array.
    while(testState->getParent() != NULL)
    {
        testState = testState->getParent();
        getResults(testState, m_oldWeights.previousOutputs[z]);
        //Backpropagate for each decision throughout the game.
        m_neuralNetwork->backpropagate(m_oldWeights.previousOutputs[z], m_oldWeights.idealOutputs[z]);
        --z;
    }

    //After the entire game has been analyzed, apply the blames.
    m_neuralNetwork->applyWeightChanges();

#ifdef DEBUG_TDNEURALNET
    printLine("Actual outputs vs Ideal values");
    for(int x = 0; x < m_oldWeights.numRounds(); ++x)
    {
        printLine3(m_oldWeights.previousOutputs[x][0], "\t",
                   m_oldWeights.idealOutputs[x][0]);
    }
    printLine("");
#endif

    //Calculate root mean square.
    double delta;
    double rms = 0.0;

    for(int x = 0; x < m_currentRound; ++x)
    {
        //TD_WIN is the absolute value of TD_LOSS.  By adding TD_WIN to both the ideal and actual
        //outputs, we get correct positive numbers.  For example, if TD_WIN is 1, then the range of
        //results is from -1 to 1.  If the number -0.5 is found, 1 will be added to it, making it positive.
       // m_oldWeights.idealOutputs[x][0] = (m_oldWeights.idealOutputs[x][0] + 1) / 2.0;
       // m_oldWeights.previousOutputs[x][0] = (m_oldWeights.previousOutputs[x][0] + 1) / 2.0;
        delta = m_oldWeights.idealOutputs[x][0] - m_oldWeights.previousOutputs[x][0];// + TD_WIN + TD_WIN;
        //The delta must be within the range 0 - 1.  Therefore, we need to squash the result.
        //Basically, this removes the negative range that is used for losses by the neural network.
        //delta = (delta / (TD_WIN * 2));
        rms += delta * delta;
    }

    //Get the root of the average error for this game.
    rms = sqrt(rms / m_currentRound);

    if(m_player == Elements::PLAYER_1)
        m_currentRound = 0;
    else
        m_currentRound = 1;
    //reset();

    return rms;
}

void TDNeuralNetPlayer::reset()
{
    if(m_player == Elements::PLAYER_1)
        m_currentRound = 0;
    else
        m_currentRound = 1;

    m_oldWeights.resetArrays(TD_EXPECTED_ROUNDS, TD_NUM_OUTPUTS);
}
