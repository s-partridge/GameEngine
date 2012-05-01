#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <string>
#include "neuronlayer.h"
#include "activation.h"

using namespace std;

class NeuralNetwork
{
public:
    //Everything needed is stored in a neural network file.
    NeuralNetwork(string filename);

    //Should pass in values for each hidden layer and the output layer.
    NeuralNetwork(string filename, const int numInputs, const int numOutputNeurons,
                  const int numHiddenLayers, const int *numHiddenNeurons,
                  const double *momentum, const double *learnRate);

    void purge();

    //Manually set the neuron weights.
    void setNeuronLayerWeights(int layerNumber, double **weights);

    ~NeuralNetwork() { purge(); }

    //Returns the number of neuron layers, including both hidden and output.
    int getNumLayers() const { return m_numHiddenLayers + 1; }
    int getNumInputs() const { return m_numInputs; }
    int getNumOutputs() const { return m_numOutputs; }
    int getNumHiddenNeurons(int layer) { return (layer < m_numHiddenLayers && layer >= 0) ? m_hiddenLayers[layer].getNumNeurons() : 0; }

    void setLayerWeights(int layer, double **weights);
    void setLayerMomentum(int layer, double momentum);
    void setLayerLearnRate(int layer, double learnRate);

    void setLayerActivation(int layer, ActivationFunctor *activation);

    void saveNNP();
    void saveNNP(string filename);

    void getResults(const double *inputs, double *&outputs);
    void backpropagate(double *actual, double *expected);

    void applyWeightChanges();
private:
    //Collects all the values stored in the neuron layers and copies them to the given double arrays.
    //The parameters are expected to be NULL.  These are used with saveNNP() to extract all needed data
    //from the neural network's layers.
    void getLayerValues(double ***&weights, double *&momentums, double *&learnRates, int *&numNeurons, int *&numInputs) const;
    void destroyLayerValues(double ***&weights, double *&momentums, double *&learnRates, int *&numNeurons, int *&numInputs) const;

    NeuronLayer *m_hiddenLayers;
    ActivationFunctor *m_activation;

    int m_numHiddenLayers;
    int m_numInputs;

    OutputLayer m_outputLayer;
    int m_numOutputs;

    string m_filename;
};

#endif // NEURALNETWORK_H
