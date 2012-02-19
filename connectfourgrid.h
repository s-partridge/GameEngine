#ifndef CONNECTFOURGRID_H
#define CONNECTFOURGRID_H

#include "grid.h"

class ConnectFourGrid : public Grid
{
public:
    ConnectFourGrid() { init(); }

    void init();
    int getFirstDifference(const Grid *rhs) const;
};

#endif // CONNECTFOURGRID_H
