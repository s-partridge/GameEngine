#include "gamedata.h"

void GameData::setNextMove(const Grid *move)
{
    m_currentState = m_currentState->getState(move);
    ++m_turnNumber;
}

void GameData::resetBoard(RulesEngine *rulesEngine)
{
    BoardState *temp;

    //Empty the tree and return to the first state.
    while(m_currentState->getParent() != NULL)
    {
        temp = m_currentState;
        m_currentState = m_currentState->getParent();
        delete temp;
    }

    m_currentState->genNextStates(0, rulesEngine);

    m_turnNumber = 0;
}

void GameData::undoMove(RulesEngine *rulesEngine)
{
    //Move currentState back one level and remove the end of the tree.
    m_currentState = m_currentState->getParent();
    m_currentState->genNextStates(0, rulesEngine);
}
