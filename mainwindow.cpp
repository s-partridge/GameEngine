#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_mainWidget = NULL;
    m_currentPlayer = Elements::PLAYER_1;
}

MainWindow::~MainWindow()
{
    purge();
}

void MainWindow::purge()
{
    if(m_mainWidget != NULL)
        delete m_mainWidget;
}

void MainWindow::setMainWidget(MainWidget *widget)
{
    if(m_mainWidget != NULL)
        delete m_mainWidget;

    m_mainWidget = widget;

    setCentralWidget(m_mainWidget);

    init();
}

void MainWindow::onResetBoard()
{
#ifdef DEBUG_VIEW
    print("Resetting game board.");
#endif
    setCurrentPlayer(Elements::PLAYER_1);
    emit tellResetBoard();
}
void MainWindow::switchCurrentPlayer()
{
    if(m_currentPlayer == Elements::PLAYER_1)
        m_currentPlayer = Elements::PLAYER_2;
    else
        m_currentPlayer = Elements::PLAYER_1;
}

void MainWindow::onMoveWasMade(const Grid *newMove)
 {
#ifdef DEBUG_VIEW
    print("mainWindow->onMoveWasMade");
#endif
    switchCurrentPlayer();
    emit validMove(newMove);
 }
void MainWindow::onMakeMove(Grid *newMove)
{
#ifdef DEBUG_VIEW
    printLine2("mainWindow->onMakeMove() with new grid at address: ", newMove);
#endif
    emit makeMove(newMove);

}
void MainWindow::init()
{
    //Window to widget
    connect(this, SIGNAL(validMove(const Grid*)), m_mainWidget, SLOT(onUpdateGrid(const Grid*)));
    connect(this, SIGNAL(tellResetBoard()), m_mainWidget, SLOT(onResetBoard()));

    //Widget to window
    connect(m_mainWidget, SIGNAL(onMakeMove(Grid*)), this, SLOT(onMakeMove(Grid *)));
    connect(m_mainWidget, SIGNAL(swapInHuman(Elements::PlayerType)), this, SLOT(onSwapInHuman(Elements::PlayerType)));
    connect(m_mainWidget, SIGNAL(swapInAI(Elements::PlayerType)), this, SLOT(onSwapInAI(Elements::PlayerType)));
    connect(m_mainWidget, SIGNAL(createAI(Elements::PlayerType,QString)), this, SLOT(onCreateAI(Elements::PlayerType,QString)));
    connect(m_mainWidget, SIGNAL(trainAI(Elements::PlayerType)), this, SLOT(onTrainAI(Elements::PlayerType)));
    connect(m_mainWidget, SIGNAL(newGame()), this, SLOT(onNewGame()));
}
