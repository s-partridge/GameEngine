#ifndef NEURALNETPLAYER_H
#define NEURALNETPLAYER_H

#include "aiplayer.h"
#include "neuralnetwork.h"
#include "trainer.h"
#include "statstructures.h"

class NeuralNetPlayer : public AIPlayer
{
public:
    NeuralNetPlayer(Elements::PlayerType player) : m_player(player), m_neuralNetwork(NULL), m_trainer(NULL), m_rulesEngine(NULL) {}

    void purge();

    ~NeuralNetPlayer() { purge(); }
    //Load an existing neural network.
    void setNeuralNetworkFromFile(string filename);
    //Create a new neural network from a set of parameters.
    void generateNeuralNetwork(string filename, const int numInputs, const int numOutputNeurons,
                               const int numHiddenLayers, const int *numHiddenNeurons,
                               const double *momentum, const double *learnRate);

    void setTrainer(Trainer *trainer) { m_trainer = trainer; }

    void setRulesEngine(RulesEngine *rulesEngine) { m_rulesEngine = rulesEngine; }

    //Make a move, confirm that it's valid, and return it.
    void makeMove(const BoardState *currentState, Grid *nextMove);
    //Make a move and return the raw results
    void getResults(const BoardState *currentState, double *results);

    //Check the next move returned from the neural network.  Corrected will be NULL
    //if the neural network made a valid move.
    void checkResults(const BoardState *currentState, Grid *results, Grid *corrected);

    Elements::PlayerType getPlayer() { return m_player; }
    void setPlayer(Elements::PlayerType player) { m_player = player; }

    void setLearnRate(double rate, int layerNumber) { m_neuralNetwork->setLayerLearnRate(layerNumber, rate); }
    void setLearnRate(double rate);

    void setMomentum(double rate, int layerNumber) { m_neuralNetwork->setLayerMomentum(layerNumber, rate); }
    void setMomentum(double rate);

    //Change the number of training iterations.
    void setNumTrainingIterations(int numIterations) { m_trainer->setNumIterations(numIterations); }
    //Train the neural net using the trainer object.
    AITrainingStats train() { return m_trainer->trainNetwork(m_neuralNetwork); }

private:
    NeuralNetwork *m_neuralNetwork;
    Trainer *m_trainer;
    Elements::PlayerType m_player;
    RulesEngine *m_rulesEngine;
};

#endif // NEURALNETPLAYER_H
