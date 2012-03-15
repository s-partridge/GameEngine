#include "neuralnetbuilder.h"

TDNeuralNetPlayer *TTTNNBuilder::buildNeuralNet(Elements::PlayerType player, RulesEngine *rulesEngine, string filename)
{
    //Initialize all the data needed for a new tic-tac-toe playing neural network.
    int numInputs = NUM_INPUTS;
    int numHiddenLayers = NUM_LAYERS;
    int *numHiddenNeurons = new int[numHiddenLayers];
    int numOutputNeurons = NUM_OUTPUTS;
    double *momentums = new double[numHiddenLayers + 1];
    double *learningRates = new double[numHiddenLayers + 1];

    //Two hiden layers, both with 27 neurons.
    numHiddenNeurons[0] = 45;
    numHiddenNeurons[1] = 45;

    //Give the same momentum and learning rate to all layers.
    for(int x = 0; x <= numHiddenLayers; ++x)
    {
        momentums[x] = MOMENTUM;
        learningRates[x] = LEARN_RATE;
    }

    TDNeuralNetPlayer *newNetwork;

    newNetwork = new TDNeuralNetPlayer(player, TD_EXPECTED_ROUNDS);
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

    //Modify the function to allow outputs between -1 and 1.
    ((Sigmoid *)activation)->setVerticalStretchFactor(2);
    ((Sigmoid *)activation)->setVerticalShiftFactor(-1);
    newNetwork->setActivationFunction(activation, 2);

    //Manage local memory.
    delete [] learningRates;
    delete [] momentums;
    delete [] numHiddenNeurons;

    return newNetwork;
}

TDNeuralNetPlayer *TTTNNBuilder::loadNeuralNet(Elements::PlayerType player, RulesEngine *rulesEngine, string filename)
{
    TDNeuralNetPlayer *newNetwork = new TDNeuralNetPlayer(player, TD_EXPECTED_ROUNDS);

    newNetwork->setNeuralNetworkFromFile(filename);
    newNetwork->setRulesEngine(rulesEngine);

    //Create a seprate instance for each layer.
    ActivationFunctor *activation = new Sigmoid();
    newNetwork->setActivationFunction(activation, 0);
    activation = new Sigmoid();
    newNetwork->setActivationFunction(activation, 1);

    //Output layer.
    activation = new Sigmoid();

    //Modify the function to allow outputs between -1 and 1.
    ((Sigmoid *)activation)->setVerticalStretchFactor(2);
    ((Sigmoid *)activation)->setVerticalShiftFactor(-1);
    newNetwork->setActivationFunction(activation, 2);

    return newNetwork;
}
