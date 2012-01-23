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

#include "neuralnetwork.h"

class Trainer
{
    int m_trainingIterations;
public:
    Trainer() : m_trainingIterations(1) {}

    void setNumIterations(int numTrainingIterations) { m_trainingIterations = numTrainingIterations; }
    virtual void trainNetwork(NeuralNetwork *network) const = 0;
};

#endif // TRAINER_H
