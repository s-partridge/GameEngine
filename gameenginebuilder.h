///////////////////////////////////////////////////////////////////////////////
//
//  file:    gameenginebuilder.h
//  author:  Samuel Partridge
//  project: Game Engine
//  purpose: Abstract factory used to generate a game specific view, neuralNet
//           Player builder, trainer, and set of grids.  This will return
//           references to each object listed above.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef GAMEENGINEBUILDER_H
#define GAMEENGINEBUILDER_H

#include "mainwindow.h"
#include "viewcontroller.h"
#include "gamecontroller.h"
#include "datacontroller.h"
#include "statisticsdata.h"
#include "gamedata.h"

class GameEngineBuilder
{
public:
    GameEngineBuilder() {}

    virtual void generateGameEngine(MainWindow *&mainWindow, ViewController *&viewController,
                            GameController *&gameController, DataController *&dataController,
                            StatisticsData *&statisticsData, GameData *&gameData) const = 0;
};

#endif // GAMEENGINEBUILDER_H
