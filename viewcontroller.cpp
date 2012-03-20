#include "viewcontroller.h"

ViewController::ViewController(QObject *parent) :
    QObject(parent), m_mainView(NULL), m_gameController(NULL), m_isGameOver(false)
{
}
void ViewController::setMainView(MainWindow *window)
{
    m_mainView = window;
    //Connect the updateGame signal with the main view's corresponding slot.
    //moveWasMade is meant to update the view.  makeMove will send a signal the other direction.
    connect(this, SIGNAL(updateGame(const Grid*)), m_mainView, SLOT(onMoveWasMade(const Grid*)));

    //Reset the game.
    connect(m_mainView, SIGNAL(startNewGame()), this, SLOT(newGame()));
    connect(this, SIGNAL(resetGame()), m_mainView, SLOT(onResetBoard()));

    //Connect the view's move signal with the nextMove slot.  nextMove will update the
    //gameController and come back with a new move.
    connect(m_mainView, SIGNAL(makeMove(const Grid*)), this, SLOT(nextMove(const Grid*)));

    connect(m_mainView, SIGNAL(swapInHuman(Elements::PlayerType)),
            this, SLOT(swapInHuman(Elements::PlayerType)));
    connect(m_mainView, SIGNAL(swapInAI(Elements::PlayerType)),
            this, SLOT(swapInAI(Elements::PlayerType)));
    connect(m_mainView, SIGNAL(buildAI(Elements::PlayerType,QString)),
            this, SLOT(buildAI(Elements::PlayerType,QString)));
    connect(m_mainView, SIGNAL(trainAI(Elements::PlayerType)),
            this, SLOT(trainAI(Elements::PlayerType)));
}

//Tell the game controller to save one of the neural network players.
void ViewController::saveAI(Elements::PlayerType playerID)
{
    m_gameController->saveNNPlayer(playerID);
}

//Tell the game controller
void ViewController::saveAI(Elements::PlayerType playerID, string fileName)
{
    m_gameController->saveNNPlayer(playerID, fileName);
}

void ViewController::nextMove(const Grid *nextMove)
{
#ifdef DEBUG_VIEW
    printLine("nextMove");
#endif
    //Only try to move if the game is not over.
    if(m_isGameOver)
        return;

    const Grid *newMove = m_gameController->attemptMove(nextMove);

    //First, check if newMove is NULL.
    if(newMove == NULL)
    {
#ifdef DEBUG_VIEW
        printLine("Invalid move.");
#endif
        emit(invalidMove());
    }

    //If the grid isn't NULL, then the move was valid.
    else
    {
#ifdef DEBUG_VIEW
        printLine("Valid move.");
#endif
        emit(updateGame(newMove));

        //See if the game is over.  If it is, emit the gameOver signal.
        if(isGameOver(newMove))
        {
#ifdef DEBUG_VIEW
            printLine("Game ending move");
#endif
            m_isGameOver = true;
            m_gameController->gameOver();
            emit gameOver();
        }
    }
}

void ViewController::newGame()
{
#ifdef DEBUG_VIEW
            printLine("newGame");
#endif
    m_isGameOver = false;
    m_gameController->resetGame();
    emit resetGame();
}
