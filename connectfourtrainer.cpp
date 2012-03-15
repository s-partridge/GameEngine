#include "connectfourtrainer.h"

AITrainingStats ConnectFourTrainer::trainNetwork(NeuralNetPlayer *player) const
{
    AITrainingStats trainingStats;
    trainingStats.init();

    for(int x = 0; x < m_numTrainingIterations; ++x)
    {
        //Make the player train against itself.
    }

    return trainingStats;
}
