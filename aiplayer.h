#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "player.h"

//Intermediate class.
//May be scoped out.

class AIPlayer : public Player
{
public:
    AIPlayer() : Player() {}
    virtual ~AIPlayer() {}
};

#endif // AIPLAYER_H
