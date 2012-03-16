#include "neuralnetwork.h"
#include "datatypes.h"

#include <fstream>
#include <fstream>
using namespace std;

NeuralNetwork::NeuralNetwork(string filename) : m_filename(filename)
{
    //Create a functor instance for the activation function object.
    m_activation = new Sigmoid();

    //skip the first byte.
    fstream saveFile(filename.c_str(), fstream::in | fstream::binary);

    saveFile.seekg(5);

    //num inputs
    saveFile.read((char *)&m_numInputs, 4);

    //num hidden layers
    saveFile.read((char *)&m_numHiddenLayers, 4);

    //num output neurons
    saveFile.read((char *)&m_numOutputs, 4);

    //num hidden neurons 1
    //num hidden neurons 2 ...
    m_hiddenLayers = new NeuronLayer[m_numHiddenLayers];

    int x;
    int temp;
    saveFile.read((char *)&temp, 4);
    m_hiddenLayers[0].init(temp, m_numInputs);

    for(x = 1; x < m_numHiddenLayers; ++x)
    {
        saveFile.read((char *)&temp, 4);
        m_hiddenLayers[x].init(temp, m_hiddenLayers[x - 1].getNumNeurons());
    }

    m_outputLayer.init(m_numOutputs, m_hiddenLayers[m_numHiddenLayers - 1].getNumNeurons());

    //momentum 1
    //momentum 2
    //momentum 3 ...

    double temp2;
    for(x = 0; x <= m_numHiddenLayers; ++x)
    {
        //SetLayerMomentum will cover hidden and output layers.
        saveFile.read((char *)& temp2, 8);
        setLayerMomentum(x, temp2);
    }

    //learnrate 1
    //learnrate 2
    //learnrate 3 ...

    for(x = 0; x < m_numHiddenLayers; ++x)
    {
        saveFile.read((char *)& temp2, 8);
        setLayerLearnRate(x, temp2);
    }

    //Activation function
    for(x = 0; x <= m_numHiddenLayers; ++x)
    {
        setLayerActivation(x, m_activation);
    }

    //hidden weights
    for(x = 0; x < m_numHiddenLayers; ++x)
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
        for(int z = 0; z <= m_outputLayer.getNumInputs(); ++z)
        {
            saveFile.read((char *)& weights[z], 8);
        }
        m_hiddenLayers[x].setWeightsForNeuron(y, weights);
        delete [] weights;
    }

}

NeuralNetwork::NeuralNetwork(string filename, const int numInputs, const int numOutputNeurons, const int numHiddenLayers, const int *numHiddenNeurons, const double *momentum, const double *learnRate)
    : m_numHiddenLayers(numHiddenLayers), m_numInputs(numInputs), m_numOutputs(numOutputNeurons), m_filename(filename)
{
    //Create a functor instance for the activation function object.
    //TODO: Add this as a parameter so that it can be defined externally.
    m_activation = new Sigmoid();

    //Seed rand here before it is used.
    srand(time(NULL));

    //Version number
    char fileVersion = NEURAL_NET_VERSION;
    //Hidden location
    int hiddenOffset = sizeof(char) + sizeof(int) * 3 + sizeof(int) * numHiddenLayers + (sizeof(double) * numHiddenLayers + 1) + (sizeof(double) * numHiddenLayers + 1);
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

    //The numbers of hidden neurons are stored in a dynamic array.
    //The first hidden layer and the output layer use different values than the others,
    //so they can't be handled in the for loop.
    m_hiddenLayers[0].init(numHiddenNeurons[0], m_numInputs);

    for(int x = 1; x < numHiddenLayers; ++x)
    {
        m_hiddenLayers[x].init(numHiddenNeurons[x], m_hiddenLayers[x - 1].getNumNeurons());
    }

    m_outputLayer.init(numOutputNeurons, m_hiddenLayers[numHiddenLayers - 1].getNumNeurons());

    for(int x = 0; x <= numHiddenLayers; ++x)
    {
        saveFile.write((char *)&momentum[x], 8);
        setLayerMomentum(x, momentum[x]);
    }

    for(int x = 0; x <= numHiddenLayers; ++x)
    {
        saveFile.write((char *)&learnRate[x], 8);
        setLayerLearnRate(x, learnRate[x]);
    }

#ifdef DEBUG_SAVENNP
    printLine("Creating neural net weights");
#endif

    int arrLength;
    //Generate weights for hidden layers.
    for(int x = 0; x < m_numHiddenLayers; ++x)
    {
#ifdef DEBUG_SAVENNP
        print3("Layer ", x, ": ");
#endif
        double *weights;
        //Generate weights for a complete layer, storing the results in wegihts.
        arrLength = m_hiddenLayers[x].generateLayerWeights(weights);

        //Account for each weight.  number of neurons * number of inputs.
        for(int y = 0; y < arrLength; ++y)
        {
#ifdef DEBUG_SAVENNP
            print2(weights[y], " ");
#endif
            saveFile.write((char *)&weights[y], 8);
        }

        delete [] weights;
#ifdef DEBUG_SAVENNP
        print("\n");
#endif
    }

#ifdef DEBUG_SAVENNP
        print("Output layer: ");
#endif
    double *weights;
    arrLength = m_outputLayer.generateLayerWeights(weights);

    for(int x = 0; x < arrLength; ++x)
    {
#ifdef DEBUG_SAVENNP
            print2(weights[x], " ");
#endif
        saveFile.write((char *)&weights[x], 8);
    }

    delete [] weights;

#ifdef DEBUG_SAVENNP
        print("\n");
#endif
}

void NeuralNetwork::purge()
{
    saveNNP();

    if(m_hiddenLayers != NULL)
    {
        delete [] m_hiddenLayers;
        m_hiddenLayers = NULL;
    }
}

void NeuralNetwork::setLayerWeights(int layer, double **weights)
{
#ifdef DEBUG_NEURALNETWORK
    printLine2("Passing new values to neuron layer ", layer);
#endif
    if(layer < m_numHiddenLayers)
    {
        for(int x = 0; x < m_hiddenLayers[layer].getNumNeurons(); ++x)
        {
            m_hiddenLayers[layer].setWeightsForNeuron(x, weights[x]);
        }
    }
    else if(layer == m_numHiddenLayers)
    {
        for(int x = 0; x < m_outputLayer.getNumNeurons(); ++x)
        {
            m_outputLayer.setWeightsForNeuron(x, weights[x]);
        }
    }
}

void NeuralNetwork::setLayerMomentum(int layer, double momentum)
{
    //Check for a hidden layer index.
    if(layer < m_numHiddenLayers)
    {
        m_hiddenLayers[layer].setMomentum(momentum);
    }
    //Check for output layer index.
    else if(layer == m_numHiddenLayers)
    {
        m_outputLayer.setMomentum(momentum);
    }
}

void NeuralNetwork::setLayerActivation(int layer, ActivationFunctor *activation)
{
    //Check for hidden layer index.
    if(layer < m_numHiddenLayers)
    {
        m_hiddenLayers[layer].setActivation(activation);
    }
    //Check for output layer index.
    else if(layer == m_numHiddenLayers)
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

void NeuralNetwork::saveNNP(string filename)
{
    //Change the file to which the neural network will be stored.
    m_filename = filename;

    saveNNP();
}

void NeuralNetwork::saveNNP()
{
#ifdef DEBUG_SAVENNP
    printLine3("Save neural network to file '" , m_filename, "'");
#endif
    int x, y, z;

    //The contents of the file will be purged and re-copied.
    //Open the file for truncation.
    fstream saveFile(m_filename.c_str(), fstream::out | fstream::binary | fstream::trunc);

    //Save the neural net version first.
    char version = NEURAL_NET_VERSION;

    //The byte at which the first hidden weight is stored.
    int hiddenOffset = sizeof(char) + sizeof(int) * 3 + sizeof(int) * m_numHiddenLayers + (sizeof(double) * m_numHiddenLayers + 1) + (sizeof(double) * m_numHiddenLayers + 1);

#ifdef DEBUG_SAVENNP
    printLine2("\tHidden offset at byte " , hiddenOffset);
#endif

    double ***weights;
    double *momentums;
    double *learnRates;
    int *numNeurons;
    int *numInputs;

#ifdef DEBUG_SAVENNP
    print("\tPulling data from each neuron layer\n");
#endif

    //Copies weights, momentum, and learning rate from each layer
    //into the local arrays defined here.
    getLayerValues(weights, momentums, learnRates, numNeurons, numInputs);

    //Write everything to the file

#ifdef DEBUG_SAVENNP
    print("\tWriting metadata to file\n");
#endif

    //The network version.
    saveFile.write((char *)&version, BYTE_SIZE);
    //The offset to the first weight.
    saveFile.write((char *)&hiddenOffset, INT_SIZE);
    //NumInputs
    saveFile.write((char *)&m_numInputs, INT_SIZE);
    //NumHiddenLayers
    saveFile.write((char *)&m_numHiddenLayers, INT_SIZE);
    //NumOutputs
    saveFile.write((char *)&m_numOutputs, INT_SIZE);
    //NumHiddenNeurons
    for(x = 0; x < m_numHiddenLayers; ++x)
    {
        saveFile.write((char *)&numNeurons[x], INT_SIZE);
    }

    //Momentums
    for(x = 0; x <= m_numHiddenLayers; ++x)
    {
        saveFile.write((char *)&momentums[x], INT_SIZE);
    }

    //Learning rates
    for(x = 0; x <= m_numHiddenLayers; ++x)
    {
        saveFile.write((char *)&learnRates[x], DOUBLE_SIZE);
    }

#ifdef DEBUG_SAVENNP
    printLine("\tWriting neuron weights to file");
#endif

    //Hidden and output weights
    //Cover each layer.
    for(x = 0; x <= m_numHiddenLayers; ++x)
    {
        //Cover each neuron in each layer.
        for(y = 0; y < numNeurons[x]; ++y)
        {
            //Cover each input to each neuron.
            for(z = 0; z < numInputs[x]; ++z)
            {
                saveFile.write((char *)&weights[x][y][z], DOUBLE_SIZE);
            }
        }
    }

    //Used to properly dispose of local memory.
    destroyLayerValues(weights, momentums, learnRates, numNeurons, numInputs);
}

void NeuralNetwork::setNeuronLayerWeights(int layerNumber, double **weights)
{
    if(layerNumber < m_numHiddenLayers)
    {
        for(int x = 0; x < m_hiddenLayers[layerNumber].getNumNeurons(); ++x)
        {
            m_hiddenLayers[layerNumber].setWeightsForNeuron(x, weights[x]);
        }
    }
    else if(layerNumber == m_numHiddenLayers)
    {
        for(int x = 0; x < m_outputLayer.getNumNeurons(); ++x)
        {
            m_outputLayer.setWeightsForNeuron(x, weights[x]);
        }
    }
}

void NeuralNetwork::getLayerValues(double ***&weights, double *&momentums, double *&learnRates, int *&numNeurons, int *&numInputs) const
{
    weights = new double**[m_numHiddenLayers + 1];
    momentums = new double[m_numHiddenLayers + 1];
    learnRates = new double[m_numHiddenLayers + 1];
    numNeurons = new int[m_numHiddenLayers + 1];
    numInputs = new int[m_numHiddenLayers + 1];

#ifdef DEBUG_SAVENNP
    printLine("Layer #\tMomentum\tLearning Rate\tNumber of Neurons\tNumber of Inputs");
#endif

    int x;
    for(x = 0; x < m_numHiddenLayers; ++x)
    {
        //GetWeightMatrix will finish creating the weights array for each layer.
        m_hiddenLayers[x].getWeightMatrix(weights[x]);

        //Get the momentum, number of neurons, and learn rate from each layer.
        momentums[x] = m_hiddenLayers[x].getMomentum();
        learnRates[x] = m_hiddenLayers[x].getLearnRate();
        numNeurons[x] = m_hiddenLayers[x].getNumNeurons();
        numInputs[x] = m_hiddenLayers[x].getNumInputs();

#ifdef DEBUG_SAVENNP
        print2(x, "\t");
        print4(momentums[x], "\t", learnRates[x], "\t\t");
        printLine3(numNeurons[x], "\t\t", numInputs[x]);
#endif
    }

    //The output layer is separate, so these values must be added separately.
    m_outputLayer.getWeightMatrix(weights[m_numHiddenLayers]);
    momentums[m_numHiddenLayers] = m_outputLayer.getMomentum();
    learnRates[m_numHiddenLayers] = m_outputLayer.getLearnRate();
    numNeurons[m_numHiddenLayers] = m_outputLayer.getNumNeurons();
    numInputs[m_numHiddenLayers] = m_outputLayer.getNumInputs();

#ifdef DEBUG_SAVENNP
        print2(x, "\t");
        print4(momentums[x], "\t", learnRates[x], "\t\t");
        printLine3(numNeurons[x], "\t\t", numInputs[x]);
#endif
}

void NeuralNetwork::destroyLayerValues(double ***&weights, double *&momentums, double *&learnRates, int *&numNeurons, int *&numInputs) const
{
    //Delete the weights array.
    //Account for each layer.
    for(int x = 0; x < m_numHiddenLayers; ++x)
    {
        m_hiddenLayers[x].destroyWeightMatrix(weights[x]);
    }

    m_outputLayer.destroyWeightMatrix(weights[m_numHiddenLayers]);

    delete [] weights;
    weights = NULL;

    //Delete the momentums array.
    delete [] momentums;
    momentums = NULL;

    //Delete the learnRates array.
    delete [] learnRates;
    learnRates = NULL;

    //Delete the numHiddenNeurons array.
    delete [] numNeurons;
    numNeurons = NULL;
}

void NeuralNetwork::getResults(const double *inputs, double *&outputs)
{
#ifdef DEBUG_NNOUTPUTS
    print("Calculating result for inputs:");
    for(int x = 0; x < m_numInputs; ++x)
    {
        print2(" ", inputs[x]);
    }
    printLine("");
#endif
    if(outputs != NULL)
        delete [] outputs;

    print("WILL IT WORK???");
    double *tempInputs = NULL;
    tempInputs = new double[m_numInputs];
    printLine("  YES!!!");
    NeuronLayer::copyArray(inputs, tempInputs, m_numInputs);

#ifdef DEBUG_NNOUTPUTS
    printLine("Calculating results from hidden layers");
#endif

    //Iterate over each hidden layer, passing the outputs one one layer as the inputs of the next.
    for(int x = 0; x < m_numHiddenLayers; ++x)
    {
        outputs = new double[m_hiddenLayers[x].getNumNeurons()];
        m_hiddenLayers[x].getResult(tempInputs, outputs);

        delete tempInputs;
        tempInputs = NULL;

        //Copy the outputs from this layer to be used as inputs for the next.
        //The output array will be used again in the next iteration,
        //so the current values must be stored elsewhere.
        NeuronLayer::copyArray(outputs, tempInputs, m_hiddenLayers[x].getNumNeurons());

        delete [] outputs;
    }

    outputs = new double[m_numOutputs];
    m_outputLayer.getResult(tempInputs, outputs);

#ifdef DEBUG_NNOUTPUTS
    print("Results:");
    for(int x = 0; x < m_numOutputs; ++x)
    {
        print2(" ", outputs[x]);
    }
    printLine("\n");
#endif
    delete tempInputs;
}

//Actual is not really needed.  The outputLayer contains the last set of actual outputs.
void NeuralNetwork::backpropagate(double *actual, double *expected)
{
    double *tempBlames;

#ifdef DEBUG_NEURALNETWORK
    printLine("\nBackpropagate");
    printLine("\nCalculate output blames");
#endif
    //Store each layer's blames in tempBlames to be passed on to the next successive layer.
    tempBlames = m_outputLayer.calcOutputBlames(actual, expected);

#ifdef DEBUG_NEURALNETWORK
    printLine("Calculate output blame deltas");
#endif
    m_outputLayer.calcBlameDeltas();

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
    //m_outputLayer.addMomentum();
    //m_outputLayer.addLearnRateToBlames();

    for(int x = m_numHiddenLayers - 1; x >= 0; --x)
    {
#ifdef DEBUG_NEURALNETWORK
    printLine3("Calc hidden layer ", x, " accumulated blames");
#endif
        m_hiddenLayers[x].calcAccBlames();

#ifdef DEBUG_NEURALNETWORK
    printLine3("Adding hidden layer ", x, " momentum");
#endif
        //m_hiddenLayers[x].addMomentum();

#ifdef DEBUG_NEURALNETWORK
    printLine3("Adding hidden layer ", x, " learning rate");
#endif
        //m_hiddenLayers[x].addLearnRateToBlames();
    }
}

void NeuralNetwork::applyWeightChanges()
{
    m_outputLayer.changeWeights();
    //m_outputLayer.setMomentumChanges();

    //m_outputLayer.resetAccBlames();
    for(int x = m_numHiddenLayers - 1; x >= 0; --x)
    {
#ifdef DEBUG_NEURALNETWORK
    printLine3("Changing hidden layer ", x, " weights");
#endif
        m_hiddenLayers[x].changeWeights();

#ifdef DEBUG_NEURALNETWORK
    printLine3("Setting hidden layer ", x, " momentum");
#endif
        //m_hiddenLayers[x].setMomentumChanges();

        //m_hiddenLayers[x].resetAccBlames();
    }
}
