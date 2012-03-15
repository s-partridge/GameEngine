#include "gamegridview.h"

GameGridView::GameGridView(int width, int height, QWidget *parent) :
    QWidget(parent), m_width(width), m_height(height)
{
    //Initialize the layout.
    m_centralLayout = new QGridLayout(this);

    setLayout(m_centralLayout);

    //Generate an initial array pointer.
    m_squares = new GridSquare**[width];

    for(int x = 0; x < width; ++x)
    {
        //Generate an array of pointers for each pointer.
        m_squares[x] = new GridSquare*[height];

        for(int y = 0; y < height; ++y)
        {
            //Initialize each button in the array.
            m_squares[x][y] = new GridSquare(x, y, this);
            //Connect each square's buttonClicked signal to the corresponding slot on the grid view.
            //Allows the grid itself to control actions from a click.
            connect(m_squares[x][y], SIGNAL(buttonClicked(int, int)), this, SLOT(squareClicked(int, int)));

            QFont font = m_squares[x][y]->font();

            font.setPointSize(20);
            font.setBold(true);

            m_squares[x][y]->setFont(font);

            //The order for the layout is given by (row, column), which means that the vertical placement is
            //set first.  Therefore, y is passed as the row parameter and x is passed as the column parameter.
            m_centralLayout->addWidget(m_squares[x][y], y, x);
        }
    }

    emptyGrid();
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
                    {
                        delete m_squares[x][y];
                        m_squares[x][y] = NULL;
                    }
                }

                delete [] m_squares[x];
                m_squares[x] = NULL;
            }
        }
        delete [] m_squares;
        m_squares = NULL;
    }

    if(m_centralLayout != NULL)
    {
        delete m_centralLayout;
        m_centralLayout = NULL;
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

void GameGridView::updateGrid(const Grid *newGrid)
{
    for(int x = 0; x < m_width; ++x)
    {
        for(int y = 0; y < m_height; ++y)
        {
            m_squares[x][y]->changePieceType(newGrid->squares[x][y]);

        }
    }

    updateText(newGrid);
}

void GameGridView::updateText(const Grid *newGrid)
{
    for(int x = 0; x < m_width; ++x)
    {
        for(int y = 0; y < m_height; ++y)
        {
            if(newGrid->squares[x][y] == Elements::EMPTY)
            {
                m_squares[x][y]->setText(" ");
                            m_squares[x][y]->setStyleSheet("QPushButton { color : black; }");
            }
            else if(newGrid->squares[x][y] == Elements::TYPE1)
            {
                std::cout << "SETTING TEXT!!!\n";
                m_squares[x][y]->setText("X");

                m_squares[x][y]->setStyleSheet("QPushButton { color : blue; }");
            }
            else if(newGrid->squares[x][y] == Elements::TYPE2)
            {
                m_squares[x][y]->setText("O");
                m_squares[x][y]->setStyleSheet("QPushButton { color : red; }");
            }
        }
    }
}

void GameGridView::emptyGrid()
{
    //For each square in squares, set its
    for(int x = 0; x < m_width; ++x)
    {
        for(int y = 0; y < m_height; ++y)
        {
            m_squares[x][y]->changePieceType(Elements::EMPTY);
            m_squares[x][y]->setText(" ");
        }
    }
}

void GameGridView::squareClicked(int x, int y)
{
    printLine4("Button pressed: ", x, ", ", y);
    emit onSquareClicked(x, y);
}
