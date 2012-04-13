#ifndef CONNECTFOURTRAINER_H
#define CONNECTFOURTRAINER_H

#include "trainer.h"
#include "connectfourgrid.h"
#include "connectfourrulesengine.h"
#include "boardstate.h"
#include "datatypes.h"

#define NUM_FUNCTIONS 2
//Used to compare four board pieces and determine if exactly two are of type opp.
#define compareC4Set(w, x, y, z, opp) ( (w == opp && x == opp && y == opp && z != w) || (w == opp && x == opp && y != w && z == opp) || (w == opp && x != w && y == opp && z == opp) || (w != opp && x == opp && y == opp && z == opp) )

class ConnectFourTrainer : public Trainer
{

    static BoardState *moveVertical(BoardState *&currentState, Elements::PlayerType friendly, Elements::PlayerType opponent);
    /*
    BoardState *moveHorizontal(BoardState *&currentState, Elements::PlayerType friendly, Elements::PlayerType opponent) const;
    */

    AITrainingStats trainVersusSelf(NeuralNetPlayer *player) const;
    AITrainingStats trainVersusTerriblePlayer(NeuralNetPlayer *player) const;
    AITrainingStats trainVersusMultiple(NeuralNetPlayer *player) const;
    AITrainingStats trainVersusMoveBlocker(NeuralNetPlayer *player) const;
public:
    ConnectFourTrainer(int numTrainingIterations, RulesEngine *rulesEngine) : Trainer(numTrainingIterations, rulesEngine) {}

    static BoardState *moveBlocker(BoardState *&currentState, Elements::PlayerType friendly, Elements::PlayerType opponent);
    AITrainingStats trainNetwork(NeuralNetPlayer *player) const;
    AITrainingStats trainTwoNetworks(NeuralNetPlayer *player1, NeuralNetPlayer *player2) const;
};

#endif // CONNECTFOURTRAINER_H
