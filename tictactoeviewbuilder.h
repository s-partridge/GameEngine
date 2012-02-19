#ifndef TICTACTOEVIEWBUILDER_H
#define TICTACTOEVIEWBUILDER_H

#include "tictactoemainwidget.h"
#include "viewbuilder.h"

class TicTacToeViewBuilder : public ViewBuilder
{
public:
    TicTacToeViewBuilder() {}

    MainWindow *generateMainView();
};

#endif // TICTACTOEVIEWBUILDER_H
