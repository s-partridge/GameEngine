#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <string>
#include "neuronlayer.h"
#include "activation.h"
class NeuralNetwork
{
public:
    //Everything needed is stored in a neural network file.
    NeuralNetwork(string filename);

    //Should pass in values for each hidden layer and the output layer.
    NeuralNetwork(string filename, const int numInputs, const int numOutputNeurons,
                  const int numHiddenLayers, const int *numHiddenNeurons,
                  const double *momentum, const double *learnRate);

    void setLayerMomentum(int layer, double momentum);
    void setLayerLearnRate(int layer, double learnRate);

    void setLayerActivation(int layer, ActivationFunctor *activation);

    void saveNNP();
    void saveNNP(string filename);

    void getResults(const double *inputs, double *outputs);
    void backpropagate(double *actual, double *expected);

private:
    NeuronLayer *m_hiddenLayers;
    int m_numHiddenLayers;
    OutputLayer m_outputLayer;
};

#endif // NEURALNETWORK_H
