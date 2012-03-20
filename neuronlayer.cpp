#include "neuronlayer.h"

void NeuronLayer::init(const int numNeurons, const int numInputs)
{
    m_numNeurons = numNeurons;

    m_neurons = new Neuron[numNeurons];
    m_blames = new double[numNeurons];
    m_accBlames = new double*[numNeurons];
    m_momentumChanges = new double*[numNeurons];
    m_weightChanges = new double[numNeurons];

    for(int x = 0; x < numNeurons; ++x)
    {
        m_neurons[x].init(numInputs);
        //NumInputs + 1 accounts for bias values.
        m_accBlames[x] = new double[numInputs + 1];
        m_momentumChanges[x] = new double[numInputs + 1];

        for(int y = 0; y < numInputs + 1; ++y)
        {
            m_accBlames[x][y] = 0.0;
            m_momentumChanges[x][y] = 0.0;
        }
    }
}

void NeuronLayer::purge()
{
    if(m_neurons != NULL)
    {
        delete [] m_neurons;
        m_neurons = NULL;
    }

    if(m_blames != NULL)
    {
        delete [] m_blames;
        m_blames = NULL;
    }

    if(m_accBlames != NULL)
    {
        for(int x = 0; x < m_numNeurons; ++x)
        {
            delete [] m_accBlames[x];
            m_accBlames[x] = NULL;
        }
        delete [] m_accBlames;
        m_accBlames = NULL;
    }

    if(m_momentumChanges != NULL)
    {
        for(int x = 0; x < m_numNeurons; ++x)
        {
            delete [] m_momentumChanges[x];
            m_momentumChanges[x] = NULL;
        }
        delete [] m_momentumChanges;
        m_momentumChanges = NULL;
    }

    if(m_weightChanges != NULL)
    {
        delete [] m_weightChanges;
        m_weightChanges = NULL;
    }

    if(m_activation != NULL)
    {
        delete m_activation;
        m_activation = NULL;
    }
}

void NeuronLayer::setWeightsForNeuron(int index, const double *weights)
{
#ifdef DEBUG_NEURONLAYER
    printLine2("\tSetting weights for neuron ", index);
#endif

    m_neurons[index].setWeights(weights);
}

int NeuronLayer::generateLayerWeights(double *&weightResults)
{
    //Make the array large enough to hold all weights.
    int arrLength = m_numNeurons * m_neurons[0].getNumWeights();
    weightResults = new double[arrLength];

    double *neuronWeights = new double[m_neurons[0].getNumWeights()];

    for(int x = 0; x < m_numNeurons; ++x)
    {
        for(int y = 0; y < m_neurons[0].getNumWeights(); ++y)
        {
            //Give a weight between -1 and 1.
            neuronWeights[y] = (rand() / ((double)RAND_MAX)) * 2 - 1;
        }

        //Store the weights in the correct neuron.
        setWeightsForNeuron(x, neuronWeights);

        //Copy the weights to the input array.
        copyArray(neuronWeights, weightResults, x * m_neurons[0].getNumWeights(), m_neurons[0].getNumWeights());
    }

    delete [] neuronWeights;

    return arrLength;
}

void NeuronLayer::copyArray(const double *source, double *&dest, int destStartIndex, int sourceNumElements)
{
    for(int x = destStartIndex; x < destStartIndex + sourceNumElements; ++x)
    {
        dest[x] = source[x - destStartIndex];
    }
}

void NeuronLayer::copyArray(const double *source, double *&dest, int numSourceElements)
{
    if(dest != NULL)
        delete [] dest;

    dest = new double[numSourceElements];
    for(int x = 0; x < numSourceElements; ++x)
    {
        dest[x] = source[x];
    }
}

void NeuronLayer::getResult(const double *inputs, double *&outputs)
{

    for(int x = 0; x < m_numNeurons; ++x)
    {
        //Copy inputs to the neuron.
        m_neurons[x].setInputs(inputs);
        //Call the activation function.
        //The activation function will sum the inputs and use them as the input
        //for m_activation.activate()
        outputs[x] = m_neurons[x].activation(m_activation);

#ifdef DEBUG_NNOUTPUTS
        printLine2("\t\tOutput after activation function: ", outputs[x]);
#endif
        //Since outputs is storing the results of the activation function, there is no need
        //to return a value.
    }
#ifdef DEBUG_NNOUTPUTS
    printLine("");
#endif
}

double *NeuronLayer::calcBlames(const double *nextLayerErrors, double **nextLayerWeightMatrix, int numErrors)
{
#ifdef DEBUG_NN_ACCBLAMES
    printLine3("Calculate blames for ", m_numNeurons, " neurons:");
#endif

    for(int x = 0; x < m_numNeurons; ++x)
    {
        m_blames[x] = 0.0;
        for(int y = 0; y < numErrors; ++y)
        {
            //X represents the weight of a neuron from the next layer.
            //Y represents a neuron.  There is one error per neuron.
            m_blames[x] += nextLayerErrors[y] * nextLayerWeightMatrix[y][x];
        }
#ifdef DEBUG_NN_ACCBLAMES
        printLine4("\tNeuron ", x, " blame is ", m_blames[x]);
#endif
    }

#ifdef DEBUG_NN_ACCBLAMES
    print("\n");
#endif
    return m_blames;
}

double *NeuronLayer::calcBlameDeltas()
{
    for(int x = 0; x < m_numNeurons; ++x)
    {
        //Call the derivative function for the neurons.
        //Multiply blames by the results.
        m_blames[x] *= m_neurons[x].derivative(m_activation);
#ifdef DEBUG_NN_ACCBLAMES
        printLine2("\tResult of blame delta: ", m_blames[x]);
#endif
    }

    return m_blames;
}

void NeuronLayer::resetAccBlames()
{
    for(int x = 0; x < m_numNeurons; ++x)
    {
        //Reset accBlames.
        for(int y = 0; y < m_neurons[0].getNumWeights(); ++y)
        {
            m_accBlames[x][y] = 0.0;
        }
    }
}

void NeuronLayer::calcAccBlames()
{
#ifdef DEBUG_NN_ACCBLAMES
    printLine5("\tCalc acc blames for ", m_numNeurons, " neurons with ", m_neurons[0].getNumWeights(), " weights");
#endif
    //Add each weight to the correct box on accBlames.
    for(int x = 0; x < m_numNeurons; ++x)
    {
        for(int y = 0; y < m_neurons[0].getNumInputs(); ++y)
        {
#ifdef DEBUG_NN_ACCBLAMES
            print3("\t\tInput neuron ", y, " output countains ");
            printLine2("value ", m_neurons[0].getInput(y));

            print3("\t\tOutput blame ", x, " countains ");
            printLine2("value ", m_blames[x]);
#endif
            m_accBlames[x][y] += m_blames[x] * m_neurons[0].getInput(y);

#ifdef DEBUG_NN_ACCBLAMES
            printLine3("\t\taccumulated blame = ", m_accBlames[x][y], "\n");
#endif
        }

        //The bias is stored at the index referenced by numInputs.
        m_accBlames[x][m_neurons[0].getNumInputs()] += m_blames[x];

#ifdef DEBUG_NN_ACCBLAMES
        print3("\t\tBias blame = ", m_accBlames[x][m_neurons[0].getNumInputs()], "\n\n");
#endif
    }
}

void NeuronLayer::addMomentum(double **&weightMatrix) const
{
#ifdef DEBUG_NEURONLAYER_MOMENTUM
    print("\tAdding momentum to accBlames:\n");
#endif
    for(int x = 0; x < m_numNeurons; ++x)
    {
#ifdef DEBUG_NEURONLAYER_MOMENTUM
        print3("\t\tx = ", x, ":");
#endif
        //Apply momentum values to the blames.
        for(int y = 0; y < m_neurons[0].getNumWeights(); ++y)
        {
#ifdef DEBUG_NEURONLAYER_MOMENTUM
            print2(" ", m_momentumChanges[x][y] * m_momentum);
#endif
            weightMatrix[x][y] = m_momentumChanges[x][y] * m_momentum;
        }
#ifdef DEBUG_NEURONLAYER_MOMENTUM
        print("\n");
#endif
    }
}

void NeuronLayer::addLearnRate(double **&weightMatrix) const
{
    for(int x = 0; x < m_numNeurons; ++x)
    {
        //Apply momentum values to the blames.
        for(int y = 0; y < m_neurons[0].getNumWeights(); ++y)
        {
            weightMatrix[x][y] = m_accBlames[x][y] * m_learnRate;
        }
    }
}

void NeuronLayer::changeWeights()
{
    //Maybe this will work.
    double **m1 = NULL;
    double **m2 = NULL;
    getEmptyWeightMatrix(m1);
    getEmptyWeightMatrix(m2);

    //Fill each weight matrix with the corresponding values.
    addLearnRate(m1);
    addMomentum(m2);

#ifdef DEBUG_NN_ACCBLAMES
    printLine("Blames applied to this layer:");
#endif
    //Add the matrices together, storing the results back in m_momentumChanges for use in the next iteration.
    for(int x = 0; x < m_numNeurons; ++x)
    {
        for(int y = 0; y < m_neurons[0].getNumWeights(); ++y)
        {
            m_momentumChanges[x][y] = m1[x][y] + m2[x][y];
#ifdef DEBUG_NN_ACCBLAMES
            print2(m_momentumChanges[x][y], " ");
#endif
        }
#ifdef DEBUG_NN_ACCBLAMES
        print("\n");
#endif
    }


    //Pass all weight changes to the correct neurons.
    for(int x = 0; x < m_numNeurons; ++x)
    {
        m_neurons[x].changeWeights(m_accBlames[x]);
    }

    //Set Accumulated blames to zero.
    resetAccBlames();

    //Manage memory.
    // For optimization:  Maybe it would be better to keep m1 and m2 around for the life of the network.
    //This would keep me from having to create and destroy them every time a training iteration ends.
    //Plus, it would only cost a number of bytes equal to numNeurons * numWeights * 8 + numNeurons * 4 + 4.
    //So, for a layer with 9 inputs and 27 neurons, the cost is just over 2KB.  Memory is cheap.  Clock cycles
    //are expensive.
    destroyWeightMatrix(m1);
    destroyWeightMatrix(m2);
}

double *OutputLayer::calcOutputBlames(const double *expectedOutput)
{
#ifdef DEBUG_NN_ACCBLAMES
    print3("Calculate blames for ", m_numNeurons, " output neurons:");
#endif

    for(int x = 0; x < getNumNeurons(); ++x)
    {
        m_blames[x] = expectedOutput[x] - m_neurons[x].getOutput();
#ifdef DEBUG_NN_ACCBLAMES
        print2(" ", m_blames[x]);
#endif
    }

#ifdef DEBUG_NN_ACCBLAMES
    print("\n");
#endif
    return m_blames;
}

double *OutputLayer::calcOutputBlames(const double *actual, const double *expectedOutput)
{
#ifdef DEBUG_NN_ACCBLAMES
    print3("Calculate blames for ", m_numNeurons, " output neurons:");
#endif

    for(int x = 0; x < getNumNeurons(); ++x)
    {
        m_blames[x] = expectedOutput[x] - actual[x];
#ifdef DEBUG_NN_ACCBLAMES
        print2(" ", m_blames[x]);
#endif
    }

#ifdef DEBUG_NN_ACCBLAMES
    print("\n");
#endif
    return m_blames;
}

void NeuronLayer::getEmptyWeightMatrix(double **&weightMatrix) const
{
    //Create an array to contain individual neurons' arrays.
    weightMatrix = new double*[m_numNeurons];
    for(int x = 0; x < m_numNeurons; ++x)
    {
        //Create one sub-array for each neuron.  All neurons have the same number of weights.
        weightMatrix[x] = new double[m_neurons[0].getNumWeights()];
    }
}

//Copy all neuron weights into a matrix.
//Although this could call getEmptyWeightMatrix first, this function is meant to be used in
//A neural network's backpropagation algorithm.  Therefore, it must be fast.  Calling another
//function will just slow it down.
void NeuronLayer::getWeightMatrix(double **&weightMatrix) const
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

void NeuronLayer::destroyWeightMatrix(double **&weightMatrix) const
{
    //Delete each array.
    for(int x = 0; x < m_numNeurons; ++x)
    {
        delete [] weightMatrix[x];
    }

    //Delete the initial array.
    delete weightMatrix;
    weightMatrix = NULL;
}
