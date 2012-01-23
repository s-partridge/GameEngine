#include "datacontroller.h"

bool DataController::isGameOver(Grid *move)
{
    //Return true if the game is not in a normal state.
    if(m_rulesEngine->testBoard(move) != Elements::NORMAL)
    {
        return true;
    }

    return false;
}

void DataController::addToTrainingStats(AITrainingStats toAdd, Elements::PlayerType player)
{
    if(player == Elements::PLAYER_1)
    {
        m_statistics->addToP1TrainingStats(toAdd);
    }
    else
    {
        m_statistics->addToP2TrainingStats(toAdd);
    }
}

void DataController::resetTrainingStats()
{
    m_statistics->resetP1TrainingStats();
    m_statistics->resetP2TrainingStats();
}

void DataController::resetStatistics()
{
    resetGameStats();
    resetTotalStats();
    resetTrainingStats();
}
