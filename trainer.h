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
#include "gamedatabase.h"

class Trainer
{
protected:
    RulesEngine *m_rulesEngine;
    int m_numTrainingIterations;
    int m_printoutInterval;

public:
    Trainer(int numTrainingIterations, RulesEngine *rulesEngine) : m_rulesEngine(rulesEngine), m_numTrainingIterations(numTrainingIterations), m_printoutInterval(100) {}

    void setNumIterations(int numTrainingIterations) { m_numTrainingIterations = numTrainingIterations; }
    void setPrintoutInterval(int printoutInterval) { m_printoutInterval = printoutInterval; }

    virtual AITrainingStats trainNetwork(NeuralNetPlayer *player, GameDatabase *database) = 0;
    virtual AITrainingStats trainTwoNetworks(NeuralNetPlayer *player1, NeuralNetPlayer *player2, GameDatabase *databse) = 0;
};

#endif // TRAINER_H
