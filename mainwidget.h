#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "controlpanelview.h"
#include "gamegridview.h"
#include <QWidget>
#include <QBoxLayout>

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(Elements::PlayerType &currentPlayer, QWidget *parent = 0);

    void purge();

signals:
    //Used to replace the data held in the gridView.
    void updateGrid(const Grid *newGrid);

    //Empties the game grid.
    void resetBoard();

    //Emitted when a change is made to the game board by the user.
    void onMakeMove(Grid *newMove);

    //Control panel related signals
    void swapInHuman(Elements::PlayerType player);
    void swapInAI(Elements::PlayerType player);
    void createAI(Elements::PlayerType player, QString filename);
    void newGame();
    void trainAI(Elements::PlayerType player);

public slots:
    virtual void makeMove(int x, int y) = 0;
    virtual void onResetBoard() { emit resetBoard(); }
    virtual void onUpdateGrid(const Grid *newGrid) = 0;

protected:
    GameGridView *m_gridView;
    ControlPanelView *m_panelView;
    QHBoxLayout *m_centralLayout;

    Elements::PlayerType &m_currentPlayer;

protected slots:
    void onCreateAI(Elements::PlayerType player, QString filename) { emit createAI(player, filename); }
    void onSwapInHuman(Elements::PlayerType player) { emit swapInHuman(player); }
    void onSwapInAI(Elements::PlayerType player) { emit swapInAI(player); }
    void onNewGame() { emit newGame(); }
    void onTrainAI(Elements::PlayerType player) { emit trainAI(player); }

    //Used with the nextMove button on the controlPanelView.
    //Allows an AI player to make a move.
    virtual void nextMove() = 0;
};

#endif // MAINWIDGET_H
