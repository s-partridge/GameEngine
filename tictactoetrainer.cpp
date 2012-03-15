#include "tictactoetrainer.h"


AITrainingStats TicTacToeTrainer::trainNetwork(NeuralNetPlayer *player) const
{
    //return trainOnBestTrackPlus(player);
    //return trainOnBestStatesOnly(player);
    return trainVersusTerriblePlayer(player);
    //return trainVersusSelf(player);
}

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

void TicTacToeTrainer::moveBlocker(BoardState *&currentState, Elements:: PlayerType friendly, Elements::PlayerType opponent) const
{
    /*    //Copy the current grid across.
    Grid* nextGrid = m_rulesEngine->createGameSpecificGrid();
    *nextGrid = *(currentState->getCurrentGrid());
    bool foundMatch = false;

    //Check for two-in-a-row.
    //Check vertical columns
    for(int x = 0; x < 3; ++x)
    {
        if(compareSet(nextGrid->squares[x][0], nextGrid->squares[x][1], nextGrid->squares[x][2], (Elements::GenericPieceType)opponent))
        {
            //Find the empty spot, if there is one; the third space could be friendly already.
            for(int y = 0; y < 3; ++y)
            {
                //Simply replace the first open two-in-a-row discovered.
                if(nextGrid->squares[x][y] == Elements::EMPTY)
                {
                    nextGrid->squares[x][y] = (Elements::GenericPieceType)friendly;
                    foundMatch = true;
                }
            }
        }
    }

    if(foundMatch)
    {
        currentState = currentState->getState(nextGrid);
    }
    else
    {
        currentState = currentState->getStateWithIndex(rand() % currentState->getNumNextStates());
    }

    delete nextGrid;*/

    //Copy the current grid across.
    Grid* nextGrid = m_rulesEngine->createGameSpecificGrid();
    *nextGrid = *(currentState->getCurrentGrid());
    bool foundMatch = false;

    //Check for two-in-a-row.
    //Check vertical columns
    for(int x = 0; x < 3; ++x)
    {
        if(compareSet(nextGrid->squares[x][0], nextGrid->squares[x][1], nextGrid->squares[x][2], (Elements::GenericPieceType)opponent))
        {
            //Find the empty spot, if there is one; the third space could be friendly already.
            for(int y = 0; y < 3; ++y)
            {
                //Simply replace the first open two-in-a-row discovered.
                if(nextGrid->squares[x][y] == Elements::EMPTY)
                {
                    nextGrid->squares[x][y] = (Elements::GenericPieceType)friendly;
                    foundMatch = true;
                }
            }
        }
    }

    if(!foundMatch)
    {//Check horizontal rows
        for(int y = 0; y < 3; ++y)
        {
            if(compareSet(nextGrid->squares[0][y], nextGrid->squares[1][y], nextGrid->squares[2][y], (Elements::GenericPieceType)opponent))
            {
                //Find the empty spot, if there is one; the third space could be friendly already.
                for(int x = 0; x < 3; ++x)
                {
                    //Simply replace the first open two-in-a-row discovered.
                    if(nextGrid->squares[x][y] == Elements::EMPTY)
                    {
                        nextGrid->squares[x][y] = (Elements::GenericPieceType)friendly;
                        foundMatch = true;
                    }
                }
            }
        }
    }

    //Check diagonals
    if(!foundMatch)
    {
        if(compareSet(nextGrid->squares[0][0], nextGrid->squares[1][1], nextGrid->squares[2][2], (Elements::GenericPieceType)opponent))
        {
            //Find the empty spot, if there is one; the third space could be friendly already.
            for(int x = 0; x < 3; ++x)
            {
                //Simply replace the first open two-in-a-row discovered.
                if(nextGrid->squares[x][x] == Elements::EMPTY)
                {
                    nextGrid->squares[x][x] = (Elements::GenericPieceType)friendly;
                    foundMatch = true;
                }
            }
        }
    }

    if(!foundMatch)
    {
        if(compareSet(nextGrid->squares[0][2], nextGrid->squares[1][1], nextGrid->squares[2][0], (Elements::GenericPieceType)opponent))
        {
            //Find the empty spot, if there is one; the third space could be friendly already.
            for(int x = 0, y = 2; x < 3; ++x, --y)
            {
                //Simply replace the first open two-in-a-row discovered.
                if(nextGrid->squares[x][y] == Elements::EMPTY)
                {
                    nextGrid->squares[x][y] = (Elements::GenericPieceType)friendly;
                    foundMatch = true;
                }
            }
        }
    }

    //Only do this if no sets were found.
    if(!foundMatch)
    {
        currentState = currentState->getStateWithIndex(rand() % currentState->getNumNextStates());

#ifdef DEBUG_TDNEURALNET
        printLine("Moved randomly");
#endif
    }
    //Otherwise, move to the blocked state.
    else
    {
        currentState = currentState->getState(nextGrid);

#ifdef DEBUG_TDNEURALNET
        printLine("Found match");
#endif
    }

    delete nextGrid;
}

AITrainingStats TicTacToeTrainer::trainVersusSelf(NeuralNetPlayer *player) const
{
    AITrainingStats trainingStats, totalStats;
    trainingStats.init();

    Elements::PlayerType currentPlayer;

    //Train on move tree.
    Grid *userOutput = m_rulesEngine->createGameSpecificGrid();

    Grid *startingGrid = m_rulesEngine->createGameSpecificGrid();

    BoardState *root = new BoardState(startingGrid, NULL, Elements::PLAYER_1, m_rulesEngine);
    BoardState *current;

    int numRounds;

    for(int x = 0; x < m_numTrainingIterations; ++x)
    {
        currentPlayer = Elements::PLAYER_1;

        numRounds = 0;

        current = root;
#ifdef DEBUG_TDNEURALNET
        printLine2("Game #", x);
#endif
        while(true/*keep going until the loop breaks internally*/)
        {
            ++numRounds;
            //Generate the grids for the next move.

            current->genNextStates(1, m_rulesEngine);

            //Switch between looking for best move for p1 and best move for p2.
            if(numRounds % 2)
                player->setCalcAsMax(false);
            else
                player->setCalcAsMax(true);

            //Choose a move.
            player->makeMove(current, userOutput);

            //previous = current;

            //Move down the tree.
            current = current->getState(userOutput);

            Elements::GameState endState = m_rulesEngine->testBoard(current->getCurrentGrid());

            if(endState != Elements::NORMAL)
            {
                //MovedLast doesn't actually matter if the number of moves is passed in.
                player->endStateReached(current, endState, false, numRounds);

                if(endState == Elements::P1WIN)
                    ++trainingStats.wins;
                else if(endState == Elements::DRAW)
                    ++trainingStats.draws;
                else
                    ++trainingStats.losses;
                break;
            }
        }
#ifdef DEBUG_TRAINER
        if(x % 1000 == 999)
        {
            print2(x + 1, " games completed.\t");
            print4("Neural network won ", trainingStats.wins, " games, tied ", trainingStats.draws);
            printLine2(" and lost ", trainingStats.losses);

            totalStats += trainingStats;
            //Reset stats after training iteration.
            trainingStats.init();
        }
#endif

        //player->reset();
    }

    delete root;

    return totalStats;

}
AITrainingStats TicTacToeTrainer::trainVersusTerriblePlayer(NeuralNetPlayer *player) const
{
    AITrainingStats trainingStats, totalStats;
    trainingStats.init();

    Elements::PlayerType currentPlayer;

    srand(time(NULL));

    //Train on move tree.
    Grid *userOutput = m_rulesEngine->createGameSpecificGrid();

    Grid *startingGrid = m_rulesEngine->createGameSpecificGrid();

    BoardState *root = new BoardState(startingGrid, NULL, Elements::PLAYER_1, m_rulesEngine);
    BoardState *current;

    for(int x = 0; x < m_numTrainingIterations; ++x)
    {
        currentPlayer = Elements::PLAYER_1;

        current = root;
#ifdef DEBUG_TDNEURALNET
        printLine2("Game #", x);
#endif
        while(true/*keep going until the loop breaks internally*/)
        {
            //Generate the grids for the next move.
            current->genNextStates(1, m_rulesEngine);

            //Choose a move.
            player->makeMove(current, userOutput);

            //previous = current;

            //Move down the tree.
            current = current->getState(userOutput);

            if(m_rulesEngine->testBoard(current->getCurrentGrid()) == Elements::NORMAL)
            {
                //Train the network by always choosing the first move in the list.
                //Not very good for learning, but it should give a good test.
                current->genNextStates(1, m_rulesEngine);

                //Randomly select a move.  Terrible play, maybe.  But it will expose the neural network to
                //a wider variety of moves that a "skilled" player.
                //current = current->getStateWithIndex(rand() % current->getNumNextStates());
                //Attempt to block every move the neural net makes.
                //previous = current;

                moveBlocker(current, Elements::PLAYER_2, currentPlayer);

                //See if the computer made the last move.
                Elements::GameState endState = m_rulesEngine->testBoard(current->getCurrentGrid());
                if(endState != Elements::NORMAL)
                {
                    player->endStateReached(current, endState, false);

                    if(endState == Elements::P1WIN)
                        ++trainingStats.wins;
                    else if(endState == Elements::DRAW)
                        ++trainingStats.draws;
                    else
                        ++trainingStats.losses;
                    break;
                }
            }
            else
            {
                Elements::GameState endState = m_rulesEngine->testBoard(current->getCurrentGrid());
                player->endStateReached(current, endState, true);

                if(endState == Elements::P1WIN)
                    ++trainingStats.wins;
                else if(endState = Elements::DRAW)
                    ++trainingStats.draws;
                else
                    ++trainingStats.losses;

                break;
            }
        }

        player->reset();

#ifdef DEBUG_TRAINER
        if(x % 1000 == 999)
        {
            print2(x + 1, " games completed.\t");
            print4("Neural network won ", trainingStats.wins, " games, tied ", trainingStats.draws);
            printLine2(" and lost ", trainingStats.losses);

            totalStats += trainingStats;

            //Reset stats after training iteration.
            trainingStats.init();
        }
#endif
    }

    delete root;

    return totalStats;

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
            if(currentState->getStateWithIndex(x)->getMoveWorth() > currentState->getStateWithIndex(indices[1])->getMoveWorth()
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
        nextState = currentState->getStateWithIndex(indices[choiceIndex]);
    }
    else
    {
        nextState = currentState->getStateWithIndex(indices[0]);
    }

    delete [] indices;

    return nextState;
}
