#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include<QString>

#include "mainwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void purge();

    void setMainWidget(MainWidget *widget);

    Elements::PlayerType &getCurrentPlayer() { return m_currentPlayer; }

signals:
    //
    void makeMove(const Grid *newMove);

    //Emit a signal to start a new game.
    void tellResetBoard();
    void startNewGame();

    //Emitted by invalidMove()
    void invalidMove();

    //After a move has been made.
    void validMove(const Grid *newMove);

    void swapInHuman(Elements::PlayerType playerID);
    void swapInAI(Elements::PlayerType playerID);
    void loadAIFromFile(Elements::PlayerType playerID, QString filename);
    void buildNewAI(Elements::PlayerType playerID, QString filename);
    void trainAI(Elements::PlayerType playerID);

public slots:
    //Should be called after the system has made a move and checked it.
    void onMoveWasMade(const Grid *newMove);

    //Should be called if makeMove() was not a valid move.
    void onInvalidMove() { emit(invalidMove()); }

    //Call to update the view to a new game.
    void onResetBoard();

    void switchCurrentPlayer();
    void setCurrentPlayer(Elements::PlayerType player) { m_currentPlayer = player; }

private slots:
    //Should be called to pass a new move to the game controller.
    void onMakeMove(Grid *newMove);

    //Emit a signal to start a new game.
    void onNewGame() { emit startNewGame(); }

    void onSwapInHuman(Elements::PlayerType playerID) { emit swapInHuman(playerID); }
    void onSwapInAI(Elements::PlayerType playerID) { emit swapInAI(playerID); }
    void onLoadAIFromFile(Elements::PlayerType playerID, QString filename) { emit loadAIFromFile(playerID, filename); }
    void onCreateAI(Elements::PlayerType playerID, QString filename) { buildNewAI(playerID, filename); }
    void onTrainAI(Elements::PlayerType playerID) { emit trainAI(playerID); }

private:
    //Create connections to the main widget.
    void init();

    MainWidget *m_mainWidget;
    Elements::PlayerType m_currentPlayer;
};

#endif // MAINWINDOW_H
