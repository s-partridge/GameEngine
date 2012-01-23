#ifndef HUMAN_H
#define HUMAN_H

#include "boardstate.h"
#include "player.h"

class Human : public Player
{
public:
    Human() : Player() {}
    virtual ~Human() {}

    void makeMove(const BoardState *currentState, Grid *nextMove) { nextMove = NULL; }
};

#endif // HUMAN_H
