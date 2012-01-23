#ifndef GAMEGRIDVIEW_H
#define GAMEGRIDVIEW_H

#include <QWidget>
#include "gridsquare.h"
#include "grid.h"

class GameGridView : public QWidget
{
    Q_OBJECT
public:
    explicit GameGridView(int width, int height, QWidget *parent = 0);
    void purge();
    ~GameGridView();

    //Copy grid squares to the gameGrid object.
    void getGameGrid(Grid *gameGrid);

signals:
    void onSquareClicked(int x, int y);

public slots:
    void squareClicked(int x, int y) { emit onSquareClicked(x, y); }
    void updateGrid(Grid *newGrid);

private:
    GridSquare ***m_squares;
    int m_height, m_width;
};

#endif // GAMEGRIDVIEW_H
