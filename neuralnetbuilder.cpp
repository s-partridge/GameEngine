#include "neuralnetbuilder.h"

NeuralNetPlayer *TTTNNBuilder::buildNeuralNet(Trainer *trainer, RulesEngine *rulesEngine, string filename)
{
    int numInputs = NUM_INPUTS;
    int numHiddenLayers = NUM_LAYERS;
    int *numHiddenNeurons = new int[numHiddenLayers];
    int numOutputNeurons = NUM_OUTPUTS;
    double *momentums = new double[numHiddenLayers + 1];
    double *learningRates = new double[numHiddenLayers + 1];

    numHiddenNeurons[0] = 27;
    numHiddenNeurons[1] = 27;

    for(int x = 0; x <= numHiddenLayers; ++x)
    {
        momentums[x] = MOMENTUM;
        learningRates[x] = LEARNING_RATE;
    }

    NeuralNetPlayer *newNetwork;

    newNetwork = new NeuralNetPlayer();
    newNetwork->generateNeuralNetwork(filename, numInputs, numOutputNeurons, numHiddenLayers,
                                      numHiddenNeurons, momentums, learningRates);

    newNetwork->setTrainer(trainer);
    newNetwork->setRulesEngine(rulesEngine);

    //Manage local memory.
    delete [] learningRates;
    delete [] momentums;
    delete [] numHiddenNeurons;

    return newNetwork;
}

NeuralNetworkPlayer *TTTNNBuilder::loadNeuralNet(Trainer *trainer, RulesEngine *rulesEngine, string filename)
{
    NeuralNetPlayer *newNetwork = new NeuralNetPlayer();

    newNetwork->setNeuralNetworkFromFile(filename);
    newNetwork->setTrainer(trainer);
    newNetwork->setRulesEngine(rulesEngine);

    return newNetwork;
}
