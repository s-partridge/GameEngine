#ifndef C4NNBUILDER_H
#define C4NNBUILDER_H

#include "neuralnetbuilder.h"
#include "c4constants.h"

class C4NNBuilder : public NeuralNetBuilder
{
public:
    TDNeuralNetPlayer *buildNeuralNet(Elements::PlayerType player, RulesEngine *rulesEngine, string filename);
    TDNeuralNetPlayer *loadNeuralNet(Elements::PlayerType player, RulesEngine *rulesEngine, string filename);
};

#endif // C4NNBUILDER_H
