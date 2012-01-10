#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <string>
#include "human.h"
#include "neuralnetplayer.h"
#include "datacontroller.h"
#include "viewcontroller.h"
class GameController
{
public:
    GameController();

    //TODO: figure out exact parameters for this.
//    void loadNNPlayer(string filename, ..., int player);

    void setViewController(ViewController *controller);
    void setDataController(dataController *controller);

    //Try to make move based on passed grid and player ID.
    //If player is AI, move will be ignored and currentState
    //from the move tree will be used instead.
    Grid *attemptMove(const Grid *move, int player);

    void resetGame();
    void undoMove();

    void tellGameOver();
    void tellNewGame();

    //Trains the AI matching the given player ID.
    //Does nothing if player is human.
    void trainAI(int player);

    //Switches the m_player pointer to m_NNPlayer from m_human.
    void swapHumanForAI(int player);
    //Switches the m_player pointer from m_NNPlayer to m_human.
    void swapAIForHuman(int player);

    //Returns true if the referenced player is not human, false otherwise.
    bool isAI(int player);

private:
    Player *m_player1;
    Player *m_player2;

    string m_NN1Filename;
    string m_NN2Filename;
    NeuralNetPlayer *m_NNPlayer1;
    NeuralNetPlayer *m_NNPlayer2;

    Human *m_human1;
    Human *m_human2;
    Player *currentPlayer;

    DataController *dataController;
    ViewController *viewController;
};

#endif // GAMECONTROLLER_H
