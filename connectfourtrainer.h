#ifndef CONNECTFOURTRAINER_H
#define CONNECTFOURTRAINER_H

#include "trainer.h"
#include "connectfourgrid.h"
#include "connectfourrulesengine.h"
#include "boardstate.h"
#include "datatypes.h"

#define NUM_FUNCTIONS 2
//Used to compare four board pieces and determine if exactly two are of type opp.
#define compareC4Set(w, x, y, z, opp) ( (w == opp && x == opp && y == opp && z != w) || (w == opp && x == opp && y != w && z == opp) || (w == opp && x != w && y == opp && z == opp) || (w != opp && x == opp && y == opp && z == opp) )

//Dividing the database into multiple files accomplishes two goals:
//First, it allows for more diversified training in trainFromDatabase.
//Second, it allows more games to be saved; STL containers are limited to the size of an
//  unsigned integer, meaning around 4,000,000 games.  If I train my networks long enough,
//  I could easily reach that number in one database.  Since different types of training
//  tend to yield different kinds of games, I can store a larger variety.
#define FILENAME_MOVEBLOCKER    "moveblocker.db"
#define FILENAME_MULTIPLE       "multiple.db"
#define FILENAME_LEFTSIDE       "leftside.db"
#define FILENAME_VERSUS         "versus.db"
#define FILENAME_SELF           "self.db"

class ConnectFourTrainer : public Trainer
{
    bool printGameStrings;
    static BoardState *moveVerticalRight(BoardState *&currentState, Elements::PlayerType friendly, Elements::PlayerType opponent);
    static BoardState *moveVertical(BoardState *&currentState, Elements::PlayerType friendly, Elements::PlayerType opponent);
    static BoardState *moveToWin(BoardState *&currentState, Elements::PlayerType friendly, Elements::PlayerType opponent);
    /*
    BoardState *moveHorizontal(BoardState *&currentState, Elements::PlayerType friendly, Elements::PlayerType opponent) const;
    */

    AITrainingStats trainFromDatabase(NeuralNetPlayer *player, GameDatabase *database);
    AITrainingStats trainFromDatabaseWithFile(NeuralNetPlayer *player, GameDatabase *database, string filename) const;

    AITrainingStats trainVersusFunction(NeuralNetPlayer *player, GameDatabase *database, BoardState *(*trainerFunction)(BoardState *&, Elements::PlayerType, Elements::PlayerType)) const;
    AITrainingStats trainVersusSelf(NeuralNetPlayer *player, GameDatabase *database) const;
    AITrainingStats trainVersusTerriblePlayer(NeuralNetPlayer *player, GameDatabase *database) const;
    AITrainingStats trainVersusMultiple(NeuralNetPlayer *player, GameDatabase *database) const;
    AITrainingStats trainVersusMoveBlocker(NeuralNetPlayer *player, GameDatabase *database) const;
public:
    ConnectFourTrainer(int numTrainingIterations, RulesEngine *rulesEngine) : Trainer(numTrainingIterations, rulesEngine), printGameStrings(false) {}

    static BoardState *moveBlocker(BoardState *&currentState, Elements::PlayerType friendly, Elements::PlayerType opponent);
    AITrainingStats trainNetwork(NeuralNetPlayer *player, GameDatabase *database);
    AITrainingStats trainTwoNetworks(NeuralNetPlayer *player1, NeuralNetPlayer *player2, GameDatabase *database);
};

#endif // CONNECTFOURTRAINER_H
