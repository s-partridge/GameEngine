#include "neuron.h"

int Neuron::init(const int numWeights)
{
    //Make sure the array is empty.
    if(m_weights == 0)
    {
        //Allocate space for the weights and the bias.
        m_numWeights = numWeights + 1;
        m_weights = new double[m_numWeights];

        //Debug statement
        //print(cout, "\t\tThere are ", m_numWeights, " weights in this neuron\n");
    }
    //The array already exists.
    else
        return 1;

    if(m_inputs == 0)
    {
        //There should be one weight per input.
        //There is no input for the bias.
        m_numInputs = numWeights;
        m_inputs = new double[m_numWeights];

        //Debug statement
        //print(cout, "\t\tThere are ", m_numInputs, " inputs to this neuron\n");
    }
    //The array already exists.
    else
        return 2;

    return 0;
}

//Set all weights at once.
void Neuron::setWeights(const double *weights)
{
    for(int x = 0; x < m_numWeights; ++x)
    {
        m_weights[x] = weights[x];
    }
}

//Add changes to each weight.
void Neuron::changeWeights(const double *changes)
{
    for(int x = 0; x < m_numWeights; ++x)
    {
        m_weights[x] += changes[x];
    }

}

//Set all inputs at once.
void Neuron::setInputs(const double *inputs)
{
    for(int x = 0; x < m_numInputs; ++x)
    {
        m_inputs[x] = inputs[x];
    }
}

//Add up all weights, multiplied by inputs, and return the total.
double Neuron::summation()
{
    double sum = 0.0;

    int x;

    for(x = 0; x < m_numInputs; ++x)
    {
        //Debug statement
        //print(cout, "\tX = ", x, "\n");
        //print(cout, "\tPrevious sum ", sum, " + ");
        //print(cout, "Input ", m_inputs[x], " * ");
        //print(cout, "Weight ", m_weights[x], " = ");

        sum += m_inputs[x] * m_weights[x];

        //print(cout, "", sum, "\n");
    }

    //Debug statement
    //print(cout, "\tX = ", x, "\n");
    //print(cout, "\tPrevious sum ", sum, " + ");
    //print(cout, "Bias ", m_weights[x], " = ");

    //The bias.
    sum += m_weights[x];

    //print(cout, "", sum, "\n");

    //Debug statement
    //print(cout, "\tOutput of summation ", sum, "\n\n");

    return sum;
}
