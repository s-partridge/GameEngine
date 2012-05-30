#include "connectfourmainwidget.h"

ConnectFourMainWidget::ConnectFourMainWidget(Elements::PlayerType &player, QWidget *parent) :
    MainWidget(player, parent)
{
    init();
}

void ConnectFourMainWidget::init()
{
    m_gridView = new GameGridView(C4_WIDTH, C4_HEIGHT, this);
    m_panelView = new ConnectFourControlPanelView(this);

    m_gridView->setSquareSize(50, 50, 100, 100);

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

void ConnectFourMainWidget::makeMove(int x, int y)
{

#ifdef C4DEBUG_VIEW
    printLine5("ConnectFourMainWidget->makeMove(", x, ", ", y, ")");
#endif

    Grid *currentGrid = new ConnectFourGrid();

    //Copy the contents of the grid view into this grid object.
    m_gridView->getGameGrid(currentGrid);

    //Modify the game grid.  Change the given square to the current player.
    currentGrid->squares[x][y] = (Elements::GenericPieceType)m_currentPlayer;

    //Make sure that there are no blank spaces between the move and the bottom of the stack.
    pullPieceDown(x, *currentGrid);
#ifdef C4DEBUG_VIEW
    print("Complete grid:");
    for(int w = 0; w < C4_WIDTH; ++w)
    {
        for(int v = 0; v < C4_HEIGHT; ++v)
        {
            printLine5(w, ", ", v, " = ", currentGrid->squares[w][v]);
        }
    }
#endif

    //Send out a signal that the board has changed.
    emit onMakeMove(currentGrid);
}

void ConnectFourMainWidget::pullPieceDown(int x, Grid &grid)
{
    int moveToIndex = -1;
    int nextFullSquare = -1;

    //Find the first empty square.
    for(int y = C4_HEIGHT - 1; y >= 0; --y)
    {
        //Break if an empty square is found.
        if(grid.squares[x][y] == Elements::EMPTY)
        {
            moveToIndex = y;
            break;
        }
    }

    //Find the first full square above the empty one.
    for(int y = 0; y < moveToIndex; ++y)
    {
        if(grid.squares[x][y] != Elements::EMPTY)
        {
            nextFullSquare = y;
            break;
        }
    }

    //This will only be true if an empty square is found below a full one.
    //By default, both values are equal.  A square can't be empty and !empty,
    //So there's no way nextFullSquare and moveToIndex can be equal.
    //NextFullSquare also cannot be greater than moveToIndex because the second
    //loop stops on moveToIndex.
    if(nextFullSquare != -1 && nextFullSquare < moveToIndex)
    {
        grid.squares[x][moveToIndex] = grid.squares[x][nextFullSquare];
        grid.squares[x][nextFullSquare] = Elements::EMPTY;
    }
}

void ConnectFourMainWidget::onUpdateGrid(const Grid *newGrid)
{
#ifdef C4DEBUG_VIEW
    print("Updating grid!");
#endif
    emit updateGrid(newGrid);
}

void ConnectFourMainWidget::nextMove()
{
    //Get the current grid from the grid view.
    Grid *gameGrid = new ConnectFourGrid();
    //m_gridView has no concept of the concrete game grids; it only
    //knows its width and height.  The grid passed in must be
    //created before it is passed to getGameGrid.
    m_gridView->getGameGrid(gameGrid);

#ifdef C4DEBUG_VIEW
    print("TicTacToeMainWidget->nextMove()");
#endif

    //Emit the makeMove signal.
    emit onMakeMove(gameGrid);
}

///////////////////////////////
//ConnectFourControlPanelView//
///////////////////////////////
ConnectFourControlPanelView::ConnectFourControlPanelView(QWidget *parent)
    : ControlPanelView(parent)
{
    m_trainBoth = new QPushButton("Train Both AIs", this);
    m_centralLayout->addWidget(m_trainBoth, 6, 3, 1, 1);
    connect(m_trainBoth, SIGNAL(clicked()), this, SLOT(onTrainBothAIs()));
}
