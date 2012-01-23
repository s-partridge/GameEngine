
#ifndef GRIDSQUARE_H
#define GRIDSQUARE_H

#include <QPushButton>
#include "datatypes.h"

class GridSquare : public QPushButton
{
    Q_OBJECT
public:
    explicit GridSquare(int xCoord, int yCoord, QObject *parent = 0);

    GenericPieceType getPieceType() { return m_value; }
    void changePieceType(GenericPieceType value) { m_value = value; }
    void changeText(QString string) { setText(string); }

signals:
    //Emitted when the button is clicked, sending out the button's
    //grid position.
    void buttonClicked(int x, int y);

private slots:
    //Emits the buttonClicked signal, sending out the grid coordinates.
    void onButtonClick() { emit buttonClicked(m_xCoord, m_yCoord); }

private:
    using namespace Elements;

    GenericPieceType m_value;

    int m_xCoord, m_yCoord;
};

#endif // GRIDSQUARE_H
