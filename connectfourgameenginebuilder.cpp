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

    //Set the correct directory for the database to load from.
    //Directory_slash is used to differentiate between Windows and Unix file systems.

    //Generate a data controller object and passit the objects it needs.
    dataController = new DataController();
    dataController->setRulesEngine(rulesEngine);
    dataController->setStatistics(statisticsData);
    dataController->setMoveTree(gameData);

    //Generate the game controller.
    gameController = new GameController();

    //Open the database from the correct directory.
    string directory = DIRECTORY_C4;
    directory += DIRECTORY_SLASH;
    gameController->setDatabaseDirectory(directory);

    gameController->setDataController(dataController);
    gameController->setAIBuilder(new C4NNBuilder());
    gameController->setAITrainer(new ConnectFourTrainer(C4_NUM_TRAINING_ITERATIONS, rulesEngine));
    gameController->setRulesEngine(rulesEngine);

    //Generate the main view.
    ConnectFourViewBuilder viewBuilder;
    mainWindow = viewBuilder.generateMainView();

    //Generate the view controller.
    viewController = new ViewController();
    viewController->setMainView(mainWindow);
    viewController->setGameController(gameController);
}
