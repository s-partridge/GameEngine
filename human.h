#ifndef HUMAN_H
#define HUMAN_H

class Human : public Player
{
public:
    virtual Human() : Player() {}
    virtual ~Human() {}

    void makeMove(const BoardState *currentState, Grid *nextMove) { nextMove = NULL; }
};

#endif // HUMAN_H
