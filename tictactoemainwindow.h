#ifndef TICTACTOEMAINWINDOW_H
#define TICTACTOEMAINWINDOW_H

#include "mainwidget.h"
#include "tictactoegrid.h"

class TicTacToeMainWidget : public MainWidget
{
    Q_OBJECT
public:
    explicit TicTacToeMainWidget(QObject *parent = 0);

    void init();
signals:

public slots:

};

class TicTacToeControlView : public ControlPanelView
{

};

#endif // TICTACTOEMAINWINDOW_H
