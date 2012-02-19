#ifndef PLAYER_H
#define PLAYER_H

#include "boardstate.h"
#include "grid.h"
#include "datatypes.h"

class Player
{
public:
    Player() {}
    virtual ~Player() {}
    virtual void makeMove(const BoardState *currentState, Grid *&nextMove) = 0;

protected:
    Elements::PlayerType m_player;
};

#endif // PLAYER_H
