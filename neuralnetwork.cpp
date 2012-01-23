#include "neuralnetwork.h"
#include "datatypes.h"

#include <fstream>
#include <fstream>
using namespace std;

NeuralNetwork::NeuralNetwork(string filename)
{
    //skip the first byte.
    fstream saveFile(filename.c_str(), fstream::in | fstream::binary);

    savefile.seekg(5);

    //num inputs
    saveFile.read((char *)&m_numInputs, 4);

    //num hidden layers
    saveFile.read((char *)&m_numHiddenLayers, 4);

    //num output neurons
    saveFile.read((char *)&m_numOutputs, 4);

    //num hidden neurons 1
    //num hidden neurons 2 ...
    m_hiddenLayers = new NeuronLayer[m_numHiddenLayers];

    int temp;
    saveFile.read((char *)&temp, 4);
    m_hiddenLayers[0].init(temp, m_numInputs);

    for(int x = 1; x < m_numHiddenLayers; ++x)
    {
        saveFile.read((char *)&temp, 4);
        m_hiddenLayers[x].init(temp, m_hiddenLayers[x - 1].getNumNeurons());
    }

    m_outputLayer.init(m_numOutputs, m_hiddenLayers[m_numHiddenLayer - 1]);

    //momentum 1
    //momentum 2
    //momentum 3 ...

    double temp2;
    for(int x = 0; x < m_numHiddenLayers; ++x)
    {
        saveFile.read((char *)& temp2, 8);
        m_hiddenLayers[x].setMomentum(temp2);
    }

    saveFile.read((char *)& temp2, 8);
    m_hiddenLayers[x].setMomentum(temp2);

    //learnrate 1
    //learnrate 2
    //learnrate 3 ...

    for(int x = 0; x < m_numHiddenLayers; ++x)
    {
        saveFile.read((char *)& temp2, 8);
        m_hiddenLayers[x].setLearnRate(temp2);
    }

    saveFile.read((char *)& temp2, 8);
    m_hiddenLayers[x].setLearnRate(temp2);

    //hidden weights
    for(int x = 0; x < m_numHiddenLayers; ++x)
    {
        for(int y = 0; y < m_hiddenLayers[x].getNumNeurons(); ++y)
        {
            double *weights = new double[m_hiddenLayers[x].getNumInputs() + 1];
            for(int z = 0; z <= m_hiddenLayers[x].getNumInputs(); ++z)
            {
                saveFile.read((char *)& weights[z], 8);
            }
            m_hiddenLayers[x].setWeightsForNeuron(y, weights);
            delete [] weights;
        }
    }

    //output weights
    for(int y = 0; y < m_outputLayer.getNumNeurons(); ++y)
    {
        double *weights = new double[m_outputLayer.getNumInputs() + 1];
        for(int z = 0; z <= m_outputLayer[x].getNumInputs(); ++z)
        {
            saveFile.read((char *)& weights[z], 8);
        }
        m_hiddenLayers[x].setWeightsForNeuron(y, weights);
        delete [] weights;
    }

}

NeuralNetwork::NeuralNetwork(string filename, const int numInputs, const int numOutputNeurons, const int numHiddenLayers, const int *numHiddenNeurons, const double *momentum, const double *learnRate)
    : m_numInputs(numInputs), m_numHiddenLayers(numHiddenLayers), m_numOutputs(numOutputNeurons), m_filename(filename)
{
    //Version number
    char fileVersion = NEURAL_NET_VERSION;
    //Hidden location
    int hiddenLocation = sizeof(char) + sizeof(int) * 3 + sizeof(int) * numHiddenLayers + (sizeof(double) * numHiddenLayers + 1) + (sizeof(double) * numHiddenLayers + 1);
    //num inputs
    //num hidden layers
    //num output neurons
    //num hidden neurons 1
    //num hidden neurons 2 ...
    //momentum 1
    //momentum 2
    //momentum 3 ...
    //learnrate 1
    //learnrate 2
    //learnrate 3 ...
    //hidden weights
    //output weights

    fstream saveFile(filename.c_str(), fstream::out | fstream::binary | fstream::trunc);

    saveFile.write((char *)&fileVersion, 1);
    saveFile.write((char *)&hiddenOffset, 4);

    saveFile.write((char *)&numInputs, 4);
    saveFile.write((char *)&numHiddenLayers, 4);
    saveFile.write((char *)&numOutputNeurons, 4);

    m_hiddenLayers = new NeuronLayer[numHiddenLayers];

    for(int x = 0; x < numHiddenLayers; ++x)
    {
        saveFile.write((char *)&numHiddenNeurons[x], 4);
    }

    m_hiddenLayers[0].init(numHiddenNeurons, numinputs);

    for(int x = 1; x < numHiddenLayers; ++x)
    {
        m_hiddenLayers[x].init(numHiddenNeurons[x], m_hiddenLayers[x - 1].getNumNeurons());
    }

    m_outputLayer.init(numOutputNeurons, m_hiddenLayers[numHiddenLayers - 1].getNumNeurons());

    for(int x = 0; x <= numHiddenLayers; ++x)
        saveFile.write((char *)&momentum[x], 8);

    for(int x = 0; x < numHiddenLayers; ++x)
    {
        m_hiddenLayers[x].setMomentum(momentum[x]);
    }

    m_outputLayer.setMomentum(momentum[numHiddenLayers]);

    for(int x = 0; x <= numHiddenLayers; ++x)
        saveFile.write((char *)&learnRate[x], 8);

    for(int x = 0; x < numHiddenLayers; ++x)
    {
        m_hiddenLayers[x].setLearnRate(learnRate[x]);
    }

    m_outputLayer.setLearnRate(learnRate[numHiddenLayers]);

    for(int x = 0; x < m_numHiddenlayers; ++x)
    {
        double *weights;
        m_hiddenLayers[x].generateLayerWeights(weights);

        for(int x = 0; x < m_hiddenLayers[x].getNumInputs() * m_hiddenLayers[x].getNumNeurons(); ++x)
        {
            saveFile.write((char *)&weights[x], 8);
        }

        delete [] weights;
    }

    double *weights;
    m_outputLayer.generateLayerWeights(weights);

    for(int x = 0; x < (m_hiddenLayers[x].getNumInputs() + 1) * m_hiddenLayers[x].getNumNeurons(); ++x)
    {
        saveFile.write((char *)&weights[x], 8);
    }

    delete [] weights;
}

void NeuralNetwork::setLayerMomentum(int layer, double momentum)
{
    if(layer < m_numLayers - 1)
    {
        m_hiddenLayers[layer].setMomentum(momentum);
    }
    else if(layer == m_numLayers - 1)
    {
        m_outputLayer.setMomentum(momentum);
    }
}

void NeuralNetwork::setLayerActivation(int layer, ActivationFunctor *activation)
{
    if(layer < m_numLayers - 1)
    {
        m_hiddenLayers[layer].setActivation(activation);
    }
    else if(layer == m_numLayers - 1)
    {
        m_outputLayer.setActivation(activation);
    }
}

void NeuralNetwork::setLayerLearnRate(int layer, double learnRate)
{
    if(layer < m_numHiddenLayers)
    {
        m_hiddenLayers[layer].setLearnRate(learnRate);
    }
    else if(layer == m_numHiddenLayers)
    {
        m_outputLayer.setLearnRate(learnRate);
    }
}

void NeuralNetwork::saveNNP()
{

}

void NeuralNetwork::saveNNP(string filename)
{

}

void NeuralNetwork::getResults(const double *inputs, double *outputs)
{
    if(outputs != NULL)
        delete [] outputs;

    double *tempInputs = new double[m_numinputs];

    arrayCopy(inputs, tempInputs);

    //Iterate over each hidden layer, passing the outputs one one layer as the inputs of the next.
    for(int x = 0; x < m_numHiddenLayers; ++x)
    {
        outputs = new double[m_hiddenLayers[0].getNumNeurons()];
        m_hiddenLayers[x].getResult(tempInputs, outputs);

        delete tempInputs;
        tempInputs = NULL;

        arrayCopy(outputs, tempInputs);

        delete [] outputs;
    }

    outputs = new double[m_numOutputs];
    m_outputLayer.getResult(tempInputs, outputs);

    delete tempInputs;
}

void NeuralNetwork::backpropagate(double *actual, double *expected)
{
    double *tempBlames;

    tempBlames = m_outputLayer.calcOutputBlames(expected);
    m_outputLayer.calcBlameDeltas();

    for(int x = m_numHiddenLayers - 1; x >= 0; --x)
    {
        tempBlames = m_hiddenLayers[x].calcBlames(tempBlames, m_hiddenLayers[x].getNumNeurons());
        m_hiddenLayers[x].calcBlameDeltas();
    }

    //output layer
    m_outputLayer.calcAccBlames();
    m_outputLayer.addMomentum();
    m_outputLayer.changeWeights();
    m_outputLayer.setMomentumChanges();

    for(int x = m_numHiddenLayers - 1; x >= 0; --x)
    {
        m_hiddenLayers[x].calcAccBlames();
        m_hiddenLayers[x].addMomentum();
        m_hiddenLayers[x].changeWeights();
        m_hiddenLayers[x].setMomentumChanges();
    }
}

void NeuralNetwork::arrayCopy(const double *input, double *output, int length)
{
    if(output != NULL)
        delete [] output;

    output = new double[length];
    for(int x = 0; x < length; ++x)
    {
        output[x] = input[x];
    }
}
