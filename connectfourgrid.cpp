#include "connectfourgrid.h"

void ConnectFourGrid::init()
{
    width = 6;
    height = 4;

#ifdef DEBUG_GRID
    print("Initializing a connect four grid.");
#endif

    squares = new Elements::GenericPieceType*[width];

    for(int x = 0; x < width; ++x)
    {
        squares[x] = new Elements::GenericPieceType[height];
        for(int y = 0; y < height; ++y)
        {
            squares[x][y] = Elements::EMPTY;
        }
    }
}

int ConnectFourGrid::getFirstDifference(const Grid *rhs) const
{
    int diffIndex = 0;

    //Return the index of the first different square as an integer.
    for(int x = 0;  x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            if(squares[x][y] != rhs->squares[x][y])
                return diffIndex;
        }
        //The difference between this and the base implementation is that this will only count
        //the number of columns that has been covered, since the only legal moves are column based.
        ++diffIndex;
    }

    //If no difference is found.
    return -1;
}
