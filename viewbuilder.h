#ifndef VIEWBUILDER_H
#define VIEWBUILDER_H

#include "mainwindow.h"

class ViewBuilder
{
public:
    ViewBuilder() {}

    virtual MainWindow *generateMainView() = 0;
};

#endif // VIEWBUILDER_H
