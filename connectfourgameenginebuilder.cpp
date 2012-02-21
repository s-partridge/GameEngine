#include "connectfourgameenginebuilder.h"

void ConnectFourGameEngineBuilder::generateGameEngine(MainWindow *&mainWindow, ViewController *&viewController,
                                                      GameController *&gameController, DataController *&dataController,
                                                      StatisticsData *&statisticsData, GameData *&gameData) const
{
    RulesEngine *rulesEngine = new ConnectFourRulesEngine();

    //Generate a new object for game statistics.
    statisticsData = new StatisticsData();

    //Generate a new object for the connect four move tree.
    gameData = new ConnectFourGameData();
    gameData->init(rulesEngine);

    //Generate a data controller object and passit the objects it needs.
    dataController = new DataController();
    dataController->setRulesEngine(rulesEngine);
    dataController->setStatistics(statisticsData);
    dataController->setMoveTree(gameData);

    //Generate the game controller.
    gameController = new GameController();
    gameController->setDataController(dataController);
    gameController->setAIBuilder(new C4NNBuilder());
    gameController->setAITrainer(new ConnectFourTrainer(NUM_TRAINING_ITERATIONS, rulesEngine));
    gameController->setRulesEngine(rulesEngine);

    //Generate the main view.
    ConnectFourViewBuilder viewBuilder;
    mainWindow = viewBuilder.generateMainView();

    //Generate the view controller.
    viewController = new ViewController();
    viewController->setMainView(mainWindow);
    viewController->setGameController(gameController);
}
