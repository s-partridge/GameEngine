#include "neuralnetplayer.h"
#include <stdlib.h>

void NeuralNetPlayer::purge()
{
    if(m_neuralNetwork != NULL)
    {
        delete m_neuralNetwork;
        m_neuralNetwork = NULL;
    }
}

void NeuralNetPlayer::setNeuralNetworkFromFile(string filename)
{
    //Make sure one doesn't already exist.
    if(m_neuralNetwork != NULL)
        delete m_neuralNetwork;

    m_neuralNetwork = new NeuralNetwork(filename);
}

void NeuralNetPlayer::generateNeuralNetwork(string filename, const int numInputs, const int numOutputNeurons, const int numHiddenLayers, const int *numHiddenNeurons, const double *momentum, const double *learnRate)
{
    if(m_neuralNetwork != NULL)
        delete m_neuralNetwork;

    m_neuralNetwork = new NeuralNetwork(filename, numInputs, numOutputNeurons,
                                        numHiddenLayers, numHiddenNeurons, momentum, learnRate);
}

//Set all neuron layers to the same learning rate.
void NeuralNetPlayer::setLearnRate(double rate)
{
    for(int x = 0; x < m_neuralNetwork->getNumLayers(); ++x)
    {
        m_neuralNetwork->setLayerLearnRate(x, rate);
    }
}

//Set all neuron layers to the same momentum value.
void NeuralNetPlayer::setMomentum(double rate)
{
    for(int x = 0; x < m_neuralNetwork->getNumLayers(); ++x)
    {
        m_neuralNetwork->setLayerMomentum(x, rate);
    }
}

void NeuralNetPlayer::makeMove(const BoardState *currentState, Grid *nextMove)
{
    //Create an array for the outputs.
    double *results = new double[NUM_OUTPUTS];
    //Calculate outputs.
    getResults(currentState, results);

    //Create a grid to hold the converted outputs.
    Grid *outputs = NULL;
    m_rulesEngine->doubleArrayToGrid(outputs, results, m_player, 0, NUM_OUTPUTS);

    //Create a grid to hold a potential replacement output.
    Grid *corrected = NULL;
    //Confirm that the results are valid.  If not, generate a replacement.
    checkResults(currentState, outputs, corrected);

    //Make sure nextMove does not contain other data.
    if(nextMove != NULL)
        delete nextMove;

    //Set nextMove equal to the result of the neural network.
    if(corrected == NULL)
    {
        nextMove = outputs;
    }
    else
    {
        nextMove = corrected;
    }
}

void NeuralNetPlayer::getResults(const BoardState *currentState, double *results)
{
    //Convert the current state into a flat array.
    const Grid *temp = currentState->getCurrentGrid();
    double *inputs = new double[NUM_INPUTS];
    m_rulesEngine->gridToDoubleArray(temp, inputs, m_player, 0, NUM_INPUTS);

    //Pass the array to the neural network to generate a new move, storing it in results.
    m_neuralNetwork->getResults(inputs, results);
}

void NeuralNetPlayer::checkResults(const BoardState *currentState, Grid *results, Grid *corrected)
{
    //If the given move was valid, do nothing.  Otherwise, replace it with a random
    //correct move.
    if(m_rulesEngine->isValidMove(currentState->getCurrentGrid(), results))
    {
        return;
    }
    else
    {
        corrected = currentState->getState(rand() % currentState->getNumNextStates());
        return;
    }
}



