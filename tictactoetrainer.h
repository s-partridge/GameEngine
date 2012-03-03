#ifndef TICTACTOETRAINER_H
#define TICTACTOETRAINER_H

#include <map>
#include "trainer.h"
#include "tictactoegrid.h"
#include "boardstate.h"
#include "datatypes.h"
#include "movetablebuilder.h"

#define NUM_CHOICES 2

class TicTacToeTrainer : public Trainer
{
    BoardState *pickNextMoveToTrain(BoardState *currentState) const;
    AITrainingStats trainOnBestTrackPlus(NeuralNetPlayer *player) const;
    AITrainingStats trainOnBestStatesOnly(NeuralNetPlayer *player) const;

    void trainOnce(NeuralNetPlayer *player, Grid *gameBoard, Grid *userOutput,
                   double *&expected, double *&outputs, Elements::PlayerType currentPlayer,
                   map<string, int> &hashmap) const;

public:
    TicTacToeTrainer(int numTrainingIterations, RulesEngine *rulesEngine) : Trainer(numTrainingIterations, rulesEngine) {}

    AITrainingStats trainNetwork(NeuralNetPlayer *player) const;
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
