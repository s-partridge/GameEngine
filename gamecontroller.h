#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <string>
#include "human.h"
#include "neuralnetplayer.h"
#include "datacontroller.h"
#include "neuralnetbuilder.h"
#include "trainer.h"
#include "rulesengine.h"

using namespace std;

class GameController
{
public:
    GameController();

    ~GameController() { purge(); }

    void purge();

    //TODO: create or load a neural network from a file.
    void createNNPlayer(Elements::PlayerType player, string filename);
    void loadNNPlayer(Elements::PlayerType player, string filename);

    void setDataController(DataController *controller) { m_dataController = controller; }
    void setAIBuilder(NeuralNetBuider *AIBuilder) { m_AIBuilder = AIBuilder; }
    void setAITrainer(Trainer *trainer) { m_AITrainer = trainer; }
    void setRulesEngine(RulesEngine *rulesEngine) { m_rulesEngine = rulesEngine; }

    //Try to make move based on passed grid and player ID.
    //If player is AI, move will be ignored and currentState
    //from the move tree will be used instead.
    Grid *attemptMove(const Grid *move, Elements::PlayerType player);

    void resetGame();
    void undoMove();

    //Trains the AI matching the given player ID.
    //Does nothing if player is human.
    void trainAI(Elements::PlayerType player);

    //Switches the m_player pointer to m_NNPlayer from m_human.
    void swapHumanForAI(Elements::PlayerType player);
    //Switches the m_player pointer from m_NNPlayer to m_human.
    void swapAIForHuman(Elements::PlayerType player);

    //Change currentPlayer from player1 to player2 or vice versa.
    void switchCurrentPlayer();

    //Returns true if the referenced player is not human, false otherwise.
    bool isAI(Elements::PlayerType player);

private:
    Player *m_player1;
    Player *m_player2;

    string m_NN1Filename;
    string m_NN2Filename;
    NeuralNetPlayer *m_NNPlayer1;
    NeuralNetPlayer *m_NNPlayer2;

    Human *m_human1;
    Human *m_human2;
    Player *m_currentPlayer;

    DataController *m_dataController;
    NeuralNetBuilder *m_AIBuilder;
    Trainer *m_AITrainer;
    RulesEngine *m_rulesEngine;
};

#endif // GAMECONTROLLER_H
