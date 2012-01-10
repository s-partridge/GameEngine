///////////////////////////////////////////////////////////////////////////////
//
//  file:    grid.h
//  author:  Samuel Partridge
//  project: Game Engine
//  purpose: Interface for game grids used by the gameState class.
//           Allows specific grids to be used for different games.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef GRID_H
#define GRID_H

class Grid
{
    int **squares;
public:
    virtual Grid() {}
    virtual void init() = 0;

    void purge();

    //Count the number of instances of a specific piece.
    int numPiecesOfType(int pieceType);

    //Count the number of differing squares between this and rhs.
    int numDifferences(const Grid *rhs);

    //Comparison operator overloads.
    bool operator==(const Grid &rhs);
    bool operator!=(const Grid &rhs) { return ! (this == rhs); }
};

#endif // GRID_H
