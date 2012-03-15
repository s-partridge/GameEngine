#ifndef NEURONLAYER_H
#define NEURONLAYER_H

#include <ctime>
#include <stdlib.h>
#include "neuron.h"
#include "activation.h"

class NeuronLayer
{
public:
    NeuronLayer() {}

    //Generate neurons and initialize the m_blames and m_accBlames arrays.
    void init(const int numNeurons, const int numInputs);

    void purge();

    ~NeuronLayer() { purge(); }

    void setActivation(ActivationFunctor *activation) { m_activation = activation; }
    void setWeightsForNeuron(int index, const double *weights);

    //Create weights for this layer's neurons.  Copy the values to weightResult.
    int generateLayerWeights(double *&weightResult);

    //Copy source into dest, starting at a given destination index, given the number of source elements.
    static void copyArray(const double *source, double *&dest, int destStartIndex, int sourceNumElements);
    static void copyArray(const double *source, double *&dest, int sourceNumElements);

    //Set the momentum value for this layer.
    void setMomentum(double momentum) { m_momentum = momentum; }
    double getMomentum() const { return m_momentum; }

    //Set the learning rate for this layer.
    void setLearnRate(double learnRate) { m_learnRate = learnRate; }
    double getLearnRate() const { return m_learnRate; }

    //Calculate outputs from each neuron.
    void getResult(const double *inputs, double *&outputs);

    int getNumInputs() const { return m_neurons[0].getNumInputs(); }
    int getNumNeurons() const { return m_numNeurons; }


    //Calculate blames for this layer and return the results.
    double *calcBlames(const double *nextLayerErrors, double **nextLayerWeightMatrix, int numErrors);

    double *calcBlameDeltas();

    //Reset accBlames to 0.
    void resetAccBlames();

    //Calc accumulated blames.
    //These are the values that are actually applied to neuron weights.
    //The term "accumulated" refers to the fact that, if there is more than one training set
    //per iteration, accBlames will accumulate errors from each set.  With my current implementation,
    //I don't plan on doing that.  However, the terminology will stay regardless.
    void calcAccBlames();

    double *getBlames() { return m_blames; }

    //Applies m_accBlames to the weights in m_neurons.
    void changeWeights();

    //Creates a matrix containing one slot for each weight in the layer, uninitialized.
    void getEmptyWeightMatrix(double **&weightMatrix) const;
    //Copies all weights from the neuron layer into a matrix.
    void getWeightMatrix(double **&weightMatrix) const;
    //Deletes the weight matrix passed in.  Assumes the matrix came from this layer.
    void destroyWeightMatrix(double **&weightMatrix) const;

protected:
    //Add momentum to weightMatrix.
    void addMomentum(double **&weightMatrix) const;

    //Add the learning rate weightMatrix.
    void addLearnRate(double **&weightMatrix) const;

    double *m_blames;

    //The values stored here will be applied to neuron weights when
    //changeWeights() is called.
    double **m_accBlames;
    //Stores values used with momentum.  These are multiplied with
    //weightChanges to produce smaller (or larger) changes as need be.
    double **m_momentumChanges;

    Neuron *m_neurons;
    int m_numNeurons;

    double m_momentum;
    double m_learnRate;

    //Temorarily stores values to be added to neuron weights.
    double *m_weightChanges;

    ActivationFunctor *m_activation;

};

//This subclass allows the output layer to be treated differently than hidden layers.
//The only difference in the way the output layer is handled is in the calculation
//of blame values.  However, it is an important enough difference to merit a separate
//class.
class OutputLayer : public NeuronLayer
{
public:
    OutputLayer() {}
    double *calcOutputBlames(const double *expectedOutput);
    double *calcOutputBlames(const double *actual, const double *expectedOutput);
};

#endif // NEURONLAYER_H
