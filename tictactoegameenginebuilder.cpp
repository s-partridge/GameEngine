#include "tictactoegameenginebuilder.h"

void TicTacToeGameEngineBuilder::generateGameEngine(MainWindow *&mainWindow, ViewController *&viewController,
                                                    GameController *&gameController, DataController *&dataController,
                                                    StatisticsData *&statisticsData, GameData *&gameData) const
{
    RulesEngine *rulesEngine = new TicTacToeRulesEngine();

    //Generate a new object for game statistics.
    statisticsData = new StatisticsData();

    //Generate a new object for the tic tac toe move tree.
    gameData = new TicTacToeGameData();
    gameData->init(rulesEngine);

    //Generate a data controller object and pass it the objects it needs.
    dataController = new DataController();
    dataController->setRulesEngine(rulesEngine);
    dataController->setStatistics(statisticsData);
    dataController->setMoveTree(gameData);

    //Set the correct directory for the database to load from.
    //Directory_slash is used to differentiate between Windows and Unix file systems.

    //Generate the game controller.
    gameController = new GameController();

    //Open the database from the correct directory.
    string directory = DIRECTORY_TTT;
    directory += DIRECTORY_SLASH;
    gameController->setDatabaseDirectory(directory);

    gameController->setDataController(dataController);
    gameController->setAITrainer(new TicTacToeTrainer(NUM_TRAINING_ITERATIONS, rulesEngine));
    gameController->setAIBuilder(new TTTNNBuilder());
    gameController->setRulesEngine(rulesEngine);

    //Generate the main view.
    TicTacToeViewBuilder viewBuilder;
    mainWindow = viewBuilder.generateMainView();

    //Generate the view controller.
    viewController = new ViewController();
    viewController->setMainView(mainWindow);
    viewController->setGameController(gameController);
}
