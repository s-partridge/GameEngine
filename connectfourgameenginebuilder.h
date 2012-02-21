#ifndef CONNECTFOURGAMEENGINEBUILDER_H
#define CONNECTFOURGAMEENGINEBUILDER_H

#include "gameenginebuilder.h"
#include "connectfourgamedata.h"
#include "connectfourgrid.h"
#include "connectfourmainwidget.h"
#include "connectfourrulesengine.h"
#include "connectfourtrainer.h"
#include "connectfourviewbuilder.h"
#include "c4nnbuilder.h"

class ConnectFourGameEngineBuilder : public GameEngineBuilder
{
public:
    ConnectFourGameEngineBuilder() {}

    void generateGameEngine(MainWindow *&mainWindow, ViewController *&viewController,
                            GameController *&gameController, DataController *&dataController,
                            StatisticsData *&statisticsData, GameData *&gameData) const;
};

#endif // CONNECTFOURGAMEENGINEBUILDER_H
