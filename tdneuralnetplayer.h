#ifndef TDNEURALNETPLAYER_H
#define TDNEURALNETPLAYER_H

#include "neuralnetplayer.h"
#include "managedarray.h"
#include "tdneuralnetwork.h"

#define GAMMA 1.0
#define LAMBDA 0.5
#define TD_LEARN_RATE 0.2
#define TD_NUM_OUTPUTS 1
#define TD_EXPECTED_ROUNDS 10

#define RANDOM_MOVE_INTERVAL 120

//Used to contain a record of each set of outputs and output layer weight changes
//for an entire game.  Meant to be stored until backpropagation starts at the end,
//and be expandable, such that the number of game rounds is not static.
struct PastWeightMatrix
{
private:
    int m_numOutputs;
    int m_numRounds;

    void expandArray(double **&arr, int newSize);
    void copyArray(double **source, double **dest, int size);


public:
    double **previousOutputs;
    double **idealOutputs;

    PastWeightMatrix(int numRounds, int numOutputs);

    ~PastWeightMatrix() { purge(); }

    int numOutputs() { return m_numOutputs; }
    int numRounds() { return m_numRounds; }
    void purge();

    void resetArrays(int numRounds, int numOutputs);
    void increaseSize();
};

class TDNeuralNetPlayer : public NeuralNetPlayer
{
public:
    TDNeuralNetPlayer(Elements::PlayerType player, int numExpectedRounds);

    void resetNumRounds() { m_currentRound = 0; }
    int getNumRounds() { return m_currentRound; }

    //Make a move, confirm that it's valid, and return it.
    void makeMove(const BoardState *currentState, Grid *&nextMove);

    void setCalcAsMax(bool calc) { calcAsMax = calc; }

    void endStateReached(BoardState *currentState, Elements::GameState finalState, bool youMovedLast, int numRounds = 0);

    void reset();

private:
    int m_currentRound;
    PastWeightMatrix m_oldWeights;

    bool calcAsMax;
};

#endif // TDNEURALNETPLAYER_H
