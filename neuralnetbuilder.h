#ifndef NEURALNETBUILDER_H
#define NEURALNETBUILDER_H

#include "neuralnetplayer.h"
#include "trainer.h"
#include "rulesengine.h"

//Interface for a neural network factory.
class NeuralNetBuilder
{
public:
    //Generate a new neural network.
    virtual NeuralNetPlayer *buildNeuralNet(Trainer *trainer, RulesEngine *rulesEngine, string filename) = 0;
    //Generate a neural network from a data file.
    virtual NeuralNetPlayer *loadNeuralNet(Trainer *trainer, RulesEngine *rulesEngine, string filename) = 0;
};

//Generates a neural network player for Tic-Tac-Toe
class TTTNNBuilder : NeuralNetBuilder
{
public:
    NeuralNetPlayer *buildNeuralnet(Trainer *trainer, RulesEngine *rulesEngine, string filename);
    NeuralNetPlayer *loadNeuralNet(Trainer *trainer, RulesEngine *rulesEngine, string filename);

};

#endif // NEURALNETBUILDER_H
