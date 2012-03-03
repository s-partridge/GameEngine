#include "neuralnetbuilder.h"

NeuralNetPlayer *TTTNNBuilder::buildNeuralNet(Elements::PlayerType player, RulesEngine *rulesEngine, string filename)
{
    //Initialize all the data needed for a new tic-tac-toe playing neural network.
    int numInputs = NUM_INPUTS;
    int numHiddenLayers = NUM_LAYERS;
    int *numHiddenNeurons = new int[numHiddenLayers];
    int numOutputNeurons = NUM_OUTPUTS;
    double *momentums = new double[numHiddenLayers + 1];
    double *learningRates = new double[numHiddenLayers + 1];

    //Two hiden layers, both with 27 neurons.
    numHiddenNeurons[0] = 27;
    numHiddenNeurons[1] = 27;

    //Give the same momentum and learning rate to all layers.
    for(int x = 0; x <= numHiddenLayers; ++x)
    {
        momentums[x] = MOMENTUM;
        learningRates[x] = LEARN_RATE;
    }

    NeuralNetPlayer *newNetwork;

    newNetwork = new NeuralNetPlayer(player);
    newNetwork->generateNeuralNetwork(filename, numInputs, numOutputNeurons, numHiddenLayers,
                                      numHiddenNeurons, momentums, learningRates);

    newNetwork->setRulesEngine(rulesEngine);

    //Create a seprate instance for each layer.
    ActivationFunctor *activation = new Sigmoid();
    newNetwork->setActivationFunction(activation, 0);
    activation = new Sigmoid();
    newNetwork->setActivationFunction(activation, 1);

    //Output layer.
    activation = new Sigmoid();
    //Stretch the output of the function.  Allows the network to output between 0 and 8;
    //one whole number for each board square.
    ((Sigmoid *)activation)->setVerticalStretchFactor(8);
    newNetwork->setActivationFunction(activation, 2);

    //Manage local memory.
    delete [] learningRates;
    delete [] momentums;
    delete [] numHiddenNeurons;

    return newNetwork;
}

NeuralNetPlayer *TTTNNBuilder::loadNeuralNet(Elements::PlayerType player, RulesEngine *rulesEngine, string filename)
{
    NeuralNetPlayer *newNetwork = new NeuralNetPlayer(player);

    newNetwork->setNeuralNetworkFromFile(filename);
    newNetwork->setRulesEngine(rulesEngine);

    //Create a seprate instance for each layer.
    ActivationFunctor *activation = new Sigmoid();
    newNetwork->setActivationFunction(activation, 0);
    activation = new Sigmoid();
    newNetwork->setActivationFunction(activation, 1);

    //Output layer.
    activation = new Sigmoid();
    //Stretch the output of the function.  Allows the network to output between 0 and 8;
    //one whole number for each board square.
    ((Sigmoid *)activation)->setVerticalStretchFactor(8);
    newNetwork->setActivationFunction(activation, 2);

    return newNetwork;
}
