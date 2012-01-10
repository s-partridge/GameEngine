#ifndef NEURONLAYER_H
#define NEURONLAYER_H

#include "neuron.h"
#include "activation.h"

class NeuronLayer
{
public:
    NeuronLayer();

    //Generate neurons and initialize the m_blames and m_accBlames arrays.
    void init(const int numNeurons, const int numInputs);

    void purge();

    ~NeuronLayer();

    void setActivation(ActivationFunctor *activation);
    void setWeightsForNeuron(int index, const double *weights) { m_neurons[index].setWeights(weights); }

    void setMomentum(double momentum) { m_momentum = momentum; }
    //Calculate outputs from each neuron.
    void getResult(const double *inputs, double *outputs);

    int getNumInputs() { return m_neruons[0].getNumInputs(); }
    int getNumNeurons() { return m_numNeurons; }


    //Calculate blames for this layer and return the results.
    double *calcBlames(const double *nextLayerErrors, int numErrors);
    void calcAccBlames();

    //Add momentum to m_accBlames.
    void addMomentum();

    //Add accBlames to momentum.  The new value will be used as momentum for
    //the next iteration.
    void setMomentum();

    //Applies m_accBlames to the weights in m_neurons.
    void changeWeights();

protected:
    double *m_blames;

    //The values stored here will be applied to neuron weights when
    //changeWeights() is called.
    double *m_accBlames;
    //Stores values used with momentum.  These are multiplied with
    //weightChanges to produce smaller (or larger) changes as need be.
    double **m_momentumChanges;

private:
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
    void calcOutputBlames(const double expectedOutput);
};

#endif // NEURONLAYER_H
