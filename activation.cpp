#include "activation.h"

void Sigmoid::setHorizontalStretchFactor(double stretch)
{
    hStretchFactor = stretch;
}

void Sigmoid::setVerticalStretchFactor(double stretch)
{
    vStretchFactor = stretch;
    vFactorSqrt = sqrt(stretch);
}

void Sigmoid::setVerticalShiftFactor(double shift)
{
    vShiftFactor = shift;
}

double Sigmoid::activation(double input) const
{
    //Complete equation:
    //          vStretchFactor
    //    ----------------------------
    //          (- hStretchFactor * x)
    //    1 + e^

    if(hStretchFactor != 1.0)
        input *= hStretchFactor;

    //Simplifying the function by separating it into parts.
    double eToNegX = pow(e, -(input));

    //By default, stretchFactor will be 1.
    double m_output = vStretchFactor / (1 + eToNegX) + vShiftFactor;

    //Spit out the result.
    return m_output;
}

double Sigmoid::simplifiedDerivative(double output) const
{
    //The derivative used in "Intro to Neural Networks"
    //If the output from the original function was vertically shifted, it must be "un-shifted"
    //before the derivative of the input can be found.  Otherwise, the results will be invalid.
    output -= vShiftFactor;
    if(hStretchFactor != 1.0)
        return hStretchFactor * ((output / vFactorSqrt) * (vFactorSqrt - output / vFactorSqrt));
    else
        return ((output / vFactorSqrt) * (vFactorSqrt - output / vFactorSqrt));
}

double Sigmoid::trueDerivative(double input) const
{
    double derived;
    double eToTheX = (pow(e, -input));
    double onePlusEToTheX = 1 + eToTheX;
    double onePlusEToTheXSquared = (pow((onePlusEToTheX), 2));

    //print(cout, "\tThe derivative of the activation function is: (e^(x)) / (1 + e^(x))^2\n\t\t(e^x) = ", eToTheX, "\n");
    //print(cout, "\t\t1 + e^(x) = ", onePlusEToTheX, "\n");
    //print(cout, "\t\t(1 + e^(x))^2 = ", onePlusEToTheXSquared, "\n");

    derived = (vStretchFactor * eToTheX) / onePlusEToTheXSquared;

    if(hStretchFactor != 1.0)
        derived *= hStretchFactor;
    return derived;
}

double Linear::activation(double input) const
{
    return vStretchFactor * input + vShiftFactor;
}

double Linear::simplifiedDerivative(double output) const
{
    return vStretchFactor;
}

double Linear::trueDerivative(double input) const
{
    return vStretchFactor;
}
