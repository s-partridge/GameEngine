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

//Apparently required for NULL
#include <stddef.h>
#include <sstream>

#include "datatypes.h"
#include "macros.h"

struct Grid
{
    Elements::GenericPieceType **squares;
    int width, height;

    Grid() { squares = NULL; }

    virtual void init() = 0;

    void purge();

    ~Grid() { purge(); }

    virtual int getFirstDifference(const Grid *rhs) const;

    //Count the number of instances of a specific piece.
    int numPiecesOfType(Elements::GenericPieceType pieceType) const;

    //Count the number of differing squares between this and rhs.
    int numDifferences(const Grid *rhs) const;

    std::string toString() const;
    void fromString(const std::string input);

    //Comparison operator overloads.
    Grid &operator=(const Grid &rhs);
    bool operator==(const Grid &rhs) const;
    bool operator!=(const Grid &rhs) const { return ! (*this == rhs); }
    void toCharArray(char *&emptyArray) const;

    //Ostream operator
    friend std::ostream & operator<<(std::ostream &o, const Grid &grid);
};

std::ostream & operator<<(std::ostream &o, const Grid &grid);

#endif // GRID_H
