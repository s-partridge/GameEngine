#ifndef TICTACTOEMAINWIDGET_H
#define TICTACTOEMAINWIDGET_H

#include "mainwidget.h"
#include "tictactoegrid.h"

class TicTacToeMainWidget : public MainWidget
{
    Q_OBJECT
public:
    explicit TicTacToeMainWidget(Elements::PlayerType &player, QWidget *parent = 0);

    void init();
signals:

public slots:
    void makeMove(int x, int y);
    void onUpdateGrid(const Grid *newGrid);

protected slots:
    void nextMove();
};

class TicTacToeControlView : public ControlPanelView
{
public:
    TicTacToeControlView(QWidget *parent = 0) : ControlPanelView(parent) {}
};

#endif // TICTACTOEMAINWIDGET_H
