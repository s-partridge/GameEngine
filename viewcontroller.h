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

    void setGameController(GameController *controller) { m_gameController = controller; }
    void setMainView(MainWindow *window) { m_mainView = window; }

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

//public slots:

private:
    MainWindow *m_mainView;
    GameController *m_gameController;
};

#endif // VIEWCONTROLLER_H
