#include "neuronlayer.h"

void NeuronLayer::init(const int numNeurons, const int numInputs)
{
    m_numNeurons = numNeurons;

    m_neurons = new Neuron[numNeurons];

    for(int x = 0; x < numNeurons; ++x)
    {
        m_neurons[x].init(numInputs);
    }
}

void NeuronLayer::generateLayerWeights(double *weightResults)
{
    //Make the array large enough to hold all weights.
    weightResults = new double[m_numNeurons * m_neurons[0].getNumWeights()];

    srand(time(NULL));

    double *neuronWeights = new double[m_neurons[0].getNumWeights()];

    for(int x = 0; x < m_numneurons; ++x)
    {
        for(int y = 0; y < m_neurons[0].getNumWeights(); ++y)
        {
            //Give a weight between -1 and 1.
            neuronWeights[y] = rand() / ((double)RAND_MAX * 2) - 1;
        }

        //Store the weights in the correct neuron.
        setWeightsForNeuron(x, neuronWeights);

        //Copy the weights to the input array.
        arrayCopy(neuronWeights, weightResults, x * m_neurons[0].getNumWeights(), m_neurons[0].getNumWeights());
    }
}

void NeuronLayer::arrayCopy(const double *source, double *dest, int destStartIndex, int sourceNumElements)
{
    for(int x = startIndex; x < destStartIndex + sourceNumElements; ++x)
    {
        dest[x] = source[x - startIndex];
    }
}

void NeuronLayer::getResult(const double *inputs, double *outputs)
{
    for(int x = 0; x < m_numNeurons; ++x)
    {
        //Copy inputs to the neuron.
        m_neurons[x].setInputs(inputs);
        //Call the activation function.
        //The activation function will sum the inputs and use them as the input
        //for m_activation.activate()
        outputs[x] = m_neurons[x].activation(m_activation);

        //Since outputs is storing the results of the activation function, there is no need
        //to return a value.
    }
}

double *NeuronLayer::calcBlames(const double *nextLayerErrors, const double **nextLayerWeightMatrix, int numErrors)
{
    for(int x = 0; x < m_numNeurons; ++x)
    {
        blames[x] = 0.0;
        for(int y = 0; y < numErrors; ++y)
        {
            //X represents the weight of a neuron from the next layer.
            //Y represents a neuron.  There is one error per neuron.
            blames[x] += nextLayerErrors[y] * nextLayerWeightMatrix[y][x];
        }
    }

    return blames;
}

double *NeuronLayer::calcBlameDeltas()
{
    for(int x = 0; x < numNeurons; ++x)
    {
        //Call the derivative function for the neurons.
        //Multiply blames by the results.
        blames[x] *= m_neurons[x].derivative(m_activation);
    }
}

void NeuronLayer::resetAccBlames()
{
    for(int x = 0; x < m_numNeurons; ++x)
    {
        //Reset accBlames.
        for(int y= 0; y < m_neurons[0].getNumWeights(); ++y)
        {
            m_accBlames[x][y] = 0.0;
        }
    }
}

void NeuronLayer::calcAccBlames()
{
    resetAccBlames();

    //Add each weight to the correct box on accBlames.
    for(int x = 0; x < m_numNeurons; ++x)
    {
        for(int y = 0; y < m_neurons[0].getNumInputs(); ++y)
        {
            m_accBlames[x][y] += blames[x] * m_neurons[0].getInput(y);
        }

        //The bias is stored at the index referenced by numInputs.
        m_accBlames[x][m_neurons[0].getNumInputs()] += blames[x];
    }
}

void NeuronLayer::addMomentum()
{
    for(int x = 0; x < m_numNeurons; ++x)
    {
        //Apply momentum values to the blames.
        for(int y = 0; y < m_neurons[0].getNumWeights(); ++y)
        {
            m_accBlames[x][y] += m_momentumChanges[x][y] * m_momentum;
        }
    }
}

void NeuronLayer::setMomentumChanges()
{
    for(int x = 0; x < m_numNeurons; ++x)
    {
        //Apply blame values to the momentums.
        for(int y = 0; y < m_neurons[0].getNumWeights(); ++y)
        {
            m_momentumChanges = m_accBlames[x][y];
        }
    }
}

void NeuronLayer::addLearnRate()
{
    for(int x = 0; x < m_numNeurons; ++x)
    {
        //Apply momentum values to the blames.
        for(int y = 0; y < m_neurons[0].getNumWeights(); ++y)
        {
            m_accBlames[x][y] *= m_learnRate;
        }
    }
}

void NeuronLayer::changeWeights()
{
    //Pass all weight changes to the correct neurons.
    for(int x = 0; x < m_numNeurons; ++x)
    {
        m_neurons[x].changeWeights(m_accBlames[x]);
    }
}

double *OutputLayer::calcOutputBlames(const double *expectedOutput)
{
    for(int x = 0; x < getNumNeurons(); ++x)
    {
        blames[x] = expectedOutput[x] - m_neurons[x].getOutput();
    }

    return blames;
}

//Copy all neuron weights into a matrix.
void NeuronLayer::getWeightMatrix(double **weightMatrix)
{
    weightMatrix = new double*[m_numNeurons];

    for(int x = 0; x < m_numNeurons; ++x)
    {
        weightMatrix[x] = new double[m_neurons[0].getNumWeights()];

        for(int y = 0; y < m_neurons[0].getNumWeights(); ++y)
        {
            weightMatrix[x][y] = m_neurons[x].getWeight(y);
        }
    }
}
