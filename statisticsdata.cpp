#include "statisticsdata.h"

StatisticsData::StatisticsData()
{
    m_currentGame.init();
    m_totalStats.init();
    m_p1Training.init();
    m_p2Training.init();
}

void StatisticsData::addToGameStats(GameStats currentGame)
{
    m_currentGame += currentGame;
    m_totalStats += currentGame;
}

