#include <QtGui/QApplication>
#include "tictactoemainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TicTacToeMainWindow w;
    w.show();

    return a.exec();
}
