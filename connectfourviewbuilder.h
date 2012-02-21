#ifndef CONNECTFOURVIEWBUILDER_H
#define CONNECTFOURVIEWBUILDER_H

#include "connectfourmainwidget.h"
#include "viewbuilder.h"

class ConnectFourViewBuilder : public ViewBuilder
{
public:
    ConnectFourViewBuilder() {}

    MainWindow *generateMainView();
};

#endif // CONNECTFOURVIEWBUILDER_H
