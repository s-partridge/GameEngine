#include "gridsquare.h"

GridSquare::GridSquare(QObject *parent) :
    m_xCoord(xCoord), m_yCoord(yCoord), QPushButton(parent)
{
    //Connect a mouse click to the internal click slot.
    connect(this, SIGNAL(released()), this, SLOT(onButtonClick()));
}

