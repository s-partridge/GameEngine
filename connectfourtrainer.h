#ifndef CONNECTFOURTRAINER_H
#define CONNECTFOURTRAINER_H

#include "trainer.h"
#include "connectfourgrid.h"
#include "boardstate.h"
#include "datatypes.h"

//Used to compare four board pieces and determine if exactly two are of type opp.
#define compareC4Set(w, x, y, z, opp) ( (w == opp && x == opp && y == opp && z != w) || (w == opp && x == opp && y != w && z == opp) || (w == opp && x != w && y == opp && z == opp) || (w != opp && x == opp && y == opp && z == opp) )

class ConnectFourTrainer : public Trainer
{
    AITrainingStats trainVersusSelf(NeuralNetPlayer *player) const;
    AITrainingStats trainVersusTerriblePlayer(NeuralNetPlayer *player) const;
    AITrainingStats trainVersusMoveBlocker(NeuralNetPlayer *player) const;
public:
    ConnectFourTrainer(int numTrainingIterations, RulesEngine *rulesEngine) : Trainer(numTrainingIterations, rulesEngine) {}

    BoardState *moveBlocker(BoardState *&currentState, Elements::PlayerType friendly, Elements::PlayerType opponent) const;
    AITrainingStats trainNetwork(NeuralNetPlayer *player) const;
    AITrainingStats trainTwoNetworks(NeuralNetPlayer *player1, NeuralNetPlayer *player2) const;
};

#endif // CONNECTFOURTRAINER_H
