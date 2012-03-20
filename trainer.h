///////////////////////////////////////////////////////////////////////////////
//
//  file:       trainer.h
//  author:     Samuel Partridge
//  project:    Game Engine
//  purpose:    Interface for training a neural network.  Offers an easier
//              method of implementing different training patterns for testing.
//
///////////////////////////////////////////////////////////////////////////////


#ifndef TRAINER_H
#define TRAINER_H

//For rand(), time, and srand()
#include <stdlib.h>
#include <ctime>

#include "neuralnetplayer.h"
#include "statstructures.h"
#include "rulesengine.h"

class Trainer
{
protected:
    RulesEngine *m_rulesEngine;
    int m_numTrainingIterations;

public:
    Trainer(int numTrainingIterations, RulesEngine *rulesEngine) : m_rulesEngine(rulesEngine), m_numTrainingIterations(numTrainingIterations) {}

    void setNumIterations(int numTrainingIterations) { m_numTrainingIterations = numTrainingIterations; }
    virtual AITrainingStats trainNetwork(NeuralNetPlayer *player) const = 0;
    virtual AITrainingStats trainTwoNetworks(NeuralNetPlayer *player1, NeuralNetPlayer *player2) const {}
};

#endif // TRAINER_H
