#ifndef GAMEDATA_H
#define GAMEDATA_H

#include "boardstate.h"
#include "rulesengine.h"
class GameData
{
public:
    GameData();

    void resetBoard();
    void undoMove();
    void genNextMoves(int numTurns);
    BoardState *getCurrentState() { return m_currentState; }
    int getTurnNumber() { return m_turnNumber; }
    void setNextMove(const grid *move);

private:
    BoardState *m_startingState;
    BoardState *m_currentState;

    int m_turnNumber;
};

#endif // GAMEDATA_H
