#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_gridView = NULL;
    m_panelView = NULL;
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
