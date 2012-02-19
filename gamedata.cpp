#include "gamedata.h"

void GameData::setNextMove(const Grid *move)
{
#ifdef DEBUG_GAMEDATA
    print("gameData->setNextMove to: ");
    for(int x = 0;  x < 3; ++x)
    {
        for(int y = 0; y < 3; ++y)
        {
            printLine5(x, ", ", y, " = ", move->squares[x][y]);
        }
    }

#endif

    m_currentState = m_currentState->getState(move);
    ++m_turnNumber;
}

void GameData::genNextMoves(int numTurns, RulesEngine *rulesEngine)
 {
#ifdef DEBUG_GAMEDATA
    print("gameData->genNextMoves");
#endif
    m_currentState->genNextStates(numTurns - 1, rulesEngine);
 }
void GameData::undoMove(RulesEngine *rulesEngine)
{
#ifdef DEBUG_GAMEDATA
    print("gameData->undoMove");
#endif

    //Move currentState back one level and remove the end of the tree.
    m_currentState = m_currentState->getParent();
    m_currentState->genNextStates(0, rulesEngine);
}
