#ifndef STATISTICSDATA_H
#define STATISTICSDATA_H

#include "statstructures.h"

class StatisticsData
{
public:
    StatisticsData();

    GameStats getCurrentStats() { return m_currentGame; }
    GameStats getTotalStats() { return m_totalStats; }

    AITrainingStats getP1TrainingStats() { return m_p1Training; }
    AITrainingStats getP2TrainingStats() { return m_p2Training; }

    void addToGameStats(GameStats currentGame);
    void addToTotalStats(GameStats total);

    void addToP1TrainingStats(AITrainingStats stats);
    void addToP2TrainingStats(AITrainingStats stats);

    void resetCurrentStats() { m_currentGame.init(); }
    void resetTotalStats() { m_totalStats.init(); }
    void resetP1TrainingStats() { m_p1Training.init(); }
    void resetP2TrainingStats() { m_p2Training.init(); }

private:
    GameStats m_currentGame;
    GameStats m_totalStats;
    AITrainingStats m_p1Training;
    AITrainingStats m_p2Training;
};

#endif // STATISTICSDATA_H
