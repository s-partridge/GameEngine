#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <QObject>
#include <QString>

#include <string>

#include "mainwindow.h"
#include "gamecontroller.h"
#include "grid.h"

class ViewController : public QObject
{
    Q_OBJECT
public:
    explicit ViewController(QObject *parent = 0);

    void setGameController(GameController *controller) { m_gameController = controller; }
    void setMainView(MainWindow *window);

    void saveAI(Elements::PlayerType playerID);
    void saveAI(Elements::PlayerType playerID, string fileName);

signals:
    void updateGame(const Grid *nextMove);
    void clearTextFields();
    void resetGame();
    void gameOver();
    void invalidMove();

public slots:
    void nextMove(const Grid *nextMove);

private slots:
    void swapInHuman(Elements::PlayerType playerID) { m_gameController->swapAIForHuman(playerID); }
    void swapInAI(Elements::PlayerType playerID) { m_gameController->swapHumanForAI(playerID); }
    void buildAI(Elements::PlayerType playerID, QString filename) { m_gameController->createNNPlayer(playerID, filename.toStdString()); }
    void trainAI(Elements::PlayerType playerID) { m_gameController->trainAI(playerID); }
    void newGame();

private:
    bool isGameOver(const Grid *move) { return m_gameController->isGameOver(move); }

    MainWindow *m_mainView;
    GameController *m_gameController;
    bool m_isGameOver;
};

#endif // VIEWCONTROLLER_H
