#include "gamecontroller.h"

GameController::GameController()
    : m_NNPlayer1(NULL), m_NNPlayer2(NULL), m_dataController(NULL),
      m_AIBuilder(NULL), m_AITrainer(NULL), m_rulesEngine(NULL)
{
    m_database = new GameDatabase();

    m_human1 = new Human();
    m_human2 = new Human();

    m_player1 = m_human1;
    m_player2 = m_human2;

    p1IsAI = false;
    p2IsAI = false;

    isP1Turn = true;

    m_currentPlayer = m_player1;
}

//Delete everything that was created locally.
void GameController::purge()
{
    if(m_database != NULL)
    {
        delete m_database;
        m_database = NULL;
    }

    if(m_human1 != NULL)
    {
        delete m_human1;
        m_human1 = NULL;
    }

    if(m_human2 != NULL)
    {
        delete m_human2;
        m_human2 = NULL;
    }

    if(m_NNPlayer1 != NULL)
    {
        delete m_NNPlayer1;
        m_NNPlayer1 = NULL;
    }

    if(m_NNPlayer2 != NULL)
    {
        delete m_NNPlayer2;
        m_NNPlayer2 = NULL;
    }

    if(m_AIBuilder != NULL)
    {
        delete m_AIBuilder;
        m_AIBuilder = NULL;
    }

    if(m_AITrainer != NULL)
    {
        delete m_AITrainer;
        m_AITrainer = NULL;
    }
}

//Generate a neural network and store its parameters in the file specified by filename.
void GameController::createNNPlayer(Elements::PlayerType player, string filename)
{
#ifdef DEBUG_GAMECONTROLLER
    printLine4("Generating new NNP for player ", player, " using file ", filename);
#endif
    fstream io;

    io.open(filename.c_str(), ios::in | ios::out);

    NeuralNetPlayer *newPlayer;

    if(io.is_open())
    {
        cout << "File exists\n";
        io.close();
        newPlayer = m_AIBuilder->loadNeuralNet(player, m_rulesEngine, filename);
    }
    else
    {
        cout << "File does not exist.  What do I do???\n";
        newPlayer = m_AIBuilder->buildNeuralNet(player, m_rulesEngine, filename);
    }


#ifdef DEBUG_GAMECONTROLLER
    print("Finished generating NNP");
#endif

    if(player == Elements::PLAYER_1)
    {
        if(m_NNPlayer1 != NULL)
            delete m_NNPlayer1;
        m_NNPlayer1 = newPlayer;
        m_NN1Filename = filename;
    }
    else
    {
        if(m_NNPlayer2 != NULL)
            delete m_NNPlayer2;
        m_NNPlayer2 = newPlayer;
        m_NN2Filename = filename;
    }
}

void GameController::saveNNPlayer(Elements::PlayerType player, string filename)
{
    if(player == Elements::PLAYER_1)
    {
        if(filename != "")
        {
            m_NN1Filename = filename;
        }
    }
    else if(player == Elements::PLAYER_2)
    {
        if(filename != "")
        {
            m_NN2Filename = filename;
        }
    }

    saveNNPlayer(player);
}

void GameController::saveNNPlayer(Elements::PlayerType player)
{
    if(player == Elements::PLAYER_1)
    {
        m_NNPlayer1->saveNeuralNetwork(m_NN1Filename);
    }
    else if(player == Elements::PLAYER_2)
    {
        m_NNPlayer2->saveNeuralNetwork(m_NN2Filename);
    }

}

const Grid *GameController::attemptMove(const Grid *move)
{
    Grid *nextMove = NULL;

    //Only do this if the current player is an AI.
    //If human, should return NULL.

#ifdef DEBUG_GAMECONTROLLER
    print("Humanity check.");
#endif
    //if(typeid(NeuralNetPlayer) == typeid(*m_currentPlayer))
    if(p1IsAI && isP1Turn || p2IsAI && !isP1Turn)
    {
#ifdef DEBUG_GAMECONTROLLER
    print("Not a human.");
#endif
        if(isP1Turn)
        {
            m_NNPlayer1->makeMove(m_dataController->getCurrentState(), nextMove);
        }
        else
        {
            m_NNPlayer2->makeMove(m_dataController->getCurrentState(), nextMove);
        }

        m_dataController->setNextMove(nextMove);
        switchCurrentPlayer();

        return nextMove;

    }
    //else if(typeid(Human) == typeid(*m_currentPlayer))
    else if(!p1IsAI && isP1Turn || !p2IsAI && !isP1Turn)
    {
#ifdef DEBUG_GAMECONTROLLER
        print("Humanity check passed.");
        print("Comparing currentMove with nextMove.");
#endif
        //If valid, return the input as the output.
        if(m_rulesEngine->isValidMove(m_dataController->getCurrentState()->getCurrentGrid(),
                                      move, m_dataController->getCurrentState()->getCurrentPlayer()))
        {
#ifdef DEBUG_GAMECONTROLLER
            print("Move is acceptable.");
#endif
            m_dataController->setNextMove(move);
            switchCurrentPlayer();
            return move;
        }
        //If not, return a null pointer.
        else
        {
#ifdef DEBUG_GAMECONTROLLER
            print("Move is unacceptable.");
#endif
            return nextMove;
        }
    }

    return NULL;
}

void GameController::gameOver()
{
    //The current state will be needed several times.  Why keep calling a function to retrieve it?
    BoardState *temp = m_dataController->getCurrentState();

    //Store the game to the database.
    m_database->storeGame(temp);

    Elements::GameState endState =m_rulesEngine->testBoard(temp->getCurrentGrid());
    //This will perform live training of the neural network players.
    if(m_dataController->getRoundNumber() % 2)
    {
        if(p1IsAI)
            m_NNPlayer1->endStateReached(temp, endState, true, m_dataController->getRoundNumber());
        if(p2IsAI)
            m_NNPlayer2->endStateReached(temp, endState, false, m_dataController->getRoundNumber());
    }
    else
    {
        if(p1IsAI)
            m_NNPlayer1->endStateReached(temp, endState, false, m_dataController->getRoundNumber());
        if(p2IsAI)
            m_NNPlayer2->endStateReached(temp, endState, true, m_dataController->getRoundNumber());
    }
}

//Reset the move tree and current game stats.
void GameController::resetGame()
{
    if(m_dataController != NULL)
    {
        m_dataController->resetTree();
        m_dataController->resetGameStats();

        //Humans don't need to be reset.
        if(p1IsAI)
            m_NNPlayer1->reset();
        if(p2IsAI)
            m_NNPlayer2->reset();

        isP1Turn = true;
    }
}

//Undo the last move made.
void GameController::undoMove()
{
    if(m_dataController != NULL)
    {
        m_dataController->undoMove();
        switchCurrentPlayer();
    }
}

//Set either player 1 or player 2 as the active player.
void GameController::switchCurrentPlayer()
{
  /*  if(m_currentPlayer == m_player1)
    {
        m_currentPlayer = m_player2;
    }
    else
    {
        m_currentPlayer = m_player1;
    }*/
    if(isP1Turn)
        isP1Turn = false;
    else
        isP1Turn = true;
}

//Train neural net player 1, 2, or both.
void GameController::trainAI(Elements::PlayerType player = Elements::NONE)
{
    AITrainingStats results;
    if(player == Elements::PLAYER_1)
    {
        if(m_NNPlayer1 != NULL)
        {
            results = m_AITrainer->trainNetwork(m_NNPlayer1, m_database);
            m_dataController->addToTrainingStats(results, Elements::PLAYER_1);

            //Reset the database to write to the user games file.
            m_database->setDBFile(FILENAME_USER_GAMES);
        }
    }

    else if(player == Elements::PLAYER_2)
    {
        if(m_NNPlayer2 != NULL)
        {
            results = m_AITrainer->trainNetwork(m_NNPlayer2, m_database);
            m_dataController->addToTrainingStats(results, Elements::PLAYER_2);

            //Reset the database to write to the user games file.
            m_database->setDBFile(FILENAME_USER_GAMES);
        }
    }

    else
    {
        if(m_NNPlayer1 != NULL && m_NNPlayer2 != NULL)
        {
            results = m_AITrainer->trainTwoNetworks(m_NNPlayer1, m_NNPlayer2, m_database);
            //Todo: Add a method in dataController to add a total for both players.

            //Reset the database to write to the user games file.
            m_database->setDBFile(FILENAME_USER_GAMES);
        }
    }
}

//Load NN Player 1 or 2 as active player 1 or 2 respectively.
void GameController::swapHumanForAI(Elements::PlayerType player)
{
    if(player == Elements::PLAYER_1)
    {/*
        m_player1 = (Player *)m_NNPlayer1;

        //CurrentPlayer won't change when m_player1 does.
        //It has to be moved manually.
        if(m_currentPlayer == m_human1)
            m_currentPlayer = (Player *)m_player1;*/

        p1IsAI = true;
    }
    else
    {
        /*
        m_player2 = m_NNPlayer2;

        //CurrentPlayer won't change when m_player2 does.
        //It has to be moved manually.
        if(m_currentPlayer == m_human2)
            m_currentPlayer = m_player2;*/
        p2IsAI = true;
    }
}

//Load Human Player 1 or 2 as active player 1 or 2 respectively.
void GameController::swapAIForHuman(Elements::PlayerType player)
{
    if(player == Elements::PLAYER_1)
    {
        /*
        m_player1 = m_human1;

        //CurrentPlayer won't change when m_player1 does.
        //It has to be moved manually.
        if(m_currentPlayer == m_NNPlayer1)
            m_currentPlayer = m_player1;*/
        p1IsAI = false;
    }
    else
    {
        /*
        m_player2 = m_human2;

        //CurrentPlayer won't change when m_player2 does.
        //It has to be moved manually.
        if(m_currentPlayer == m_NNPlayer2)
            m_currentPlayer = m_player2;*/
        p2IsAI = false;
    }
}

//Check a player to see if it is an AI.  Return true if it is.
bool GameController::isAI(Elements::PlayerType player)
{
    Player *toCheck;
    if(player == Elements::PLAYER_1)
    {
        //toCheck = m_player1;
        return p1IsAI;
    }
    else
    {
        //toCheck = m_player2;
        return p2IsAI;
    }

    //if(typeid(*toCheck) == typeid(NeuralNetPlayer))
    //    return true;

    //return false;
}
