#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "mainwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    virtual void init() = 0;

    virtual void purge();

private:
    MainWidget *m_mainWidget;
    Elements::PlayerType currentPlayer;
};

#endif // MAINWINDOW_H
