#ifndef CONNECTFOURGAMEDATA_H
#define CONNECTFOURGAMEDATA_H

#include "gamedata.h"
#include "connectfourgrid.h"

class ConnectFourGameData : public GameData
{
public:
    ConnectFourGameData() {}

    void resetBoard(RulesEngine *rulesEngine);
    void init(RulesEngine *rulesEngine);
};

#endif // CONNECTFOURGAMEDATA_H
