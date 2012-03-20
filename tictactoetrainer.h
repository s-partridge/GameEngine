#ifndef TICTACTOETRAINER_H
#define TICTACTOETRAINER_H

#include <map>
#include "trainer.h"
#include "tictactoegrid.h"
#include "boardstate.h"
#include "datatypes.h"
#include "movetablebuilder.h"

#define NUM_CHOICES 2

//Used to compare three board pieces and determine if exactly two are of type opp.
#define compareSet(x, y, z, opp) ( (x == opp && y == x && z != x) || (x == opp && y != x && z == x) || (x != y && y == opp && z == y) )

class TicTacToeTrainer : public Trainer
{
    BoardState *pickNextMoveToTrain(BoardState *currentState) const;
    AITrainingStats trainOnBestTrackPlus(NeuralNetPlayer *player) const;
    AITrainingStats trainOnBestStatesOnly(NeuralNetPlayer *player) const;

    AITrainingStats trainVersusSelf(NeuralNetPlayer *player) const;
    AITrainingStats trainVersusTerriblePlayer(NeuralNetPlayer *player) const;

    void moveBlocker(BoardState *&currentState, Elements::PlayerType friendly, Elements::PlayerType opponent) const;

    void trainOnce(NeuralNetPlayer *player, Grid *gameBoard, Grid *userOutput,
                   double *&expected, double *&outputs, Elements::PlayerType currentPlayer,
                   map<string, int> &hashmap) const;

public:
    TicTacToeTrainer(int numTrainingIterations, RulesEngine *rulesEngine) : Trainer(numTrainingIterations, rulesEngine) {}

    AITrainingStats trainNetwork(NeuralNetPlayer *player) const;
    AITrainingStats trainTwoNetworks(NeuralNetPlayer *player1, NeuralNetPlayer *player2) const;
};

/*
class CompleteTicTacToeTrainer : public Trainer
{
    BoardState *pickNextMoveToTrain(BoardState *currentState) const;
public:
    TicTacToeTrainer(int numTrainingIterations, RulesEngine *rulesEngine) : Trainer(numTrainingIterations, rulesEngine) {}

    AITrainingStats trainNetwork(NeuralNetPlayer *player) const;
};*/

#endif // TICTACTOETRAINER_H
