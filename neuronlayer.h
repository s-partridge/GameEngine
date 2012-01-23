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

    ~NeuronLayer();

    void setActivation(ActivationFunctor *activation) { m_activation = activation; }
    void setWeightsForNeuron(int index, const double *weights) { m_neurons[index].setWeights(weights); }

    //Create weights for this layer's neurons.  Copy the values to weightResult.
    void generateLayerWeights(double *weightResult);

    //Copy source into dest, starting at a given destination index, given the number of source elements.
    void arrayCopy(const double *source, double *dest, int destStartIndex, int sourceNumElements);

    //Set the momentum value for this layer.
    void setMomentum(double momentum) { m_momentum = momentum; }

    //Set the learning rate for this layer.
    void setLearnRate(double learnRate) { m_learnRate = learnRate; }

    //Calculate outputs from each neuron.
    void getResult(const double *inputs, double *outputs);

    int getNumInputs() { return m_neurons[0].getNumInputs(); }
    int getNumNeurons() { return m_numNeurons; }


    //Calculate blames for this layer and return the results.
    void calcBlames(const double *nextLayerErrors, int numErrors);

    void calcBlameDeltas();

    //Reset accBlames to 0.
    void resetAccBlames();

    //Calc accumulated blames.
    //These are the values that are actually applied to neuron weights.
    //The term "accumulated" refers to the fact that, if there is more than one training set
    //per iteration, accBlames will accumulate errors from each set.  With my current implementation,
    //I don't plan on doing that.  However, the terminology will stay regardless.
    void calcAccBlames();

    double *getBlames() { return m_blames; }

    //Add momentum to m_accBlames.
    void addMomentum();

    //Add accBlames to momentumChanges.  The new value will be used as momentumChanges for
    //the next iteration.
    void setMomentumChanges();

    //Applies m_accBlames to the weights in m_neurons.
    void changeWeights();

    void getWeightMatrix(double **weightMatrix);

protected:
    double *m_blames;

    //The values stored here will be applied to neuron weights when
    //changeWeights() is called.
    double *m_accBlames;
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
    void calcOutputBlames(const double expectedOutput);
};

#endif // NEURONLAYER_H
