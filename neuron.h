#ifndef NEURON_H
#define NEURON_H

#include "activation.h"
#include "macros.h"

class Neuron
{
public:
    Neuron() : m_numWeights(0), m_numInputs(0) {}

    int init(const int numWeights);

    void purge();

    ~Neuron();

    //Set and get weight values.
    void setWeights(const double *weights);
    void setWeight(const int index, const double value) { m_weights[index] = value; }
    double getWeight(const int index) { return m_weights[index]; }
    double *getWeights() { return m_weights; }

    int getNumWeights() { return m_numWeights; }

    //Set and get the bias value.
    void setBias(const double value) { m_weights[m_numWeights - 1] = value; }
    double getBias() { return m_weights[m_numWeights - 1]; }

    //Change weights in the neuron. Adds these changes to the existing weights.
    //changes is expected to be the same size as weights.
    void changeWeights(const double *changes);

    void setInputs(const double *inputs);
    void setInput(const int index, const int input) { m_inputs[index] = input; }
    double getInput(const int index) { return m_inputs[index]; }
    double *getInputs() { return m_inputs; }

    int getNumInputs() { return m_numInputs; }

    double getOutput() { return m_output; }

    //Multiply each input by its corresponding weight, sum those results,
    //and return the total.
    double summation();

    //Call the derivative and activation functions from the function object passed in.
    //Allows for an easy method of swapping activation functions.
    double activation(const ActivationFunctor *activate) { activate->activation(summation()); }
    double derivative(const ActivationFunctor *derivate) { derivate->simplifiedDerivative(m_output); }

private:
    double *m_inputs;
    double *m_weights;
    double m_output;
    int m_numInputs;
    int m_numWeights;
};

#endif // NEURON_H
