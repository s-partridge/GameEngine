#ifndef TICTACTOEGAMEENGINEBUILDER_H
#define TICTACTOEGAMEENGINEBUILDER_H

#include "gameenginebuilder.h"
#include "tictactoegamedata.h"
#include "tictactoegrid.h"
#include "tictactoemainwidget.h"
#include "tictactoerulesengine.h"
#include "tictactoetrainer.h"
#include "tictactoeviewbuilder.h"

class TicTacToeGameEngineBuilder : public GameEngineBuilder
{
public:
    TicTacToeGameEngineBuilder() {}

    void generateGameEngine(MainWindow *&mainWindow, ViewController *&viewController,
                            GameController *&gameController, DataController *&dataController,
                            StatisticsData *&statisticsData, GameData *&gameData) const;
};

#endif // TICTACTOEGAMEENGINEBUILDER_H
