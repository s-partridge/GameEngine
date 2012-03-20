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

class TicTacToeControlPanelView : public ControlPanelView
{
    Q_OBJECT
public:
    TicTacToeControlPanelView(QWidget *parent = 0);

protected:
    QPushButton *m_trainBoth;

protected slots:
    void onTrainBothAIs() { emit trainAI(Elements::NONE); }
};

#endif // TICTACTOEMAINWIDGET_H
