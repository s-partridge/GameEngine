#include "tictactoegrid.h"

void TicTacToeGrid::init()
{
    width = 3;
    height = 3;

    squares = new *Elements::GenericPieceType[width];

    for(int x = 0; x < width; ++x)
    {
        squares[x] = new Elements::GenericPieceType[height];
    }
}
