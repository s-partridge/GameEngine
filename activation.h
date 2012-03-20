#ifndef ACTIVATION_H
#define ACTIVATION_H

#include <cmath>
#include "datatypes.h"

class ActivationFunctor
{
public:
    virtual double activation(double input) const = 0;

    //Depending on the activation function, the simplified derivative may be
    //faster to calculate than the true derivative.  It is important to note
    //that the simplified derivative takes the OUTPUT of a neuron as its
    //input, while the true derivative uses the sum of a neuron's INPUTS.
    virtual double simplifiedDerivative(double output) const = 0;
    virtual double trueDerivative(double input) const = 0;
};

class Sigmoid : public ActivationFunctor
{
    //stretchFactor allows the function to map a larger range of outputs,
    //rather than only giving results between 0.0 and 1.0.
    double hFactorSqrt, vFactorSqrt;
    double hStretchFactor, vStretchFactor;
    double vShiftFactor;

public:
    Sigmoid() :  hFactorSqrt(1), vFactorSqrt(1), hStretchFactor(1), vStretchFactor(1), vShiftFactor(0) {}
    void setVerticalStretchFactor(double stretch);
    void setHorizontalStretchFactor(double stretch);
    void setVerticalShiftFactor(double shift);

    double activation(double input) const;
    double simplifiedDerivative(double output) const;
    double trueDerivative(double input) const;
};

class Linear : public ActivationFunctor
{
    //y = stretch * x + shift
    double vShiftFactor, vStretchFactor;

public:
    Linear() : vStretchFactor(1), vShiftFactor(0) {}
    void setVerticalStretchFactor(double stretch) { vStretchFactor = stretch; }
    void setVerticalShiftFactor(double shift) { vShiftFactor = shift; }

    double activation(double input) const;
    double simplifiedDerivative(double output) const;
    double trueDerivative(double input) const;
};

#endif // ACTIVATION_H
