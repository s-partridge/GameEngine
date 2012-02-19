#ifndef NEURALNETBUILDER_H
#define NEURALNETBUILDER_H

#include "neuralnetplayer.h"
#include "trainer.h"
#include "rulesengine.h"
#include "datatypes.h"

//Interface for a neural network factory.
class NeuralNetBuilder
{
public:
    //Generate a new neural network.
    virtual NeuralNetPlayer *buildNeuralNet(Elements::PlayerType player, RulesEngine *rulesEngine, string filename) = 0;
    //Generate a neural network from a data file.
    virtual NeuralNetPlayer *loadNeuralNet(Elements::PlayerType player, RulesEngine *rulesEngine, string filename) = 0;
};

//Generates a neural network player for Tic-Tac-Toe
class TTTNNBuilder : public NeuralNetBuilder
{
public:
    NeuralNetPlayer *buildNeuralNet(Elements::PlayerType player, RulesEngine *rulesEngine, string filename);
    NeuralNetPlayer *loadNeuralNet(Elements::PlayerType player, RulesEngine *rulesEngine, string filename);

};

#endif // NEURALNETBUILDER_H
