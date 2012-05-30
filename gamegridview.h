#ifndef GAMEGRIDVIEW_H
#define GAMEGRIDVIEW_H

#include <QWidget>
#include <QLayout>

#include "gridsquare.h"
#include "grid.h"
#include "macros.h"

class GameGridView : public QWidget
{
    Q_OBJECT
public:
    explicit GameGridView(int width, int height, QWidget *parent = 0);
    void purge();
    ~GameGridView();

    //Copy grid squares to the gameGrid object.
    void getGameGrid(Grid *gameGrid);
    void setSquareSize(int minX, int minY, int maxX, int maxY);

signals:
    void onSquareClicked(int x, int y);

public slots:
    //
    void updateGrid(const Grid *newGrid);
    void emptyGrid();

protected:
    virtual void updateText(const Grid *newGrid);

private slots:
    //Used when a button in m_squares has been pressed.
    void squareClicked(int x, int y);

private:
    GridSquare ***m_squares;
    QGridLayout *m_centralLayout;
    int m_width, m_height;
};

#endif // GAMEGRIDVIEW_H
