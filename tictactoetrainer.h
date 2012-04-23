#ifndef TICTACTOETRAINER_H
#define TICTACTOETRAINER_H

#include <map>
#include "trainer.h"
#include "tictactoegrid.h"
#include "boardstate.h"
#include "datatypes.h"
#include "movetablebuilder.h"

#define NUM_CHOICES 2

//Used to compare three board pieces and determine if exactly two are of type opp.
#define compareSet(x, y, z, opp) ( (x == opp && y == x && z != x) || (x == opp && y != x && z == x) || (x != y && y == opp && z == y) )

//Dividing the database into multiple files accomplishes two goals:
//First, it allows for more diversified training in trainFromDatabase.
//Second, it allows more games to be saved; STL containers are limited to the size of an
//  unsigned integer, meaning arounf 4,000,000 games.  If I train my networks long enough,
//  I could easily reach that number in one database.  Since different types of training
//  tend to yield different kinds of games, I can store a larger variety.
#define FILENAME_TERRIBLE       "terrible.db"
#define FILENAME_VERSUS         "versus.db"
#define FILENAME_SELF           "self.db"

class TicTacToeTrainer : public Trainer
{
    BoardState *pickNextMoveToTrain(BoardState *currentState) const;

    //No need to add database to these two methods; they don't actually do anything now.
    //TODO: remove trainOnBestTrackPlus() and trainOnBestStatesOnly()
    AITrainingStats trainOnBestTrackPlus(NeuralNetPlayer *player) const;
    AITrainingStats trainOnBestStatesOnly(NeuralNetPlayer *player) const;

    AITrainingStats trainVersusSelf(NeuralNetPlayer *player, GameDatabase *database) const;
    AITrainingStats trainVersusTerriblePlayer(NeuralNetPlayer *player, GameDatabase *database) const;

    void moveBlocker(BoardState *&currentState, Elements::PlayerType friendly, Elements::PlayerType opponent) const;

    void trainOnce(NeuralNetPlayer *player, Grid *gameBoard, Grid *userOutput,
                   double *&expected, double *&outputs, Elements::PlayerType currentPlayer,
                   map<string, int> &hashmap) const;

public:
    TicTacToeTrainer(int numTrainingIterations, RulesEngine *rulesEngine) : Trainer(numTrainingIterations, rulesEngine) {}

    AITrainingStats trainNetwork(NeuralNetPlayer *player, GameDatabase *database);
    AITrainingStats trainTwoNetworks(NeuralNetPlayer *player1, NeuralNetPlayer *player2, GameDatabase *database);
};

/*
class CompleteTicTacToeTrainer : public Trainer
{
    BoardState *pickNextMoveToTrain(BoardState *currentState) const;
public:
    TicTacToeTrainer(int numTrainingIterations, RulesEngine *rulesEngine) : Trainer(numTrainingIterations, rulesEngine) {}

    AITrainingStats trainNetwork(NeuralNetPlayer *player) const;
};*/

#endif // TICTACTOETRAINER_H
