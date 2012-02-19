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

int Grid::numPiecesOfType(Elements::GenericPieceType pieceType) const
{
    int count = 0;

#ifdef DEBUG_GRID
    printLine3("NumPiecesOfType(", pieceType,")");
#endif

    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            //Increment the count for each square found equal to pieceType.
            if(squares[x][y] == pieceType)
                ++count;
        }
    }

#ifdef DEBUG_GRID
    printLine3("Found ", count, " pieces.");
#endif

    return count;
}

int Grid::getFirstDifference(const Grid *rhs) const
{
    int diffIndex = 0;

    //Return the index of the first different square as an integer.
    for(int x = 0;  x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            if(squares[x][y] != rhs->squares[x][y])
                return diffIndex;
            ++diffIndex;
        }
    }

    //If no difference is found.
    return -1;
}

int Grid::numDifferences(const Grid *rhs) const
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

Grid &Grid::operator =(const Grid &rhs)
{
    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            //Set each square equal to the right hand side.
            squares[x][y] = rhs.squares[x][y];
        }
    }

    return *this;
}

bool Grid::operator ==(const Grid &rhs) const
{
#ifdef DEBUG_GRID
    print("RHS:");
    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            printLine5(x, ", ", y, " = ", rhs.squares[x][y]);
        }
    }

    print("This:");
    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            printLine5(x, ", ", y, " = ", squares[x][y]);
        }
    }
#endif

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

std::string Grid::toString() const
{
    std::string o;
    std::stringstream ss;

    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            ss << squares[x][y];

            //Add a space between all elements, but add no space after the last element.
            if(y < height - 1)
                ss << " ";
        }
        if(x < width - 1)
            ss << " ";
    }

    o = ss.str();
    return o;
}

void Grid::fromString(const std::string input)
{
    std::stringstream ss;

    int temp;
    //Put the input string in the stream.
    ss << input;
    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            //Pull each element out of the stream and copy it to the grid.
            ss >> temp;
            squares[x][y] = (Elements::GenericPieceType)temp;
        }
    }
}

std::ostream & operator<<(std::ostream &o, const Grid &grid)
{
    for(int x = 0; x < grid.width; ++x)
    {
        for(int y = 0; y < grid.height; ++y)
        {
            o << grid.squares[x][y];

            //Add a space between all elements, but add no space after the last element.
            if(y < grid.height - 1)
                o << " ";
        }
        if(x < grid.width - 1)
            o << " ";
    }

    return o;
}
