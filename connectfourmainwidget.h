#ifndef CONNECTFOURMAINWIDGET_H
#define CONNECTFOURMAINWIDGET_H

#include "mainwidget.h"
#include "connectfourgrid.h"
#include "c4constants.h"

class ConnectFourMainWidget : public MainWidget
{
    Q_OBJECT
public:
    explicit ConnectFourMainWidget(Elements::PlayerType &player, QWidget *parent = 0);


    void init();
signals:

public slots:
    void makeMove(int x, int y);
    void onUpdateGrid(const Grid *newGrid);

protected slots:
    void nextMove();

public:
    //Pieces can only be placed directly on top of each other.
    void pullPieceDown(int x, Grid &grid);
};

//May subclass ControlPanelView later on.  For now, the default implementation is good enough.
class ConnectFourControlPanelView : public ControlPanelView
{
    Q_OBJECT
public:
    ConnectFourControlPanelView(QWidget *parent);

protected:
    QPushButton *m_trainBoth;

protected slots:
    void onTrainBothAIs() { emit trainAI(Elements::NONE); }
};

#endif // CONNECTFOURMAINWIDGET_H
