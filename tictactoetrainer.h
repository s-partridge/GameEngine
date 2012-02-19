#ifndef TICTACTOETRAINER_H
#define TICTACTOETRAINER_H

#include "trainer.h"
#include "tictactoegrid.h"
#include "boardstate.h"
#include "datatypes.h"

#define NUM_CHOICES 2

class TicTacToeTrainer : public Trainer
{
    BoardState *pickNextMoveToTrain(BoardState *currentState) const;
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
