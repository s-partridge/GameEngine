#include "c4nnbuilder.h"

TDNeuralNetPlayer *C4NNBuilder::buildNeuralNet(Elements::PlayerType player, RulesEngine *rulesEngine, string filename)
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
    //Stretch the output of the function.  Allows the network to output between 0 and 6;
    //one whole number for each board column.
/*
    ((Sigmoid *)activation)->setVerticalStretchFactor(2);
    ((Sigmoid *)activation)->setVerticalShiftFactor(-1);
*/
    //Represent a value between 0 (loss) and 10 (win)
    ((Sigmoid *)activation)->setVerticalStretchFactor(2);
    ((Sigmoid *)activation)->setVerticalShiftFactor(-1);
    ((Sigmoid *)activation)->setHorizontalStretchFactor(0.4);

    newNetwork->setActivationFunction(activation, 2);

    //Manage local memory.
    delete [] learningRates;
    delete [] momentums;
    delete [] numHiddenNeurons;

    return newNetwork;
}

TDNeuralNetPlayer *C4NNBuilder::loadNeuralNet(Elements::PlayerType player, RulesEngine *rulesEngine, string filename)
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
    //Stretch the output of the function.  Allows the network to output between 0 and 6;
    //one whole number for each board column.
    ((Sigmoid *)activation)->setVerticalStretchFactor(2);
    ((Sigmoid *)activation)->setVerticalShiftFactor(-1);
    ((Sigmoid *)activation)->setHorizontalStretchFactor(0.4);

    newNetwork->setActivationFunction(activation, 2);

    return newNetwork;
}
