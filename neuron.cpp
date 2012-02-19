#include "neuron.h"

int Neuron::init(const int numWeights)
{
#ifdef DEBUG_NEURON
    printLine3("Initializing neuron with ", numWeights, " inputs");
#endif
    //Make sure the array is empty.
    if(m_weights == NULL)
    {
        //Allocate space for the weights and the bias.
        m_numWeights = numWeights + 1;
        m_weights = new double[m_numWeights];

        //Debug statement
        //print("\t\tThere are ", m_numWeights, " weights in this neuron\n");
    }
    //The array already exists.
    else
        return 1;

    if(m_inputs == NULL)
    {
        //There should be one weight per input.
        //There is no input for the bias.
        m_numInputs = numWeights;
        m_inputs = new double[m_numWeights];

        //Debug statement
        //print("\t\tThere are ", m_numInputs, " inputs to this neuron\n");
    }
    //The array already exists.
    else
        return 2;

    return 0;
}

void Neuron::purge()
{
    if(m_inputs != NULL)
    {
        delete [] m_inputs;
        m_inputs = NULL;
    }

    if(m_weights != NULL)
    {
        delete [] m_weights;
        m_weights = NULL;
    }
}

//Set all weights at once.
void Neuron::setWeights(const double *weights)
{
#ifdef DEBUG_NEURON
    print("\t\tSetting neuron weights: ");
#endif
    for(int x = 0; x < m_numWeights; ++x)
    {
        m_weights[x] = weights[x];

#ifdef DEBUG_NEURON
        print2(m_weights[x], " ");
#endif
    }

#ifdef DEBUG_NEURON
    print("\n");
#endif
}

//Add changes to each weight.
void Neuron::changeWeights(const double *changes)
{
#ifdef DEBUG_NEURON
    print("\t\tChanging neuron weights by: ");
#endif
    for(int x = 0; x < m_numWeights; ++x)
    {
#ifdef DEBUG_NEURON
        print2(changes[x], " ");
#endif
        m_weights[x] += changes[x];
    }
#ifdef DEBUG_NEURON
    print("\n");
#endif
}

//Set all inputs at once.
void Neuron::setInputs(const double *inputs)
{
#ifdef DEBUG_NEURON
//    print("\tSetting neuron inputs: ");
#endif
    for(int x = 0; x < m_numInputs; ++x)
    {
#ifdef DEBUG_NEURON
//    print2(inputs[x], " ");
#endif
        m_inputs[x] = inputs[x];
    }
#ifdef DEBUG_NEURON
//    printLine("");
#endif
}

//Add up all weights, multiplied by inputs, and return the total.
double Neuron::summation()
{
    double sum = 0.0;

    int x;


    for(x = 0; x < m_numInputs; ++x)
    {
#ifdef DEBUG_NEURON
        print3("\n\tX = ", x, "; ");
        print3("\tPrevious sum ", sum, " + ");
        print3("Input ", m_inputs[x], " * ");
        print3("Weight ", m_weights[x], " = ");
#endif
        sum += m_inputs[x] * m_weights[x];

        //print(cout, "", sum, "\n");
    }

#ifdef DEBUG_NEURON
    print3("\n\tX = ", x, "; ");
    print3("\tPrevious sum ", sum, " + ");
    print3("Bias ", m_weights[x], " = ");
#endif
    //The bias.
    sum += m_weights[x];

#ifdef DEBUG_NEURON
    printLine2("\tOutput of summation ", sum);
#endif
    return sum;
}

double Neuron::activation(const ActivationFunctor *activate)
{
    m_output = activate->activation(summation());
    return m_output;
}

double Neuron::derivative(const ActivationFunctor *derivate)
{
    double temp = derivate->simplifiedDerivative(m_output);

#ifdef DEBUG_NEURON
        printLine2("\tResult of derivative: ", temp);
#endif
    return temp;
}
