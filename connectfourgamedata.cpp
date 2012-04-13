#include "connectfourgamedata.h"

void ConnectFourGameData::resetBoard(RulesEngine *rulesEngine)
{
#ifdef DEBUG_C4GAMEDATA
    printLine("C4GameData->resetBoard()");
    std::string gameResult;
    m_currentState->toString(gameResult);
    printLine2("Move list:", gameResult);
#endif

    //Delete the entire current tree.
    delete m_startingState;
    init(rulesEngine);
}

void ConnectFourGameData::init(RulesEngine *rulesEngine)
{
    //Generate Connect Four specific states.
    ConnectFourGrid *startingGrid = new ConnectFourGrid();

    m_startingState = new BoardState(startingGrid, NULL, Elements::PLAYER_1, rulesEngine, 1);
    m_currentState = m_startingState;
    m_turnNumber = 0;
}
