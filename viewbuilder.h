#ifndef VIEWBUILDER_H
#define VIEWBUILDER_H

class ViewBuilder
{
public:
    ViewBuilder();

    virtual QWidget generateCentralWidget() = 0;
};

#endif // VIEWBUILDER_H
