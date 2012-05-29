#include "connectfourtrainer.h"

//#define DBTRAIN

AITrainingStats ConnectFourTrainer::trainNetwork(NeuralNetPlayer *player, GameDatabase *database)
{
    database->setPrintLogs(false);
#ifndef DBTRAIN
#ifdef DEBUG_TRAINER
    printLine("Training against self");
#endif
    //trainVersusSelf(player, database);
#ifdef DEBUG_TRAINER
    printLine("Training against terrible player");
#endif
    //trainVersusTerriblePlayer(player, database);
#ifdef DEBUG_TRAINER
    printLine("Training against blocking player");
#endif
    //trainVersusMoveBlocker(player, database);
#ifdef DEBUG_TRAINER
    printLine("Training against multiple players");
#endif
    //Train against everything.
    m_numTrainingIterations /= 10;
    trainVersusFunction(player, database, moveVertical);
    trainVersusFunction(player, database, moveVerticalRight);
    trainVersusFunction(player, database, moveBlocker);
    m_numTrainingIterations *= 10;
    trainVersusFunction(player, database, moveToWin);
    //trainVersusMultiple(player, database);
#else
    printGameStrings = true;
    for(int x = 0; x < 5; ++x)
    {
        cout << "train from database. " << 5 - x << " more iteration(s)." << endl;
        trainFromDatabase(player, database);
    }
    printGameStrings = false;
#endif
    database->setPrintLogs(true);

}

bool updateBoardState(BoardState *&current, const Grid *next)
{
    current = current->getState(next);

    if(current == NULL)
    {
        cerr << "Invalid state transition" << endl;
        return false;
    }
    return true;
}

AITrainingStats ConnectFourTrainer::trainFromDatabaseWithFile(NeuralNetPlayer *player, GameDatabase *database, string filename) const
{
    AITrainingStats stats;
    BoardState *root = NULL;
    BoardState *current = NULL;
    //Load database for training.
    database->setDBFile(filename);
    //Start with the data file already loaded.
    int size = database->size();
    int gameLength;

    cout << "\n\t\t" << size << " games in database\n";

    for(int x = 0; x < size; ++x)
    {
        //Print out status a set number of times.
        //Print out number
        if(x == size >> 2 || x == size >> 1 || x == ((size >> 2) + (size >> 1)))
            cout << "\r\t\t" << size - x << " games remaining...";
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
    cout << endl;

    return stats;
}

AITrainingStats ConnectFourTrainer::trainFromDatabase(NeuralNetPlayer *player, GameDatabase *database)
{
    cout << "Train from databases" << endl;

    AITrainingStats stats;

    int numIterations = m_numTrainingIterations;

    //Evaluate the neural network before and after training.
    m_numTrainingIterations = 21;

    //Turn off training for the player.
    player->setTrain(false);

    //Test the network 10 times on each type of opponent.
    //stats += trainVersusSelf(player, database);   //No point in testing against itself.
    //stats += trainVersusTerriblePlayer(player, database);
    //stats += trainVersusMoveBlocker(player, database);
    stats += trainVersusMultiple(player, database);

    //Print results.
    cout << "\t" << stats.wins + stats.losses + stats.draws << " games played. ";
    cout << stats.wins << " wins,\t" << stats.draws << " draws,\t" << stats.losses << " losses" << endl;

    //Enable training again.
    player->setTrain(true);

    cout << "\tTraining network." << endl;

    for(int x = 0; x < 10; ++x)
    {
        trainFromDatabaseWithFile(player, database, FILENAME_USER_GAMES);
        trainFromDatabaseWithFile(player, database, FILENAME_LEFTSIDE);
        trainFromDatabaseWithFile(player, database, FILENAME_MOVEBLOCKER);
        trainFromDatabaseWithFile(player, database, FILENAME_MULTIPLE);

        cout << "\t\t" << 10 - x - 1 << " more iterations" << endl;
    }

    trainFromDatabaseWithFile(player, database, FILENAME_VERSUS);

    //Turn off training for the player.
    player->setTrain(false);

    stats.init();

    //Test the network 10 times on each type of opponent.
    //stats += trainVersusSelf(player, database);   //No point in testing against itself.
    //stats += trainVersusTerriblePlayer(player, database);
    //stats += trainVersusMoveBlocker(player, database);
    stats += trainVersusMultiple(player, database);

    //Print results.
    cout << "\t" << stats.wins + stats.losses + stats.draws << " games played. ";
    cout << stats.wins << " wins,\t" << stats.draws << " draws,\t" << stats.losses << " losses" << endl;

    //Enable training again.
    player->setTrain(true);

    //Return the number of training iterations to normal.
    m_numTrainingIterations = numIterations;

    return stats;
}

BoardState *ConnectFourTrainer::moveVerticalRight(BoardState *&currentState, Elements::PlayerType friendly, Elements::PlayerType opponent)
{
    //Move in the rightmost column.
    currentState = currentState->getStateWithIndex(currentState->getNumNextStates() - 1);
    return currentState;
}

BoardState *ConnectFourTrainer::moveVertical(BoardState *&currentState, Elements::PlayerType friendly, Elements::PlayerType opponent)
{
    currentState = currentState->getStateWithIndex(0);
    return currentState;
}

BoardState *ConnectFourTrainer::moveBlocker(BoardState *&currentState, Elements::PlayerType friendly, Elements::PlayerType opponent)
{
    static ConnectFourRulesEngine re;
    Grid *currentGrid = re.createGameSpecificGrid();
    *currentGrid = *(currentState->getCurrentGrid());

    //Check for vertical three-in-a-row
    for(int x = 0; x < C4_WIDTH; ++x)
    {
        //Skip this column if it's full.
        if(currentGrid->squares[x][0] != Elements::EMPTY)
            continue;

        for(int y = 0; y < C4_HEIGHT - 3; ++y)
        {
            //Found three-out-of-four.
            if(compareC4Set(currentGrid->squares[x][y], currentGrid->squares[x][y + 1], currentGrid->squares[x][y + 2], currentGrid->squares[x][y + 3], (Elements::GenericPieceType)opponent))
            {
                if(currentGrid->squares[x][y] == Elements::EMPTY)
                {
                    currentGrid->squares[x][y] = (Elements::GenericPieceType)friendly;
                    updateBoardState(currentState, currentGrid);

                    return currentState;
                }
            }
        }
    }

    //Check for horizontal three-in-a-row
    //Check for open three-in-a-row.
    for(int x = 0; x < C4_WIDTH - 3; ++x)
    {
        int rowValue = -1, columnValue = -1;
        //Find a potential three-in-a-row set.
        for(int y = C4_HEIGHT - 1; y >= 0; --y)
        {
            for(int z = x; z < x + 4; ++z)
            {
                //Look for the first empty square in a row.
                if(currentGrid->squares[z][y] == Elements::EMPTY)
                {
                    //The first empty row.
                    rowValue = y;

                    //The empty square in that row.
                    columnValue = z;

                    //See detailed explanation of reasons for GOTO in notebook
                    goto LABEL_C4_COMPARE_HORIZONTAL_THREE_IN_A_ROW;
                }
            }
        }
        LABEL_C4_COMPARE_HORIZONTAL_THREE_IN_A_ROW:

        //Choose a win before blocking three-in-a-row.
        /*
        if(rowValue != -1 && columnValue != -1 && compareC4Set(currentGrid->squares[x][rowValue], currentGrid->squares[x + 1][rowValue], currentGrid->squares[x + 2][rowValue], currentGrid->squares[x + 3][rowValue], (Elements::GenericPieceType)friendly))
        {
            currentGrid->squares[columnValue][rowValue] = (Elements::GenericPieceType)friendly;
            updateBoardState(currentState, currentGrid);
            return currentState;
        }*/

        if(rowValue != -1 && columnValue != -1 && compareC4Set(currentGrid->squares[x][rowValue], currentGrid->squares[x + 1][rowValue], currentGrid->squares[x + 2][rowValue], currentGrid->squares[x + 3][rowValue], (Elements::GenericPieceType)opponent))
        {
            currentGrid->squares[columnValue][rowValue] = (Elements::GenericPieceType)friendly;
            updateBoardState(currentState, currentGrid);
            return currentState;
        }
    }

    //Check for open two-in-a-row.
    // ???
    // Work on this if necessary.  Training may be adequate without it.

    //TODO: implement the use of this array
    //For optimization, so not all rows have to be checked.
    //Once an empty square has been found in a row, that row can be ignored.
    //If the middle row is found empty, diagonal checks can stop completely.
    //bool rowIsEmpty[C4_WIDTH] = {false};

    //Check for diagonals.
    //Start from the bottom and go up.
    for(int y = C4_HEIGHT - 4; y >= 0; --y)
    {
        //Check for left-leaning diagonals.
        //Check sets of four from left to right.
        for(int x = 0; x < C4_WIDTH - 3; ++x)
        {
            if(compareC4Set(currentGrid->squares[x][y], currentGrid->squares[x + 1][y + 1], currentGrid->squares[x + 2][y + 2], currentGrid->squares[x + 3][y + 3], (Elements::GenericPieceType)opponent))
            {
                int openSpace = -1;
                //Find the empty square.
                for(int z = 0; z < 4; ++z)
                {
                    if(currentGrid->squares[x + z][y + z] == Elements::EMPTY)
                    {
                        openSpace = z;
                        break;
                    }
                }

                //No open space found.
                if(openSpace == -1)
                {
                    break;
                }

                //Check whether it a piece can be placed there.
                //Check if the square is on the bottom row.
                if(y + openSpace >= C4_HEIGHT - 1)
                {
                    currentGrid->squares[x + openSpace][y + openSpace] = (Elements::GenericPieceType)friendly;
                    updateBoardState(currentState, currentGrid);
                    return currentState;
                }
                //Can only do this comparison if y + openSpace is higher than the bottom row.
                //Otherwise, this will be out of bounds.  The easiest way to do that is with a separate if block.
                //The body of this if clause is the same, but the openSpace value requires a separate block.
                else if(currentGrid->squares[x + openSpace][y + openSpace + 1] != Elements::EMPTY)
                {
                    currentGrid->squares[x + openSpace][y + openSpace] = (Elements::GenericPieceType)friendly;
                    updateBoardState(currentState, currentGrid);
                    return currentState;
                }
            }
        }

        //Check for right-leaning diagonals.
        //Check sets of four from left to right.
        for(int x = C4_WIDTH - 1; x >= 3; --x)
        {
            if(compareC4Set(currentGrid->squares[x][y], currentGrid->squares[x - 1][y + 1], currentGrid->squares[x - 2][y + 2], currentGrid->squares[x - 3][y + 3], (Elements::GenericPieceType)opponent))
            {
                int openSpace = -1;
                //Find the empty square.
                for(int z = 0; z < 4; ++z)
                {
                    if(currentGrid->squares[x - z][y + z] == Elements::EMPTY)
                    {
                        openSpace = z;
                        break;
                    }
                }

                //No open space found.
                if(openSpace == -1)
                {
                    break;
                }

                //Check whether it a piece can be placed there.
                //Check if the square is on the bottom row.
                if(y + openSpace >= C4_HEIGHT - 1)
                {
                    currentGrid->squares[x - openSpace][y + openSpace] = (Elements::GenericPieceType)friendly;
                    updateBoardState(currentState, currentGrid);
                    return currentState;
                }
                //Can only do this comparison if y + openSpace is higher than the bottom row.
                //Otherwise, this will be out of bounds.  The easiest way to do that is with a separate if block.
                //The body of this if clause is the same, but the openSpace value requires a separate block.
                else if(currentGrid->squares[x - openSpace][y + openSpace + 1] != Elements::EMPTY)
                {
                    currentGrid->squares[x - openSpace][y + openSpace] = (Elements::GenericPieceType)friendly;
                    updateBoardState(currentState, currentGrid);
                    return currentState;
                }
            }
        }
    }

    //If no sets of three are found, move randomly.

    currentState = currentState->getStateWithIndex(rand() % currentState->getNumNextStates());
    return currentState;
}

BoardState *ConnectFourTrainer::moveToWin(BoardState *&currentState, Elements::PlayerType friendly, Elements::PlayerType opponent)
{
    static ConnectFourRulesEngine re;
    Grid *currentGrid = re.createGameSpecificGrid();
    *currentGrid = *(currentState->getCurrentGrid());

    //The player should first try to win if it can.
    //The comparison for wins is the same as the comparison for blocks.
    //Therefore, these checks can be put through a do-while loop twice.
    //Since toCheck will need to be switched during the loop anyway,
    //I opted to use it as the condition for the do while loop.
    //By inverting it at the beginning of the loop, I can first run through for friendly squares,
    //Then run though for opponent squares.
    //This also means I do not have to keep track of another counter (I am using toCheck to end the loop).
    Elements::GenericPieceType toCheck = (Elements::GenericPieceType)opponent;
    do
    {

        if(toCheck == (Elements::GenericPieceType)opponent)
        {
            toCheck = (Elements::GenericPieceType)friendly;
        }
        else
        {
            toCheck = (Elements::GenericPieceType)opponent;
        }
        //Check for vertical three-in-a-row
        for(int x = 0; x < C4_WIDTH; ++x)
        {
            //Skip this column if it's full.
            if(currentGrid->squares[x][0] != Elements::EMPTY)
                continue;

            for(int y = 0; y < C4_HEIGHT - 3; ++y)
            {
                //Found three-out-of-four.
                if(compareC4Set(currentGrid->squares[x][y], currentGrid->squares[x][y + 1], currentGrid->squares[x][y + 2], currentGrid->squares[x][y + 3], (Elements::GenericPieceType)toCheck))
                {
                    if(currentGrid->squares[x][y] == Elements::EMPTY)
                    {
                        currentGrid->squares[x][y] = (Elements::GenericPieceType)friendly;
                        updateBoardState(currentState, currentGrid);

                        return currentState;
                    }
                }
            }
        }

        //Check for horizontal three-in-a-row
        //Check for open three-in-a-row.
        for(int x = 0; x < C4_WIDTH - 3; ++x)
        {
            int rowValue = -1, columnValue = -1;
            //Find a potential three-in-a-row set.
            for(int y = C4_HEIGHT - 1; y >= 0; --y)
            {
                for(int z = x; z < x + 4; ++z)
                {
                    //Look for the first empty square in a row.
                    if(currentGrid->squares[z][y] == Elements::EMPTY)
                    {
                        //The first empty row.
                        rowValue = y;

                        //The empty square in that row.
                        columnValue = z;

                        //See detailed explanation of reasons for GOTO in notebook
                        goto LABEL_C4_COMPARE_HORIZONTAL_THREE_IN_A_ROW;
                    }
                }
            }
LABEL_C4_COMPARE_HORIZONTAL_THREE_IN_A_ROW:

            if(rowValue != -1 && columnValue != -1 && compareC4Set(currentGrid->squares[x][rowValue], currentGrid->squares[x + 1][rowValue], currentGrid->squares[x + 2][rowValue], currentGrid->squares[x + 3][rowValue], (Elements::GenericPieceType)toCheck))
            {
                currentGrid->squares[columnValue][rowValue] = (Elements::GenericPieceType)friendly;
                updateBoardState(currentState, currentGrid);
                return currentState;
            }
        }

        //Check for open two-in-a-row.
        // ???
        // Work on this if necessary.  Training may be adequate without it.

        //TODO: implement the use of this array
        //For optimization, so not all rows have to be checked.
        //Once an empty square has been found in a row, that row can be ignored.
        //If the middle row is found empty, diagonal checks can stop completely.
        //bool rowIsEmpty[C4_WIDTH] = {false};

        //Check for diagonals.
        //Start from the bottom and go up.
        for(int y = C4_HEIGHT - 4; y >= 0; --y)
        {
            //Check for left-leaning diagonals.
            //Check sets of four from left to right.
            for(int x = 0; x < C4_WIDTH - 3; ++x)
            {
                if(compareC4Set(currentGrid->squares[x][y], currentGrid->squares[x + 1][y + 1], currentGrid->squares[x + 2][y + 2], currentGrid->squares[x + 3][y + 3], (Elements::GenericPieceType)toCheck))
                {
                    int openSpace = -1;
                    //Find the empty square.
                    for(int z = 0; z < 4; ++z)
                    {
                        if(currentGrid->squares[x + z][y + z] == Elements::EMPTY)
                        {
                            openSpace = z;
                            break;
                        }
                    }

                    //No open space found.
                    if(openSpace == -1)
                    {
                        break;
                    }

                    //Check whether it a piece can be placed there.
                    //Check if the square is on the bottom row.
                    if(y + openSpace >= C4_HEIGHT - 1)
                    {
                        currentGrid->squares[x + openSpace][y + openSpace] = (Elements::GenericPieceType)friendly;
                        updateBoardState(currentState, currentGrid);
                        return currentState;
                    }
                    //Can only do this comparison if y + openSpace is higher than the bottom row.
                    //Otherwise, this will be out of bounds.  The easiest way to do that is with a separate if block.
                    //The body of this if clause is the same, but the openSpace value requires a separate block.
                    else if(currentGrid->squares[x + openSpace][y + openSpace + 1] != Elements::EMPTY)
                    {
                        currentGrid->squares[x + openSpace][y + openSpace] = (Elements::GenericPieceType)friendly;
                        updateBoardState(currentState, currentGrid);
                        return currentState;
                    }
                }
            }

            //Check for right-leaning diagonals.
            //Check sets of four from left to right.
            for(int x = C4_WIDTH - 1; x >= 3; --x)
            {
                if(compareC4Set(currentGrid->squares[x][y], currentGrid->squares[x - 1][y + 1], currentGrid->squares[x - 2][y + 2], currentGrid->squares[x - 3][y + 3], (Elements::GenericPieceType)toCheck))
                {
                    int openSpace = -1;
                    //Find the empty square.
                    for(int z = 0; z < 4; ++z)
                    {
                        if(currentGrid->squares[x - z][y + z] == Elements::EMPTY)
                        {
                            openSpace = z;
                            break;
                        }
                    }

                    //No open space found.
                    if(openSpace == -1)
                    {
                        break;
                    }

                    //Check whether it a piece can be placed there.
                    //Check if the square is on the bottom row.
                    if(y + openSpace >= C4_HEIGHT - 1)
                    {
                        currentGrid->squares[x - openSpace][y + openSpace] = (Elements::GenericPieceType)friendly;
                        updateBoardState(currentState, currentGrid);
                        return currentState;
                    }
                    //Can only do this comparison if y + openSpace is higher than the bottom row.
                    //Otherwise, this will be out of bounds.  The easiest way to do that is with a separate if block.
                    //The body of this if clause is the same, but the openSpace value requires a separate block.
                    else if(currentGrid->squares[x - openSpace][y + openSpace + 1] != Elements::EMPTY)
                    {
                        currentGrid->squares[x - openSpace][y + openSpace] = (Elements::GenericPieceType)friendly;
                        updateBoardState(currentState, currentGrid);
                        return currentState;
                    }
                }
            }
        }
    }while(toCheck == friendly);
    //If no sets of three are found, move randomly.

    currentState = currentState->getStateWithIndex(rand() % currentState->getNumNextStates());
    return currentState;
}


AITrainingStats ConnectFourTrainer::trainVersusFunction(NeuralNetPlayer *player, GameDatabase *database, BoardState *(*trainerFunction)(BoardState *&, Elements::PlayerType, Elements::PlayerType)) const
{
    //Load the correct file into the database.
    database->setDBFile(FILENAME_MOVEBLOCKER);

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

            current->genNextStates(player->searchDepth(), m_rulesEngine);

            //root->printMemoryAddresses(0);

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

            if(m_rulesEngine->testBoard(current->getCurrentGrid()) == Elements::NORMAL)
            {
                //Train the network by always choosing the first move in the list.
                //Not very good for learning, but it should give a good test.
                current->genNextStates(player->searchDepth(), m_rulesEngine);

                //Randomly select a move.  Terrible play, maybe.  But it will expose the neural network to
                //a wider variety of moves that a "skilled" player.
                //current = current->getStateWithIndex(0);
                //Attempt to block every move the neural net makes.

                //TODO: MoveBlocker doesn't exist yet.
                trainerFunction(current, Elements::PLAYER_2, currentPlayer);

                //See if the computer made the last move.
                Elements::GameState endState = m_rulesEngine->testBoard(current->getCurrentGrid());
                if(endState != Elements::NORMAL)
                {
                    if(printGameStrings)
                    {
                        string end;
                        current->toString(end);
                        cout << end << endl;
                    }

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

                if(printGameStrings)
                {
                    string end;
                    current->toString(end);
                    cout << end << endl;
                }

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
        //Store game in the database.
        database->storeGame(current);
    }

    delete root;

    if(m_numTrainingIterations < m_printoutInterval)
        return trainingStats;
    return totalStats;
}

AITrainingStats ConnectFourTrainer::trainVersusMultiple(NeuralNetPlayer *player, GameDatabase *database) const
{
    //Load the correct file into the database.
    database->setDBFile(FILENAME_MULTIPLE);

    AITrainingStats trainingStats, totalStats;
    trainingStats.init();

    Elements::PlayerType currentPlayer;

    //Array of function pointers.
    BoardState *(*trainerFunction)(BoardState *&, Elements::PlayerType, Elements::PlayerType);

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

        if(x % 2 == 0)
            trainerFunction = moveVertical;
        else if(x % 2 == 1)
            trainerFunction = moveVerticalRight;
        else
            trainerFunction = moveBlocker;

        root->deleteNextStates();
        current = root;
#ifdef DEBUG_TDNEURALNET
        printLine2("Game #", x);
#endif
        while(true/*keep going until the loop breaks internally*/)
        {
            ++numRounds;
            //Generate the grids for the next move.

            current->genNextStates(player->searchDepth(), m_rulesEngine);

            //root->printMemoryAddresses(0);

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

            if(m_rulesEngine->testBoard(current->getCurrentGrid()) == Elements::NORMAL)
            {
                //Train the network by always choosing the first move in the list.
                //Not very good for learning, but it should give a good test.
                current->genNextStates(player->searchDepth(), m_rulesEngine);

                //Always take the leftmost possible column.
                current = trainerFunction(current, Elements::PLAYER_2, Elements::PLAYER_1);
                //Attempt to block every move the neural net makes.

                //TODO: MoveBlocker doesn't exist yet.
                //moveBlocker(current, Elements::PLAYER_2, currentPlayer);

                //See if the computer made the last move.
                Elements::GameState endState = m_rulesEngine->testBoard(current->getCurrentGrid());
                if(endState != Elements::NORMAL)
                {
                    if(printGameStrings)
                    {
                        string end;
                        current->toString(end);
                        cout << end << endl;
                    }

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

                if(printGameStrings)
                {
                    string end;
                    current->toString(end);
                    cout << end << endl;
                }

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
        if(x % m_printoutInterval == m_printoutInterval - 1)
        {
            print2(x + 1, " games completed.\t");
            print4("Neural network won ", trainingStats.wins, " games, tied ", trainingStats.draws);
            print2(" and lost ", trainingStats.losses);

            totalStats += trainingStats;

            trainingStats.rootMeanSquare /= m_printoutInterval;

            printLine2("; root mean square: ", trainingStats.rootMeanSquare);

            //Reset stats after training iteration.
            trainingStats.init();

            /*
            //Print last game result
            for(int y = 0; y < C4_HEIGHT; ++y)
            {
                for(int x = 0; x < C4_WIDTH; ++x)
                {
                    cout << current->getCurrentGrid()->squares[x][y] << ' ';
                }
                cout << endl;
            }
            cout << endl;*/
        }
#endif
        //Write the game to the database.
        database->storeGame(current);
    }

    delete root;

    if(m_numTrainingIterations < m_printoutInterval)
        return trainingStats;
    return totalStats;
}

AITrainingStats ConnectFourTrainer::trainVersusTerriblePlayer(NeuralNetPlayer *player, GameDatabase *database) const
{
    //Load the correct file into the database.
    database->setDBFile(FILENAME_LEFTSIDE);

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

            current->genNextStates(player->searchDepth(), m_rulesEngine);

            //root->printMemoryAddresses(0);

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

            if(m_rulesEngine->testBoard(current->getCurrentGrid()) == Elements::NORMAL)
            {
                //Train the network by always choosing the first move in the list.
                //Not very good for learning, but it should give a good test.
                current->genNextStates(player->searchDepth(), m_rulesEngine);

                //Always take the leftmost possible column.
                current = current->getStateWithIndex(0);

                //Rightmost column.
                //current = current->getStateWithIndex(current->getNumNextStates() - 1);

                //Attempt to block every move the neural net makes.

                //TODO: MoveBlocker doesn't exist yet.
                //moveBlocker(current, Elements::PLAYER_2, currentPlayer);

                //See if the computer made the last move.
                Elements::GameState endState = m_rulesEngine->testBoard(current->getCurrentGrid());
                if(endState != Elements::NORMAL)
                {
                    if(printGameStrings)
                    {
                        string end;
                        current->toString(end);
                        cout << end << endl;
                    }

                    player->endStateReached(current, endState, false);

                    /*
                    if(endState == Elements::P1WIN)
                        ++trainingStats.wins;
                    else if(endState == Elements::DRAW)
                        ++trainingStats.draws;
                    else
                        ++trainingStats.losses;
                        */

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

                if(printGameStrings)
                {
                    string end;
                    current->toString(end);
                    cout << end << endl;
                }

                /*
                if(endState == Elements::P1WIN)
                    ++trainingStats.wins;
                else if(endState == Elements::DRAW)
                    ++trainingStats.draws;
                else
                    ++trainingStats.losses;
                */

                if(endState == (Elements::GameState)player->getPlayer())
                    ++trainingStats.wins;
                else if(endState == Elements::DRAW)
                    ++trainingStats.draws;
                else
                    ++trainingStats.losses;

                break;
            }

        }
#ifdef DEBUG_TRAINER
        if(x % m_printoutInterval == m_printoutInterval - 1)
        {
            print2(x + 1, " games completed.\t");
            print4("Neural network won ", trainingStats.wins, " games, tied ", trainingStats.draws);
            printLine2(" and lost ", trainingStats.losses);

            totalStats += trainingStats;
            //Reset stats after training iteration.
            trainingStats.init();

            /*
            //Print last game result
            for(int y = 0; y < C4_HEIGHT; ++y)
            {
                for(int x = 0; x < C4_WIDTH; ++x)
                {
                    cout << current->getCurrentGrid()->squares[x][y] << ' ';
                }
                cout << endl;
            }
            cout << endl;*/
        }
#endif
        //Store the game in the database.
        database->storeGame(current);
    }

    delete root;

    if(m_numTrainingIterations < m_printoutInterval)
        return trainingStats;
    return totalStats;
}

AITrainingStats ConnectFourTrainer::trainVersusMoveBlocker(NeuralNetPlayer *player, GameDatabase *database) const
{
    //Load the correct file into the database.
    database->setDBFile(FILENAME_MOVEBLOCKER);

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

            current->genNextStates(player->searchDepth(), m_rulesEngine);

            //root->printMemoryAddresses(0);

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

            if(m_rulesEngine->testBoard(current->getCurrentGrid()) == Elements::NORMAL)
            {
                //Train the network by always choosing the first move in the list.
                //Not very good for learning, but it should give a good test.
                current->genNextStates(player->searchDepth(), m_rulesEngine);

                //Randomly select a move.  Terrible play, maybe.  But it will expose the neural network to
                //a wider variety of moves that a "skilled" player.
                //current = current->getStateWithIndex(0);
                //Attempt to block every move the neural net makes.

                //TODO: MoveBlocker doesn't exist yet.
                moveBlocker(current, Elements::PLAYER_2, currentPlayer);

                //See if the computer made the last move.
                Elements::GameState endState = m_rulesEngine->testBoard(current->getCurrentGrid());
                if(endState != Elements::NORMAL)
                {
                    if(printGameStrings)
                    {
                        string end;
                        current->toString(end);
                        cout << end << endl;
                    }

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

                if(printGameStrings)
                {
                    string end;
                    current->toString(end);
                    cout << end << endl;
                }

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
        //Store game in the database.
        database->storeGame(current);
    }

    delete root;

    if(m_numTrainingIterations < m_printoutInterval)
        return trainingStats;
    return totalStats;
}

AITrainingStats ConnectFourTrainer::trainVersusSelf(NeuralNetPlayer *player, GameDatabase *database) const
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

            current->genNextStates(player->searchDepth(), m_rulesEngine);

            //root->printMemoryAddresses(0);

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
                if(printGameStrings)
                {
                    string end;
                    current->toString(end);
                    cout << end << endl;
                }

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
        //Store game to database.
        database->storeGame(current);
    }

    delete root;

    if(m_numTrainingIterations < m_printoutInterval)
        return trainingStats;
    return totalStats;
}

AITrainingStats ConnectFourTrainer::trainTwoNetworks(NeuralNetPlayer *player1, NeuralNetPlayer *player2, GameDatabase *database)
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

            current->genNextStates(currentNNPlayer->searchDepth(), m_rulesEngine);

            //Choose a move.
            currentNNPlayer->makeMove(current, userOutput);

            //previous = current;

            //Move down the tree.
            current = current->getState(userOutput);

            Elements::GameState endState = m_rulesEngine->testBoard(current->getCurrentGrid());

            //endState will not be normal when the game reaches an ending state.
            if(endState != Elements::NORMAL)
            {
                if(printGameStrings)
                {
                    string end;
                    current->toString(end);
                    cout << end << endl;
                }

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
        if(x % 500 == 499)
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

        //Store the game in the database.
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
    return totalStats = totalStats1;
}
