#ifndef CONNECTFOURTRAINER_H
#define CONNECTFOURTRAINER_H

#include "trainer.h"
#include "connectfourgrid.h"
#include "boardstate.h"
#include "datatypes.h"

class ConnectFourTrainer : public Trainer
{
    AITrainingStats trainVersusSelf(NeuralNetPlayer *player) const;
    AITrainingStats trainVersusTerriblePlayer(NeuralNetPlayer *player) const;
public:
    ConnectFourTrainer(int numTrainingIterations, RulesEngine *rulesEngine) : Trainer(numTrainingIterations, rulesEngine) {}

    AITrainingStats trainNetwork(NeuralNetPlayer *player) const;
    AITrainingStats trainTwoNetworks(NeuralNetPlayer *player1, NeuralNetPlayer *player2) const;
};

#endif // CONNECTFOURTRAINER_H
