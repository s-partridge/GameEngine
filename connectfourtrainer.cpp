#include "connectfourtrainer.h"

AITrainingStats ConnectFourTrainer::trainNetwork(NeuralNetPlayer *player) const
{
    return trainVersusTerriblePlayer(player);
    //return trainVersusSelf(player);
}

AITrainingStats ConnectFourTrainer::trainVersusTerriblePlayer(NeuralNetPlayer *player) const
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
                current->genNextStates(1, m_rulesEngine);

                //Randomly select a move.  Terrible play, maybe.  But it will expose the neural network to
                //a wider variety of moves that a "skilled" player.
                current = current->getStateWithIndex(0);
                //Attempt to block every move the neural net makes.

                //TODO: MoveBlocker doesn't exist yet.
                //moveBlocker(current, Elements::PLAYER_2, currentPlayer);

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
#ifdef DEBUG_TRAINER
        if(x % 500 == 499)
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


AITrainingStats ConnectFourTrainer::trainVersusSelf(NeuralNetPlayer *player) const
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
        if(x % 500 == 499)
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

AITrainingStats ConnectFourTrainer::trainTwoNetworks(NeuralNetPlayer *player1, NeuralNetPlayer *player2) const
{
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

        current = root;
#ifdef DEBUG_TDNEURALNET
        printLine2("Game #", x);
#endif
        while(true/*keep going until the loop breaks internally*/)
        {
            ++numRounds;

            //Generate the grids for the next move.

            current->genNextStates(1, m_rulesEngine);

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
    }
    delete root;
    delete userOutput;

#ifdef DEBUG_TRAINER
    print2(m_numTrainingIterations, " games completed.\t");
    print4("Neural network 1 won ", totalStats1.wins, " games, tied ", totalStats1.draws);
    printLine2(" and lost ", totalStats1.losses);
#endif

    return totalStats = trainingStats1;

}
