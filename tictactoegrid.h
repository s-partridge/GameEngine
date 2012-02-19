#ifndef TICTACTOEGRID_H
#define TICTACTOEGRID_H

#include "grid.h"
#include "datatypes.h"

class TicTacToeGrid : public Grid
{
public:
    TicTacToeGrid() { init(); }

    void init();
};

#endif // TICTACTOEGRID_H
