#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "player.h"

//Intermediate class.
//May be scoped out.

class AIPlayer : public Player
{
public:
    virtual AIPlayer() : Player() {}
    virtual ~AIPlayer() {}
};

#endif // AIPLAYER_H
