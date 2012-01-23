#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "QBoxLayout.h"
#include "gamegridview.h"
#include "controlpanelview.h"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(PlayerType &currentPlayer, QWidget *parent = 0);

signals:
    void updateGrid(Grid *newGrid);
    void onMakeMove(Grid *newMove);

public slots:
    virtual void makeMove(int x, int y);

protected:
    using namespace Elements;

    GameGridView *m_gridView;
    ControlPanelView *m_panelView;
    QHBoxLayout *m_centralLayout;

    PlayerType &m_currentPlayer;
};

#endif // MAINWIDGET_H
