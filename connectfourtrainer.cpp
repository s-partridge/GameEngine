#include "connectfourtrainer.h"

AITrainingStats ConnectFourTrainer::trainNetwork(NeuralNetPlayer *player) const
{
    AITrainingStats trainingStats;
    trainingStats.init();

    for(int x = 0; x < m_numTrainingIterations; ++x)
    {
        //Can't complete until I find a way to check the move table.
    }

    return trainingStats;
}
