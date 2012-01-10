#ifndef ACTIVATION_H
#define ACTIVATION_H

struct ActivationFunctor
{
    virtual double activation(double input) = 0;

    //Depending on the activation function, the simplified derivative may be
    //faster to calculate than the true derivative.  It is important to note
    //that the simplified derivative takes the OUTPUT of a neuron as its
    //input, while the true derivative uses the sum of a neuron's INPUTS.
    virtual double simplifiedDerivative(double output) = 0;
    virtual double trueDerivative(double input) = 0;
};

struct Sigmoid : public ActivationFunctor
{
    double activation(double input);
    double simplifiedDerivative(double output);
    double trueDerivative(double input);
};

#endif // ACTIVATION_H
