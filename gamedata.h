#ifndef GAMEDATA_H
#define GAMEDATA_H

#include "boardstate.h"
#include "rulesengine.h"
class GameData
{
public:
    GameData() : m_startingState(NULL), m_currentState(NULL) {}

    ~GameData() { purge(); }

    void purge();

    virtual void init(RulesEngine *rulesEngine) = 0;
    virtual void resetBoard(RulesEngine *rulesEngine) = 0;
    void undoMove(RulesEngine *rulesEngine);
    void genNextMoves(int numTurns, RulesEngine *rulesEngine);
    BoardState *getCurrentState() { return m_currentState; }
    int getTurnNumber() { return m_turnNumber; }
    void setNextMove(const Grid *move);

protected:
    BoardState *m_startingState;
    BoardState *m_currentState;

    int m_turnNumber;
};

#endif // GAMEDATA_H
