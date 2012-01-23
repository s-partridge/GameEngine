#include "grid.h"

void Grid::purge()
{
    if(squares != NULL)
    {
        for(int x = 0; x < width; ++x)
        {
            if(squares[x] != NULL)
            {
                delete [] squares[x];
            }
        }

        delete [] squares;
        squares = NULL;
    }
}

int Grid::numPiecesOfType(Elements::GenericPieceType pieceType)
{
    int count = 0;

    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            //Increment the count for each square found equal to pieceType.
            if(squares[x][y] == pieceType)
                ++count;
        }
    }

    return count;
}

int Grid::numDifferences(const Grid *rhs)
{
    int count = 0;

    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            //Increment for each differing square found.
            if(squares[x][y] != rhs->squares[x][y])
            {
                ++count;
            }
        }
    }

    return count;
}

Grid& Grid::operator =(const Grid &rhs)
{
    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            //Set each square equal to the right hand side.
            squares[x][y] = rhs.squares[x][y];
        }
    }
}

bool Grid::operator ==(const Grid &rhs)
{
    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            //If any square is different, the grids are not equal.
            if(squares[x][y] != rhs.squares[x][y])
                return false;
        }
    }

    return true;
}
