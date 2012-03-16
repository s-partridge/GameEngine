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
        //Create an array for the outputs.  There is only one.
        double *results = NULL;
        //Create an array to house results from separate board states.
        double *allResults = new double[currentState->getNumNextStates()];

        //Collect state worth from all next states.
        for(int x = 0; x < currentState->getNumNextStates(); ++x)
        {
            getResults(currentState->getStateWithIndex(x)->getCurrentGrid(), m_player, results);
            allResults[x] = results[0];
        }

        int bestIndex = 0;

        //Use minimax to determine best state.
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
        }
        else
        {
            //Find the state that had the lowest perceived reward.
            for(int x = 1; x < currentState->getNumNextStates(); ++x)
            {
                if(allResults[x] < allResults[bestIndex])
                    bestIndex = x;
            }
        }


        //TODO: Show neural network the previous move so that the expected reward value can be stored as well.

        //Store the values from the nerual net's calculation.
        //There can only be one output for TD-learning.  So, only use
        //m_oldWeights.previousOutputs[m_currentRound][0] = allResults[bestIndex];

        //Return the grid of the best state.
        *nextMove = *(currentState->getStateWithIndex(bestIndex)->getCurrentGrid());
    }
    else
    {
        //Randomly select a move.
        *nextMove = *(currentState->getStateWithIndex(rand() % currentState->getNumNextStates())->getCurrentGrid());
        //Evaluate that move and store the result.
        getResults(nextMove, m_player, m_oldWeights.previousOutputs[m_currentRound]);
    }

    //Skip the counter to cover opponent's turn.
    m_currentRound += 2;
}

void TDNeuralNetPlayer::endStateReached(BoardState *currentState, Elements::GameState finalState, bool youMovedLast, int numRounds)
{
    double finalStateValue;
    if(finalState == Elements::DRAW)
        finalStateValue = 0.0;
    else if(finalState == (Elements::GameState)m_player)
        finalStateValue = 1.0;
    else
        finalStateValue = -1.0;

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
#ifdef DEBUG_TDNEURALNET
        printLine(*(testState->getCurrentGrid()));
        printLine(m_oldWeights.previousOutputs[z][0]);
#endif

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
    reset();
}

void TDNeuralNetPlayer::reset()
{
    if(m_player == Elements::PLAYER_1)
        m_currentRound = 0;
    else
        m_currentRound = 1;

    m_oldWeights.resetArrays(TD_EXPECTED_ROUNDS, TD_NUM_OUTPUTS);
}
