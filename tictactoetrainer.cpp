#include "tictactoetrainer.h"

//#define DBTRAIN

AITrainingStats TicTacToeTrainer::trainNetwork(NeuralNetPlayer *player, GameDatabase *database)
{
    database->setPrintLogs(false);
#ifndef DBTRAIN
#ifdef DEBUG_TRAINER
    cout << "Training versus terrible player" << endl;
#endif
    //for(int x = 0; x < 4; ++x)
    m_numTrainingIterations /= 10;
    trainVersusTerriblePlayer(player, database);
    m_numTrainingIterations *= 10;
#ifdef DEBUG_TRAINER
    cout << "Training versus self" << endl;
#endif
    //trainVersusSelf(player, database);
#ifdef DEBUG_TRAINER
    //m_numTrainingIterations = 100;
    //m_printoutInterval = 1;
    cout << "Training versus blocking player" << endl;
#endif
    trainVersusMoveBlocker(player, database);
#else
    printGameStrings = true;
 //   for(int x = 0; x < 5; ++x)
 //   {
 //       cout << "train from database. " << 5 - x << " more iteration(s)." << endl;
        trainFromDatabase(player, database);
 //   }
    printGameStrings = false;

    /*database->setDBFile("small.db");
    BoardState *root = NULL;
    BoardState *current = NULL;
    int gameLength;
    for(int x = 0; x < m_numTrainingIterations; ++x)
    {
        database->loadGameFromIndex(root, current, gameLength, 0, m_rulesEngine);

        cout << player->endStateReached(current, m_rulesEngine->testBoard(current->getCurrentGrid()), true, gameLength) << endl;
    }*/
#endif
    database->setPrintLogs(true);
}

AITrainingStats TicTacToeTrainer::trainFromDatabaseWithFile(NeuralNetPlayer *player, GameDatabase *database, string filename) const
{
    AITrainingStats stats;
    BoardState *root = NULL;
    BoardState *current = NULL;
    //Load database for training.
    database->setDBFile(filename);
    //Start with the data file already loaded.
    int size = database->size();
    int gameLength;

    //cout << "\n\t\t" << size << " games in database\n";

    for(int x = 0; x < size; ++x)
    {
        //Print out status a set number of times.
        //Print out number
        //if(x == size >> 2 || x == size >> 1 || x == ((size >> 2) + (size >> 1)))
        //    cout << "\r\t\t" << size - x << " games remaining...";
        //Pick a random game from the database.
       // int index = rand() % size;
        //Pull a game from the database.
        database->loadGameFromIndex(root, current, gameLength, x, m_rulesEngine);
        //Get the end state of the game.
        Elements::GameState endState = m_rulesEngine->testBoard(current->getCurrentGrid());
        //Train the player.
        //False is used because youMovedLast only matters when the game length is not defined.
        player->endStateReached(current, endState, false, gameLength);

    }
    //cout << endl;

    return stats;
}

AITrainingStats TicTacToeTrainer::trainFromDatabase(NeuralNetPlayer *player, GameDatabase *database)
{
    cout << "Train from databases" << endl;

    AITrainingStats stats;

    int numIterations = m_numTrainingIterations;

    //Evaluate the neural network before and after training.
    m_numTrainingIterations = 10;

    //Turn off training for the player.
    player->setTrain(false);

    //Test the network 10 times on each type of opponent.
    //stats += trainVersusSelf(player, database);   //No point in testing against itself.
    stats += trainVersusTerriblePlayer(player, database);
    stats += trainVersusMoveBlocker(player, database);

    //Print results.
    cout << "\t" << stats.wins + stats.losses + stats.draws << " games played. ";
    cout << stats.wins << " wins,\t" << stats.draws << " draws,\t" << stats.losses << " losses" << endl;

    //Enable training again.
    player->setTrain(true);

    cout << "\tTraining network." << endl;

    int numGameIterations = 900;
    for(int x = 0; x < numGameIterations; ++x)
    {
        trainFromDatabaseWithFile(player, database, FILENAME_USER_GAMES);
        trainFromDatabaseWithFile(player, database, FILENAME_TERRIBLE);
        trainFromDatabaseWithFile(player, database, FILENAME_VERSUS);
        trainFromDatabaseWithFile(player, database, FILENAME_SELF);

        cout << "\t\t" << numGameIterations - x - 1 << " more iterations" << endl;
    }

    trainFromDatabaseWithFile(player, database, FILENAME_VERSUS);

    //Turn off training for the player.
    player->setTrain(false);

    stats.init();

    //Test the network 10 times on each type of opponent.
    //stats += trainVersusSelf(player, database);   //No point in testing against itself.
    stats += trainVersusTerriblePlayer(player, database);
    stats += trainVersusMoveBlocker(player, database);

    //Print results.
    cout << "\t" << stats.wins + stats.losses + stats.draws << " games played. ";
    cout << stats.wins << " wins,\t" << stats.draws << " draws,\t" << stats.losses << " losses" << endl;

    //Enable training again.
    player->setTrain(true);

    //Return the number of training iterations to normal.
    m_numTrainingIterations = numIterations;

    return stats;
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
        if(!foundMatch && compareSet(nextGrid->squares[x][0], nextGrid->squares[x][1], nextGrid->squares[x][2], (Elements::GenericPieceType)opponent))
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
            if(!foundMatch && compareSet(nextGrid->squares[0][y], nextGrid->squares[1][y], nextGrid->squares[2][y], (Elements::GenericPieceType)opponent))
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

    BoardState *tempState;
    //Only do this if no sets were found.
    if(!foundMatch)
    {
        tempState = currentState->getStateWithIndex(rand() % currentState->getNumNextStates());

#ifdef DEBUG_TDNEURALNET
        printLine("Moved randomly");
#endif
    }
    //Otherwise, move to the blocked state.
    else
    {
        tempState = currentState->getState(nextGrid);

#ifdef DEBUG_TDNEURALNET
        printLine("Found match");
#endif
    }

    if(tempState == NULL)
    {
        cerr << "WARNING: The state being transitioned to is invalid.\n";
        cerr << "Current state address: " << currentState;
        cerr << "\nCurrent state grid:\t" << *(currentState->getCurrentGrid()) << endl;
        cerr << "Attempted state grid:\t" << *(nextGrid) << endl;
        exit(1);
    }
    currentState = tempState;
    delete nextGrid;
}

AITrainingStats TicTacToeTrainer::trainVersusSelf(NeuralNetPlayer *player, GameDatabase *database) const
{
    //Load the correct file into the database.
    database->setDBFile(FILENAME_SELF);

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

        root->deleteNextStates();
        current = root;
#ifdef DEBUG_TDNEURALNET
        printLine2("Game #", x);
#endif
        while(true/*keep going until the loop breaks internally*/)
        {
            ++numRounds;
            //Generate the grids for the next move.

            current->genNextStates(DFS_TREE_DEPTH, m_rulesEngine);

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
        if((x % m_printoutInterval == m_printoutInterval - 1))
        {
            print2(x + 1, " games completed.\t");
            print4("Neural network won ", trainingStats.wins, " games, tied ", trainingStats.draws);
            printLine2(" and lost ", trainingStats.losses);

            totalStats += trainingStats;
            //Reset stats after training iteration.
            trainingStats.init();
        }
#endif
        //Store game in database
        database->storeGame(current);
    }

    delete root;

    if(m_numTrainingIterations < m_printoutInterval)
        return trainingStats;

    return totalStats;

}

AITrainingStats TicTacToeTrainer::trainTwoNetworks(NeuralNetPlayer *player1, NeuralNetPlayer *player2, GameDatabase *database)
{
    //Load the correct file into the database.
    database->setDBFile(FILENAME_VERSUS);

    AITrainingStats trainingStats1, totalStats1;
    AITrainingStats trainingStats2, totalStats2, totalStats;
    trainingStats1.init();
    trainingStats2.init();

    NeuralNetPlayer *currentNNPlayer;
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
        currentNNPlayer = player1;

        numRounds = 0;

        root->deleteNextStates();
        current = root;
#ifdef DEBUG_TDNEURALNET
        printLine2("Game #", x);
#endif
        while(true/*keep going until the loop breaks internally*/)
        {
            ++numRounds;

            //Generate the grids for the next move.

            current->genNextStates(DFS_TREE_DEPTH, m_rulesEngine);

            //Choose a move.
            currentNNPlayer->makeMove(current, userOutput);

            //previous = current;

            //Move down the tree.
            current = current->getState(userOutput);

            Elements::GameState endState = m_rulesEngine->testBoard(current->getCurrentGrid());

            //endState will not be normal when the game reaches an ending state.
            if(endState != Elements::NORMAL)
            {
                //EndStateReached is what will train the networks.  The function must be passed
                //which player made the last move.
                if(currentPlayer == Elements::PLAYER_1)
                {
                    player1->endStateReached(current, endState, true, numRounds);
                    player2->endStateReached(current, endState, false, numRounds);
                }
                else
                {
                    player1->endStateReached(current, endState, false, numRounds);
                    player2->endStateReached(current, endState, true, numRounds);
                }

                if(endState == Elements::P1WIN)
                {
                    ++trainingStats1.wins;
                    ++trainingStats2.losses;
                }
                else if(endState == Elements::DRAW)
                {
                    ++trainingStats1.draws;
                    ++trainingStats2.draws;
                }
                else
                {
                    ++trainingStats1.losses;
                    ++trainingStats2.wins;
                }
                break;
            }

            //Switch the current player.
            if(currentPlayer == Elements::PLAYER_1)
            {
                currentPlayer = Elements::PLAYER_2;
                currentNNPlayer = player2;
            }

            else
            {
                currentPlayer = Elements::PLAYER_1;
                currentNNPlayer = player1;
            }

        }
#ifdef DEBUG_TRAINER
        if(x % m_printoutInterval == m_printoutInterval - 1)
        {
            print2(x + 1, " games completed.\t");
            print4("Neural network 1 won ", trainingStats1.wins, " games, tied ", trainingStats1.draws);
            printLine2(" and lost ", trainingStats1.losses);
            totalStats1 += trainingStats1;
            totalStats2 += trainingStats2;
            //Reset stats after training iteration.
            trainingStats1.init();
            trainingStats2.init();
        }
#endif

        //Store game in database
        database->storeGame(current);
    }
    delete root;
    delete userOutput;

#ifdef DEBUG_TRAINER
    print2(m_numTrainingIterations, " games completed.\t");
    print4("Neural network 1 won ", totalStats1.wins, " games, tied ", totalStats1.draws);
    printLine2(" and lost ", totalStats1.losses);
#endif

    if(m_numTrainingIterations < m_printoutInterval)
        return trainingStats1;

    return totalStats = trainingStats1;

}

AITrainingStats TicTacToeTrainer::trainVersusTerriblePlayer(NeuralNetPlayer *player, GameDatabase *database) const
{
    //Load the correct file into the database.
    database->setDBFile(FILENAME_TERRIBLE);

    AITrainingStats trainingStats, totalStats;
    trainingStats.init();

    Elements::PlayerType currentPlayer;

    srand(time(NULL));

    //If player 2 is passed in here, it will be trained incorrectly unless calcAsMax is reset.
    bool calcAsMax = player->getCalcAsMax();
    player->setCalcAsMax(true);

    //Train on move tree.
    Grid *userOutput = m_rulesEngine->createGameSpecificGrid();

    Grid *startingGrid = m_rulesEngine->createGameSpecificGrid();

    BoardState *root = new BoardState(startingGrid, NULL, Elements::PLAYER_1, m_rulesEngine);
    BoardState *current;

    for(int x = 0; x < m_numTrainingIterations; ++x)
    {
        currentPlayer = Elements::PLAYER_1;

        root->deleteNextStates();
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
                current->genNextStates(DFS_TREE_DEPTH, m_rulesEngine);

                //Randomly select a move.  Terrible play, maybe.  But it will expose the neural network to
                //a wider variety of moves that a "skilled" player.
                current = current->getStateWithIndex(0);//rand() % current->getNumNextStates());
                //Attempt to block every move the neural net makes.
                //previous = current;

                //moveBlocker(current, Elements::PLAYER_2, currentPlayer);

                //See if the computer made the last move.
                Elements::GameState endState = m_rulesEngine->testBoard(current->getCurrentGrid());
                if(endState != Elements::NORMAL)
                {
                    player->endStateReached(current, endState, false);

                    if(endState == (Elements::GameState)player->getPlayer())
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

                if(endState == (Elements::GameState)player->getPlayer())
                    ++trainingStats.wins;
                else if(endState == Elements::DRAW)
                    ++trainingStats.draws;
                else
                    ++trainingStats.losses;

                break;
            }
        }

        player->reset();

#ifdef DEBUG_TRAINER
        if(x % m_printoutInterval == m_printoutInterval - 1)
        {
            print2(x + 1, " games completed.\t");
            print4("Neural network won ", trainingStats.wins, " games, tied ", trainingStats.draws);
            printLine2(" and lost ", trainingStats.losses);

            totalStats += trainingStats;

            //Reset stats after training iteration.
            trainingStats.init();
        }
#endif

        //store game in database
        database->storeGame(current);
    }

    delete root;

    player->setCalcAsMax(calcAsMax);

    if(m_numTrainingIterations < m_printoutInterval)
        return trainingStats;
    return totalStats;

}

AITrainingStats TicTacToeTrainer::trainVersusMoveBlocker(NeuralNetPlayer *player, GameDatabase *database) const
{
    //Load the correct file into the database.
    database->setDBFile(FILENAME_BLOCKER);

    AITrainingStats trainingStats, totalStats;
    trainingStats.init();

    Elements::PlayerType currentPlayer;

    srand(time(NULL));

    //If player 2 is passed in here, it will be trained incorrectly unless calcAsMax is reset.
    bool calcAsMax = player->getCalcAsMax();
    player->setCalcAsMax(true);

    //Train on move tree.
    Grid *userOutput = m_rulesEngine->createGameSpecificGrid();

    Grid *startingGrid = m_rulesEngine->createGameSpecificGrid();

    BoardState *root = new BoardState(startingGrid, NULL, Elements::PLAYER_1, m_rulesEngine);
    BoardState *current;

    for(int x = 0; x < m_numTrainingIterations; ++x)
    {
        currentPlayer = Elements::PLAYER_1;

        root->deleteNextStates();
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
                current->genNextStates(DFS_TREE_DEPTH, m_rulesEngine);

                moveBlocker(current, Elements::PLAYER_2, currentPlayer);

                //See if the computer made the last move.
                Elements::GameState endState = m_rulesEngine->testBoard(current->getCurrentGrid());
                if(endState != Elements::NORMAL)
                {
                    trainingStats.rootMeanSquare += player->endStateReached(current, endState, false);

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
                trainingStats.rootMeanSquare += player->endStateReached(current, endState, true);

                if(endState == Elements::P1WIN)
                    ++trainingStats.wins;
                else if(endState == Elements::DRAW)
                    ++trainingStats.draws;
                else
                    ++trainingStats.losses;

                break;
            }
        }

        player->reset();

#ifdef DEBUG_TRAINER
        if(x % m_printoutInterval == m_printoutInterval - 1)
        {
            print2(x + 1, " games completed.\t");
            print4("Neural network won ", trainingStats.wins, " games, tied ", trainingStats.draws);
            print2(" and lost ", trainingStats.losses);

            trainingStats.rootMeanSquare /= m_printoutInterval;

            printLine2("; root mean square: ", trainingStats.rootMeanSquare);
            totalStats += trainingStats;

/*            if(trainingStats.rootMeanSquare < 0.145)
            {
                cout << "Neural network is doing well enough.  Stopping training early." << endl;
                break;
            }*/

            //Reset stats after training iteration.
            trainingStats.init();
        }
#endif

        //store game in database
        database->storeGame(current);
    }

    delete root;

    player->setCalcAsMax(calcAsMax);

    if(m_numTrainingIterations < m_printoutInterval)
        return trainingStats;
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
            int *lastMoves = NULL;
            m_rulesEngine->genNextMoves(gameBoard, next, lastMoves, currentPlayer, numNextStates);

            delete [] lastMoves;

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
