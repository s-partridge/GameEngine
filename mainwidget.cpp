#include "mainwidget.h"

MainWidget::MainWidget(PlayerType &currentPlayer, QWidget *parent) :
    QWidget(parent), m_currentPlayer(currentPlayer)
{
    m_gridView = NULL;
    m_panelView = NULL;
    m_centralLayout = new QHBoxLayout(this);
}

void MainWidget::purge()
{
    if(m_gridView != NULL)
        delete m_gridView;
    if(m_panelView != NULL)
        delete m_panelView;

    if(m_centralLayout != NULL)
        delete m_centralLayout;
}
