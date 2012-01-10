#include "activation.h"

double Sigmoid::Activation(double input)
{
    //Simplifying the function by separating it into parts.
    double eToNegX = pow(e, -(input));

    m_output = 1 / (1 + eToNegX);

    //Spit out the result.
    return m_output;
}

double Sigmoid::simplifiedDerivative(double output)
{
    //The derivative used in "Intro to Neural Networks"
    return (output * (1 - output));
}

double Sigmoid::trueDerivative(double input)
{
    double derived;
    double eToTheX = (pow(e, -input));
    double onePlusEToTheX = 1 + eToTheX;
    double onePlusEToTheXSquared = (pow((onePlusEToTheX), 2));

    //print(cout, "\tThe derivative of the activation function is: (e^(x)) / (1 + e^(x))^2\n\t\t(e^x) = ", eToTheX, "\n");
    //print(cout, "\t\t1 + e^(x) = ", onePlusEToTheX, "\n");
    //print(cout, "\t\t(1 + e^(x))^2 = ", onePlusEToTheXSquared, "\n");

    derived = eToTheX / onePlusEToTheXSquared;

    return derived;
}
