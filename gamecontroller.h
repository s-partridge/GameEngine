#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

//For typeid keyword and related functions.
#include <typeinfo>

#include <string>
#include "human.h"
#include "neuralnetplayer.h"
#include "datacontroller.h"
#include "neuralnetbuilder.h"
#include "trainer.h"
#include "rulesengine.h"
#include "macros.h"

using namespace std;

class GameController
{
public:
    GameController();

    ~GameController() { purge(); }

    void purge();

    //Create or load a neural network from a file.  Generates a new player if the
    //file passed in does not exist.  Otherwise, loads network values.
    void createNNPlayer(Elements::PlayerType player, string filename);

    //Save a neural network, given a new file name.
    void saveNNPlayer(Elements::PlayerType player, string filename);
    //Save a neural network under the existing file name.
    void saveNNPlayer(Elements::PlayerType player);

    void setDataController(DataController *controller) { m_dataController = controller; }
    void setAIBuilder(NeuralNetBuilder *AIBuilder) { m_AIBuilder = AIBuilder; }
    void setAITrainer(Trainer *trainer) { m_AITrainer = trainer; }
    void setRulesEngine(RulesEngine *rulesEngine) { m_rulesEngine = rulesEngine; }

    //Try to make move based on passed grid and player ID.
    //If player is AI, move will be ignored and currentState
    //from the move tree will be used instead.
    const Grid *attemptMove(const Grid *move);

    void resetGame();
    void undoMove();

    void gameOver();
    bool isGameOver(const Grid *move) { return m_dataController->isGameOver(move); }

    //Trains the AI matching the given player ID.
    //Trains both AIs if Elements::NONE is passed, assuming both player are AI.
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

    bool p1IsAI;
    bool p2IsAI;

    bool isP1Turn;

    Human *m_human1;
    Human *m_human2;
    Player *m_currentPlayer;

    DataController *m_dataController;
    NeuralNetBuilder *m_AIBuilder;
    Trainer *m_AITrainer;
    RulesEngine *m_rulesEngine;
};

#endif // GAMECONTROLLER_H
