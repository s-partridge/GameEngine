#include "c4nnbuilder.h"

NeuralNetPlayer *C4NNBuilder::buildNeuralNet(Elements::PlayerType player, RulesEngine *rulesEngine, string filename)
{
    //Initialize all the data needed for a new tic-tac-toe playing neural network.
    int numInputs = C4_NUM_INPUTS;
    int numHiddenLayers = C4_NUM_LAYERS;
    int *numHiddenNeurons = new int[numHiddenLayers];
    int numOutputNeurons = C4_NUM_OUTPUTS;
    double *momentums = new double[numHiddenLayers + 1];
    double *learningRates = new double[numHiddenLayers + 1];

    //Two hiden layers, both with 27 neurons.
    numHiddenNeurons[0] = C4_NUM_NEURONS_1;
    numHiddenNeurons[1] = C4_NUM_NEURONS_2;

    //Give the same momentum and learning rate to all layers.
    for(int x = 0; x <= numHiddenLayers; ++x)
    {
        momentums[x] = C4_MOMENTUM;
        learningRates[x] = C4_LEARN_RATE;
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
    //Stretch the output of the function.  Allows the network to output between 0 and 6;
    //one whole number for each board column.
    ((Sigmoid *)activation)->setVerticalStretchFactor(C4_OUTPUT_STRETCH);
    newNetwork->setActivationFunction(activation, 2);

    //Manage local memory.
    delete [] learningRates;
    delete [] momentums;
    delete [] numHiddenNeurons;

    return newNetwork;
}

NeuralNetPlayer *C4NNBuilder::loadNeuralNet(Elements::PlayerType player, RulesEngine *rulesEngine, string filename)
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
    //Stretch the output of the function.  Allows the network to output between 0 and 6;
    //one whole number for each board column.
    ((Sigmoid *)activation)->setVerticalStretchFactor(C4_OUTPUT_STRETCH);
    newNetwork->setActivationFunction(activation, 2);

    return newNetwork;
}
