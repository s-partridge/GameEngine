#include "tictactoemainwindow.h"

TicTacToeMainWidget::TicTacToeMainWindow(QObject *parent) :
    MainWidget(parent)
{
    m_gridView = new GameGridView(3, 3, this);
    m_panelView = new TicTacToeControlView(this);

    m_centralLayout->addWidget(m_gridView);
    m_centralLayout->addWidget(m_panelView);

    connect(m_gridView, SIGNAL(onSquareClicked(int,int)), this, SLOT(makeMove(int,int)));
}

void TicTacToeMainWidget::makeMove(int x, int y)
{
    TicTacToeGrid *currentGrid = new TicTacToeGrid();

    //Copy the contents of the grid view into this grid object.
    m_gridView->getGameGrid(currentGrid);

    currentGrid->squares[x][y] = (Elements::GenericPieceType)m_currentPlayer;
}
