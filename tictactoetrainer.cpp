#include "tictactoetrainer.h"

void switchCurrentPlayer(Elements::PlayerType &player)
{
    if(player == Elements::PLAYER_1)
    {
        player = Elements::PLAYER_2;
    }
    else
    {
        player = Elements::PLAYER_1;
    }
}

AITrainingStats TicTacToeTrainer::trainNetwork(NeuralNetPlayer *player) const
{
    return trainOnBestTrackPlus(player);
    //return trainOnBestStatesOnly(player);
}

AITrainingStats TicTacToeTrainer::trainOnBestTrackPlus(NeuralNetPlayer *player) const
{
    map<string, int> hashmap;
    MoveTableBuilder::generateHashTable("/Users/samuel/Documents/test.mt", hashmap);

    Elements::PlayerType currentPlayer;

    AITrainingStats trainingStats;
    trainingStats.init();

    //Seed rand()
    srand(time(NULL));

    //To store the results from the network.
    //For tic tac toe, these are expected to be a single value.  The arrays are for consistency.
    double *outputs = new double[NUM_OUTPUTS];
    double *expected = new double[NUM_OUTPUTS];

    Grid *userOutput = m_rulesEngine->createGameSpecificGrid();
    Grid *gameBoard = new TicTacToeGrid();
    Grid *previous = new TicTacToeGrid();
    Grid **next = NULL;

    int numNextStates = 0;

    for(int x = 0; x < m_numTrainingIterations; ++x)
    {
        currentPlayer = Elements::PLAYER_1;
        //Reset the game board.
        gameBoard->init();

        while(m_rulesEngine->testBoard(gameBoard) == Elements::NORMAL)
        {
            m_rulesEngine->genNextMoves(gameBoard, next, currentPlayer, numNextStates);

            //Train on the current state.  Otherwise, the blank board will never be learned.
            //This will also mean that the network imprints the best move track twice.

            for(int y = 0; y < numNextStates; ++y)
            {
                trainOnce(player, next[y], userOutput, expected, outputs, currentPlayer, hashmap);

                *previous = *next[y];

                //Update gameBoard.  Expected contains the index of the next move.
                m_rulesEngine->updateGrid(next[y], expected, currentPlayer);

                //Training statistics.
                ++trainingStats.totalMoves;
                //User otherPlayer, rather than currentPlayer.
                //CurrentPlayer now represents the opponent.
                if(m_rulesEngine->isValidMove(previous, userOutput, currentPlayer))
                {
                    //GameBoard will contain the best possible move from the previous state.
                    //If userOutput is equal to gameBoard, the AI made the best possible move.
                    if(*next[y] == *userOutput)
                        ++trainingStats.bestMoves;
                }
                else
                {
                    ++ trainingStats.errors;
                }
            }

            trainOnce(player, gameBoard, userOutput, expected, outputs, currentPlayer, hashmap);

            *previous = *gameBoard;

            //Update gameBoard.  Expected contains the index of the next move.
            m_rulesEngine->updateGrid(gameBoard, expected, currentPlayer);


            //Training statistics.
            ++trainingStats.totalMoves;
            //User otherPlayer, rather than currentPlayer.
            //CurrentPlayer now represents the opponent.
            if(m_rulesEngine->isValidMove(previous, userOutput, currentPlayer))
            {
                //GameBoard will contain the best possible move from the previous state.
                //If userOutput is equal to gameBoard, the AI made the best possible move.
                if(*gameBoard == *userOutput)
                    ++trainingStats.bestMoves;
            }
            else
            {
                ++ trainingStats.errors;
            }

            switchCurrentPlayer(currentPlayer);

            //Clear out used memory.
            for(int z = 0; z < numNextStates; ++z)
            {
                delete next[z];
            }
            delete [] next;

        }
        if(x % 1000 == 999)
        {
            printLine2("Total Moves:\t", trainingStats.totalMoves);
            printLine2("Total Best Moves:\t", trainingStats.bestMoves);
            printLine2("Total Errors:\t", trainingStats.errors);
            print("\n");

            trainingStats.init();
        }
    }

    return trainingStats;
}

void TicTacToeTrainer::trainOnce(NeuralNetPlayer *player, Grid *gameBoard, Grid *userOutput,
                                 double *&expected, double *&outputs, Elements::PlayerType currentPlayer,
                                 map<string, int> &hashmap) const
{
    //Make a move.  Store the result in outputs.
    player->getResults(gameBoard, currentPlayer, outputs);

    //Store the current board.
    *userOutput = *gameBoard;
    //Update it to match the move made.
    m_rulesEngine->updateGrid(userOutput, outputs, currentPlayer);

    //Get the next baord.
    string test = gameBoard->toString();
    expected[0] = hashmap[test];

    //Pass the expected results to the network.
    player->trainNetwork(outputs, expected);
}

AITrainingStats TicTacToeTrainer::trainOnBestStatesOnly(NeuralNetPlayer *player) const
{
    map<string, int> hashmap;
    MoveTableBuilder::generateHashTable("/Users/samuel/Documents/test.mt", hashmap);

    Elements::PlayerType currentPlayer;

    AITrainingStats trainingStats;
    trainingStats.init();

    //Seed rand()
    srand(time(NULL));

    //To store the results from the network.
    //For tic tac toe, these are expected to be a single value.  The arrays are for consistency.
    double *outputs = new double[NUM_OUTPUTS];
    double *expected = new double[NUM_OUTPUTS];

    Grid *userOutput = m_rulesEngine->createGameSpecificGrid();
    TicTacToeGrid *gameBoard = new TicTacToeGrid();
    TicTacToeGrid *previous = new TicTacToeGrid();

    userOutput->init();
    previous->init();

    for(int x = 0; x < m_numTrainingIterations; ++x)
    {
        currentPlayer = Elements::PLAYER_1;
        //Set the game board to blank.
        gameBoard->init();

        //Play through a complete game.  Stop when an ending state is reached.
        while(m_rulesEngine->testBoard(gameBoard) == Elements::NORMAL)
        {
            //Make a move.  Store the result in outputs.
            player->getResults(gameBoard, currentPlayer, outputs);

            //Store the current board.
            *userOutput = *gameBoard;
            //Update it to match the move made.
            m_rulesEngine->updateGrid(userOutput, outputs, currentPlayer);

            //Get the next baord.
            string test = gameBoard->toString();
            expected[0] = hashmap[test];

            //Store the current board in previous.
            *previous = *gameBoard;

            //Update gameBoard and currentPlayer.  Expected contains the index of the next move.
            m_rulesEngine->updateGrid(gameBoard, expected, currentPlayer);

            //Training statistics.
            ++trainingStats.totalMoves;
            //User otherPlayer, rather than currentPlayer.
            //CurrentPlayer now represents the opponent.
            if(m_rulesEngine->isValidMove(previous, userOutput, currentPlayer))
            {
                //GameBoard will contain the best possible move from the previous state.
                //If userOutput is equal to gameBoard, the AI made the best possible move.
                if(gameBoard == userOutput)
                    ++trainingStats.bestMoves;
            }
            else
            {
                ++ trainingStats.errors;
            }


            //Pass the expected results to the network.
            player->trainNetwork(outputs, expected);

            //Switch current player.
            if(currentPlayer == Elements::PLAYER_1)
            {
                currentPlayer = Elements::PLAYER_2;
            }
            else
            {
                currentPlayer = Elements::PLAYER_1;
            }
        }
        if(x % 1000 == 999)
        {
            printLine2("Total Moves:\t", trainingStats.totalMoves);
            printLine2("Total Best Moves:\t", trainingStats.bestMoves);
            printLine2("Total Errors:\t", trainingStats.errors);
            print("\n");

            trainingStats.init();
        }
#ifdef DEBUG_TRAINER
    //printLine4("Neural Net best moves: ", trainingStats.bestMoves, " worst moves: ", trainingStats.errors);
        printLine2(m_numTrainingIterations - x - 1, " iterations remaining");
#endif
    }

    return trainingStats;
}

BoardState *TicTacToeTrainer::pickNextMoveToTrain(BoardState *currentState) const
{
    //Store potential next move indices.
    int *indices = new int[NUM_CHOICES];
    BoardState *nextState;
    indices[0] = currentState->getNextBestStateIndex();

    if(currentState->getNumNextStates() > 1)
    {
        //Find the second-best state.
        indices[1] = 0;
        for(int x = 1; x < currentState->getNumNextStates(); ++x)
        {
            //If the state at x is better than indices[1], replace it.
            //But only if it is not also the next best state.
            if(currentState->getState(x)->getMoveWorth() > currentState->getState(indices[1])->getMoveWorth()
                    && x != currentState->getNextBestStateIndex())
                indices[1] = x;
        }

        int choiceIndex = rand() % NUM_CHOICES;

        //If choiceIndex is higher than the number of next states,
        //simply use the index of the best state.
        if(choiceIndex >= currentState->getNumNextStates())
        {
            choiceIndex = currentState->getNextBestStateIndex();
        }

        //Randomly pick one of the two best states.
        nextState = currentState->getState(indices[choiceIndex]);
    }
    else
    {
        nextState = currentState->getState(indices[0]);
    }

    delete [] indices;

    return nextState;
}
