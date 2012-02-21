#ifndef CONNECTFOURTRAINER_H
#define CONNECTFOURTRAINER_H

#include "trainer.h"
#include "connectfourgrid.h"
#include "boardstate.h"
#include "datatypes.h"

class ConnectFourTrainer : public Trainer
{

public:
    ConnectFourTrainer(int numTrainingIterations, RulesEngine *rulesEngine) : Trainer(numTrainingIterations, rulesEngine) {}

    AITrainingStats trainNetwork(NeuralNetPlayer *player) const;
};

#endif // CONNECTFOURTRAINER_H
