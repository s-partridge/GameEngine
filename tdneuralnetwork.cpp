#include "tdneuralnetwork.h"

void TDNeuralNetwork::tdBackpropagate(double *actual, double *expected, double *prevSummation)
{
    double *tempBlames;

#ifdef DEBUG_NEURALNETWORK
    printLine("\nBackpropagate");
    printLine("\nCalculate output blames");
#endif
    //Store each layer's blames in tempBlames to be passed on to the next successive layer.
    tempBlames = m_outputLayer.calcTDOutputBlames(actual, expected);

#ifdef DEBUG_NEURALNETWORK
    printLine("Calculate output blame deltas");
#endif
    m_outputLayer.calcTDOutputBlameDeltas(prevSummation);

    //TODO:
    //Refactor the output layer.  Stupid idea to make it separate.
    //All this extra crap needs to be done just because it's not part of the layer array.
    double **weightMatrix = NULL;

#ifdef DEBUG_NEURALNETWORK
    printLine("Create a weight matrix for output layer");
#endif

    //The weight matrix is necessary for calculating blames on each successive layer.
    //It stores each weight individually withough accumulating them.
    m_outputLayer.getWeightMatrix(weightMatrix);

#ifdef DEBUG_NEURALNETWORK
    printLine("Calc last hidden layer blames");
#endif

    //Calculate blames for the first hidden layer.
    tempBlames = m_hiddenLayers[m_numHiddenLayers - 1].calcBlames(tempBlames, weightMatrix, m_outputLayer.getNumNeurons());


#ifdef DEBUG_NEURALNETWORK
    printLine("Calc last hidden layer blame deltas");
#endif

    //Calculate the blame deltas for the firs hidden layer.  Blame deltas are the actual values used to
    //change the weights of a neuron layer.
    m_hiddenLayers[m_numHiddenLayers - 1].calcBlameDeltas();

    //Delete the reference to the weight matrix.
    m_outputLayer.destroyWeightMatrix(weightMatrix);

    for(int x = m_numHiddenLayers - 2; x >= 0; --x)
    {
        //Get the weight matrix from the next layer.
        m_hiddenLayers[x + 1].getWeightMatrix(weightMatrix);

#ifdef DEBUG_NEURALNETWORK
        printLine3("Calc hidden layer ", x, "blames");
#endif

        tempBlames = m_hiddenLayers[x].calcBlames(tempBlames, weightMatrix, m_hiddenLayers[x + 1].getNumNeurons());

#ifdef DEBUG_NEURALNETWORK
        printLine3("Calc hidden layer ", x, "blame deltas");
#endif

        m_hiddenLayers[x].calcBlameDeltas();
        //Destroy the reference to the weight matrix from the next layer.
        m_hiddenLayers[x + 1].destroyWeightMatrix(weightMatrix);
    }

    //output layer
    //These steps must be performed in the correct order for each
    //layer in the network.  If they are not done right, the network
    //will learn incorrectly.

    //TODO:
    //Create a function in NeuronLayer to consolidate all these methods.

#ifdef DEBUG_NEURALNETWORK
    printLine("Calc accumulated output blames");
#endif

    m_outputLayer.calcAccBlames();

#ifdef DEBUG_NEURALNETWORK
    printLine("Adding output momentum");
#endif
    m_outputLayer.addMomentum();
    m_outputLayer.addLearnRateToBlames();

    for(int x = m_numHiddenLayers - 1; x >= 0; --x)
    {
#ifdef DEBUG_NEURALNETWORK
    printLine3("Calc hidden layer ", x, " accumulated blames");
#endif
        m_hiddenLayers[x].calcAccBlames();

#ifdef DEBUG_NEURALNETWORK
    printLine3("Adding hidden layer ", x, " momentum");
#endif
        m_hiddenLayers[x].addMomentum();

#ifdef DEBUG_NEURALNETWORK
    printLine3("Adding hidden layer ", x, " learning rate");
#endif
        m_hiddenLayers[x].addLearnRateToBlames();
    }
}
