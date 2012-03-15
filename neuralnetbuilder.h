#ifndef NEURALNETBUILDER_H
#define NEURALNETBUILDER_H

#include "trainer.h"
#include "rulesengine.h"
#include "datatypes.h"
#include "tdneuralnetplayer.h"

//Interface for a neural network factory.
class NeuralNetBuilder
{
public:
    //Generate a new neural network.
    virtual TDNeuralNetPlayer *buildNeuralNet(Elements::PlayerType player, RulesEngine *rulesEngine, string filename) = 0;
    //Generate a neural network from a data file.
    virtual TDNeuralNetPlayer *loadNeuralNet(Elements::PlayerType player, RulesEngine *rulesEngine, string filename) = 0;
};

//Generates a neural network player for Tic-Tac-Toe
class TTTNNBuilder : public NeuralNetBuilder
{
public:
    TDNeuralNetPlayer *buildNeuralNet(Elements::PlayerType player, RulesEngine *rulesEngine, string filename);
    TDNeuralNetPlayer *loadNeuralNet(Elements::PlayerType player, RulesEngine *rulesEngine, string filename);

};

#endif // NEURALNETBUILDER_H
