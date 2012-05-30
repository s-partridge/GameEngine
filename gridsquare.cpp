#include "gridsquare.h"

GridSquare::GridSquare(int xCoord, int yCoord, QWidget *parent) :
    QPushButton(parent), m_xCoord(xCoord), m_yCoord(yCoord)
{
    setMinimumSize(50, 50);
    setMaximumSize(100, 100);
    //Connect a mouse click to the internal click slot.
    connect(this, SIGNAL(released()), this, SLOT(onButtonClick()));
}

