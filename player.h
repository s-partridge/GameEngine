#ifndef PLAYER_H
#define PLAYER_H

#include "boardstate.h"
#include "grid.h"

class Player
{
public:
    virtual Player() {}
    virtual ~Player() {}
    virtual void makeMove(const BoardState *currentState, Grid *nextMove) = 0;
};

#endif // PLAYER_H
