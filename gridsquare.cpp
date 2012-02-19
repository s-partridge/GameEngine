#include "gridsquare.h"

GridSquare::GridSquare(int xCoord, int yCoord, QWidget *parent) :
    m_xCoord(xCoord), m_yCoord(yCoord), QPushButton(parent)
{
    setMinimumSize(100, 100);
    setMaximumSize(100, 100);
    //Connect a mouse click to the internal click slot.
    connect(this, SIGNAL(released()), this, SLOT(onButtonClick()));
}

