#include "tictactoemainwidget.h"

TicTacToeMainWidget::TicTacToeMainWidget(Elements::PlayerType &player, QWidget *parent) :
    MainWidget(player, parent)
{
    init();
}

void TicTacToeMainWidget::init()
{
    m_gridView = new GameGridView(3, 3, this);
    m_panelView = new TicTacToeControlPanelView(this);

    m_centralLayout->addWidget(m_gridView);
    m_centralLayout->addWidget(m_panelView);

    connect(this, SIGNAL(resetBoard()), m_gridView, SLOT(emptyGrid()));
    connect(this, SIGNAL(updateGrid(const Grid*)), m_gridView, SLOT(updateGrid(const Grid*)));
    connect(m_gridView, SIGNAL(onSquareClicked(int,int)), this, SLOT(makeMove(int,int)));
    connect(m_panelView, SIGNAL(newGame()), this, SLOT(onNewGame()));
    connect(m_panelView, SIGNAL(nextMove()), this, SLOT(nextMove()));
    connect(m_panelView, SIGNAL(setHuman(Elements::PlayerType)), this, SLOT(onSwapInHuman(Elements::PlayerType)));
    connect(m_panelView, SIGNAL(setAI(Elements::PlayerType)), this, SLOT(onSwapInAI(Elements::PlayerType)));
    connect(m_panelView, SIGNAL(createAI(Elements::PlayerType,QString)), this, SLOT(onCreateAI(Elements::PlayerType,QString)));
    connect(m_panelView, SIGNAL(trainAI(Elements::PlayerType)), this, SLOT(onTrainAI(Elements::PlayerType)));
}

void TicTacToeMainWidget::makeMove(int x, int y)
{

#ifdef DEBUG_VIEW
    printLine5("TicTacToeMainWidget->makeMove(", x, ", ", y, ")");
#endif

    TicTacToeGrid *currentGrid = new TicTacToeGrid();

    //Copy the contents of the grid view into this grid object.
    m_gridView->getGameGrid(currentGrid);

    //Modify the game grid.  Change the given square to the current player.
    currentGrid->squares[x][y] = (Elements::GenericPieceType)m_currentPlayer;

#ifdef DEBUG_VIEW
    print("Complete grid:");
    for(int w = 0; w < 3; ++w)
    {
        for(int v = 0; v < 3; ++v)
        {
            printLine5(w, ", ", v, " = ", currentGrid->squares[w][v]);
        }
    }
#endif

    //Send out a signal that the board has changed.
    emit onMakeMove(currentGrid);
}

void TicTacToeMainWidget::onUpdateGrid(const Grid *newGrid)
{
#ifdef DEBUG_VIEW
    print("Updating grid!");
#endif
    emit updateGrid(newGrid);
}

void TicTacToeMainWidget::nextMove()
{
    //Get the current grid from the grid view.
    TicTacToeGrid *gameGrid = new TicTacToeGrid;
    //m_gridView has no concept of the concrete game grids; it only
    //knows its width and height.  The grid passed in must be
    //created before it is passed to getGameGrid.
    m_gridView->getGameGrid(gameGrid);

#ifdef DEBUG_VIEW
    print("TicTacToeMainWidget->nextMove()");
#endif

    //Emit the makeMove signal.
    emit onMakeMove(gameGrid);
}

///////////////////////////////
//TicTacToeControlPanelView//
///////////////////////////////
TicTacToeControlPanelView::TicTacToeControlPanelView(QWidget *parent)
    : ControlPanelView(parent)
{
    m_trainBoth = new QPushButton("Train Both AIs", this);
    m_centralLayout->addWidget(m_trainBoth, 6, 3, 1, 1);
    connect(m_trainBoth, SIGNAL(clicked()), this, SLOT(onTrainBothAIs()));
}
