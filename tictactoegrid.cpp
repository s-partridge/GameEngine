#include "tictactoegrid.h"

void TicTacToeGrid::init()
{
    width = 3;
    height = 3;

#ifdef DEBUG_GRID
    print("Initializing a tic tac toe grid.");
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
