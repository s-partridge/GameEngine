#include "gamegridview.h"

GameGridView::GameGridView(int width, int height, QWidget *parent) :
    QWidget(parent), m_width(width), m_height(height)
{
    //Generate an initial array pointer.
    squares = new **GridSqare[width];

    for(int x = 0; x < width; ++x)
    {
        //Generate an array of pointers for each pointer.
        squares[x] = new *GridSquare[height];

        for(int y = 0; y < height; ++y)
        {
            //Initialize each button in the array.
            squares[x][y] = new GridSquare(x, y, this);
            //Connect each square's buttonClicked signal to the corresponding slot on the grid view.
            //Allows the grid itself to control actions from a click.
            connect(squares[x][y], SIGNAL(buttonClicked(int, int)), this, SLOT(squareClicked(int, int)));
        }
    }
}

void GameGridView::purge()
{
    if(m_squares != NULL)
    {
        for(int x = 0; x < m_width; ++x)
        {
            if(m_squares[x] != NULL)
            {
                for(int y = 0; y < m_height; ++y)
                {
                    if(m_squares[x][y] != NULL)
                        delete m_squares[x][y];
                }

                delete [] m_squares[x];
            }
        }

        delete [] m_squares;
    }
}

GameGridView::~GameGridView()
{
    purge();
}

void GameGridView::getGameGrid(Grid *gameGrid)
{
    for(int x = 0; x < m_width; ++x)
    {
        for(int y = 0; y < m_height; ++y)
        {
            //Copy each grid square into this grid.
            // TODO: Change this.  Keep the piece type as a local value only.
            gameGrid->squares[x][y] = m_squares[x][y]->getPieceType();
        }
    }
}

void GameGridView::updateGrid(Grid *newGrid)
{
    for(int x = 0; x < m_width; ++x)
    {
        for(int y = 0; y < m_height; ++y)
        {
            m_squares[x][y]->changePieceType(newGrid->squares[x][y]);
        }
    }
}
