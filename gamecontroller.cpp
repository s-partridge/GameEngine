#include "gamecontroller.h"

GameController::GameController()
    : m_NNPlayer1(NULL), m_NNPlayer2(NULL), m_dataController(NULL),
      m_AIBuilder(NULL), m_AITrainer(NULL), m_rulesEngine(NULL)
{
    m_human1 = new Human();
    m_human2 = new Human();

    m_player1 = m_human1;
    m_player2 = m_human2;

    m_currentPlayer = m_player1;
}

//Delete everything that was created locally.
void GameController::purge()
{
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
}

//Generate a neural network and store its parameters in the file specified by filename.
void GameController::createNNPlayer(Elements::PlayerType player, string filename)
{
    NeuralNetPlayer newPlayer = m_AIBuilder->buildNeuralNet(m_AITrainer, m_rulesEngine, filename);

    if(player == Elements::PLAYER_1)
    {
        m_NNPlayer1 = newPlayer;
        m_NN1Filename = filename;
    }
    else
    {
        m_NNPlayer2 = newPlayer;
        m_NN2Filename = filename;
    }
}

//Load a neural network from a file specified by filename.
void GameController::loadNNPlayer(Elements::PlayerType player, string filename)
{
    NeuralNetPlayer newPlayer = m_AIBuilder->loadNeuralNet(m_AITrainer, m_rulesEngine, filename);

    //TODO: Make sure that currentPlayer is updated correctly.
    //  not sure whether the pointer will update to the correct memory address.
    //  It may stay at the human address and not move to the NN Player.
    if(player == Elements::PLAYER_1)
    {
        m_NNPlayer1 = newPlayer;
        m_NN1Filename = filename;
    }
    else
    {
        m_NNPlayer2 = newPlayer;
        m_NN2Filename = filename;
    }
}

Grid *GameController::attemptMove(const Grid *move)
{
    Grid *nextMove = NULL;

    //Only do this if the current player is an AI.
    //If human, should return NULL.
    if(typeid(NeuralNetPlayer) == typeid(*m_currentPlayer))
    {
        m_currentPlayer->makeMove(m_dataController->getCurrentState(), nextMove);
    }
    switchCurrentPlayer();

    return nextMove;
}

//Reset the move tree and current game stats.
void GameController::resetGame()
{
    if(m_dataController != NULL)
    {
        m_dataController->resetTree();
        m_dataController->resetGameStats();
    }
}

//Undo the last move made.
void GameController::undoMove()
{
    if(m_dataController != NULL)
    {
        m_dataController->undoMove();
    }
}

//Set either player 1 or player 2 as the active player.
void GameController::switchCurrentPlayer()
{
    if(m_currentPlayer == m_player1)
    {
        m_currentPlayer == m_player2;
    }
    else
    {
        m_currentPlayer == m_player1;
    }
}

//Train neural net player 1, 2, or both.
void GameController::trainAI(Elements::PlayerType player = Elements::NONE)
{
    AITrainingStats results;
    if(player == Elements::PLAYER_1 || player == Elements::NONE)
    {
        if(m_NNPlayer1 != NULL)
        {
            results = m_NNPlayer1->train();
            m_dataController->addToTrainingStats(results, PLAYER_1);
        }
    }

    if(player == Elements::PLAYER_2 || player == Elements::NONE)
    {
        if(m_NNPlayer2 != NULL)
        {
            results = m_NNPlayer2->train();
            m_dataController->addToTrainingStats(results, PLAYER_2);
        }
    }
}

//Load NN Player 1 or 2 as active player 1 or 2 respectively.
void GameController::swapHumanForAI(Elements::PlayerType player)
{
    if(player == Elements::PLAYER_1)
    {
        m_player1 = m_NNPlayer1;
    }
    else
    {
        m_player2 = m_NNPlayer2;
    }
}

//Load Human Player 1 or 2 as active player 1 or 2 respectively.
void GameController::swapAIForHuman(Elements::PlayerType player)
{
    if(player == Elements::PLAYER_1)
    {
        m_player1 = m_human1;
    }
    else
    {
        m_player2 = m_human2;
    }
}

//Check a player to see if it is an AI.  Return true if it is.
bool GameController::isAI(Elements::PlayerType player)
{
    Player *toCheck;
    if(player == Elements::PLAYER_1)
    {
        toCheck = m_player1;
    }
    else
    {
        toCheck = m_player2;
    }

    if(typeid(*toCheck) == typeid(NeuralNetPlayer))
        return true;

    return false;
}
