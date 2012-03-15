#ifndef TDNEURALNETWORK_H
#define TDNEURALNETWORK_H

#include "neuralnetwork.h"

class TDNeuralNetwork : public NeuralNetwork
{
public:
    TDNeuralNetwork(string filename) : NeuralNetwork(filename) {}
    TDNeuralNetwork(string filename, const int numInputs, const int numOutputNeurons,
                  const int numHiddenLayers, const int *numHiddenNeurons,
                  const double *momentum, const double *learnRate)
        : NeuralNetwork(filename, numInputs, numOutputNeurons, numHiddenLayers,
                        numHiddenNeurons, momentum, learnRate) { }

    //Trains the network on a different set of data than backpropagate.
    //TD-lambda learning relies on the summation of previous moves in a game to
    //change the weights for the current board state.
    void tdBackpropagate(double *actual, double *expected, double *prevSummation);
};

#endif // TDNEURALNETWORK_H
