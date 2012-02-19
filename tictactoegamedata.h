#ifndef TICTACTOEGAMEDATA_H
#define TICTACTOEGAMEDATA_H

#include "gamedata.h"
#include "tictactoegrid.h"

class TicTacToeGameData : public GameData
{
public:
    TicTacToeGameData() {}

    void resetBoard(RulesEngine *rulesEngine);
    void init(RulesEngine *rulesEngine);
};

#endif // TICTACTOEGAMEDATA_H
