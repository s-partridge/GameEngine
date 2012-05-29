#ifndef NEURALNETPLAYER_H
#define NEURALNETPLAYER_H

#include "aiplayer.h"
#include "neuralnetwork.h"
#include "statstructures.h"

class NeuralNetPlayer : public AIPlayer
{
public:
    NeuralNetPlayer(Elements::PlayerType player) : m_neuralNetwork(NULL), m_player(player), m_rulesEngine(NULL), train(true), m_searchDepth(1) {}

    void purge();

    ~NeuralNetPlayer() { purge(); }
    //Load an existing neural network.
    void setNeuralNetworkFromFile(string filename);
    //Create a new neural network from a set of parameters.
    void generateNeuralNetwork(string filename, const int numInputs, const int numOutputNeurons,
                               const int numHiddenLayers, const int *numHiddenNeurons,
                               const double *momentum, const double *learnRate);

    //Some neural networks can be used with the minimax calculation.
    //SetCalcAsMax allows an interface to assign minimum or maximum
    //calculations when deciding on a next move.
    virtual void setCalcAsMax(bool calc) {}
    virtual bool getCalcAsMax() { return true; }

    //Save the neural network.
    void saveNeuralNetwork(string filename) { m_neuralNetwork->saveNNP(filename); }
    void saveNeuralNetwork() { m_neuralNetwork->saveNNP(); }

    void setRulesEngine(RulesEngine *rulesEngine) { m_rulesEngine = rulesEngine; }

    void setTrain(bool train) { this->train = train; }

    //Make a move, confirm that it's valid, and return it.
    void makeMove(const BoardState *currentState, Grid *&nextMove);
    //Make a move and return the raw results
    void getResults(const BoardState *currentState, double *&results);
    void getResults(const Grid *currentGrid, Elements::PlayerType player, double *&results);

    int getHiddenLayerNeurons(int layer) { return m_neuralNetwork->getNumHiddenNeurons(layer); }

    //Check the next move returned from the neural network.
    //Corrected will be NULL if the neural network made a valid move.
    void checkResults(const BoardState *currentState, Grid *results, Grid *&corrected);

    //Train the neural network if necessary.
    void trainNetwork(double *actualOutputs, double *expectedOutputs) { if(train) m_neuralNetwork->backpropagate(actualOutputs, expectedOutputs); }

    Elements::PlayerType getPlayer() { return m_player; }
    void setPlayer(Elements::PlayerType player) { m_player = player; }

    void setActivationFunction(ActivationFunctor *activation, int layerNumber) { m_neuralNetwork->setLayerActivation(layerNumber, activation); }
    void setActivationFunction(ActivationFunctor *activation);

    void setLearnRate(double rate, int layerNumber) { m_neuralNetwork->setLayerLearnRate(layerNumber, rate); }
    void setLearnRate(double rate);

    void setMomentum(double rate, int layerNumber) { m_neuralNetwork->setLayerMomentum(layerNumber, rate); }
    void setMomentum(double rate);

    void setSearchDepth(int depth) { m_searchDepth = depth; }
    int searchDepth() { return m_searchDepth; }

protected:
    NeuralNetwork *m_neuralNetwork;
    Elements::PlayerType m_player;
    RulesEngine *m_rulesEngine;

    int m_searchDepth;

    bool train;
};

#endif // NEURALNETPLAYER_H
