#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <QObject>
#include <QString>
#include <string>
#include "mainwindow.h"
#include "gamecontroller.h"
#include "grid.h"
#include "trainer.h"

class ViewController : public QObject
{
    Q_OBJECT
public:
    explicit ViewController(QObject *parent = 0);
    void newGame();
    void tellMakeNextMove(const Grid *nextMove);
    void gameOver();

    void swapInHuman(int playerID);
    void swapInAI(int playerID);
    void loadAIFromFile(int playerID, string fileName);
    void buildNewAI(int playerID, string fileName);
    void trainAI(int playerID, Trainer *trainer);

    void saveAI(int playerID);
    void saveAI(int playerID, string fileName);

signals:
    void updateGame(const Grid *nextMove);
    void clearTextFields();
    void resetGame();

public slots:

private:
    MainWindow *mainView;
    GameController *gameController;
};

#endif // VIEWCONTROLLER_H
