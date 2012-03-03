#define SHOW_GUI
//#define QUICK_TEST

#include <QtGui/QApplication>

#include "tictactoegameenginebuilder.h"
#include "connectfourgameenginebuilder.h"
#include "macros.h"
#include "movetablebuilder.h"
#include "unitTests.h"

void trainXOR();

void setSpecificWeights(NeuralNetwork *network);

int main(int argc, char *argv[])
{
#ifdef SHOW_GUI
    QApplication a(argc, argv);

    MainWindow *mainWindow;
    ViewController *viewController;
    GameController *gameController;
    DataController *dataController;
    StatisticsData *statisticsData;
    GameData *gameData;

    print("Generating objects\n");

    //TicTacToeGameEngineBuilder gameEngineBuilder;
    ConnectFourGameEngineBuilder gameEngineBuilder;
    gameEngineBuilder.generateGameEngine(mainWindow, viewController, gameController,
                                         dataController, statisticsData, gameData);

    print("Finished generating objects\n");

    mainWindow->show();

    print("Showing main window\n");

    static int retVal = a.exec();

    delete gameController;

    return retVal;
#endif

#ifdef QUICK_TEST
    //RulesEngine *re = new TicTacToeRulesEngine();
    //testC4RulesEngine();

    RulesEngine *engine = new ConnectFourRulesEngine();
    Grid *first = engine->createGameSpecificGrid();

    //A starting state for a relatively small move tree.
    first->squares[0][0] = Elements::EMPTY;
    first->squares[0][1] = Elements::TYPE2;
    first->squares[0][2] = Elements::TYPE2;
    first->squares[0][3] = Elements::TYPE2;

    first->squares[1][0] = Elements::EMPTY;
    first->squares[1][1] = Elements::TYPE1;
    first->squares[1][2] = Elements::TYPE1;
    first->squares[1][3] = Elements::TYPE1;

    first->squares[2][0] = Elements::EMPTY;
    first->squares[2][1] = Elements::TYPE2;
    first->squares[2][2] = Elements::TYPE2;
    first->squares[2][3] = Elements::TYPE2;

    first->squares[3][0] = Elements::TYPE2;
    first->squares[3][1] = Elements::TYPE1;
    first->squares[3][2] = Elements::TYPE1;
    first->squares[3][3] = Elements::TYPE1;

    first->squares[4][0] = Elements::TYPE1;
    first->squares[4][1] = Elements::TYPE2;
    first->squares[4][2] = Elements::TYPE2;
    first->squares[4][3] = Elements::TYPE2;

    first->squares[5][0] = Elements::TYPE2;
    first->squares[5][1] = Elements::TYPE1;
    first->squares[5][2] = Elements::TYPE1;
    first->squares[5][3] = Elements::TYPE1;

    cout << "Current grid: " << *first << endl;
    cout << "Game state:   " << engine->testBoard(first) << endl;

   // BoardState *newState = new BoardState(first, NULL, Elements::PLAYER_1, engine);

  //  MoveTableBuilder builder;
  //  builder.createMoveTableFile(newState, "/Users/samuel/Documents/c4.mt", engine);

    delete engine;
    //delete first;
    delete first;

    /*
    Grid *first = new TicTacToeGrid();
    Grid *empty = new TicTacToeGrid();

    BoardState *tree = new BoardState(empty, NULL, Elements::PLAYER_1, re, 2);

    first->squares[0][1] = Elements::TYPE1;

    if(*empty == *(tree->getState(first)->getCurrentGrid()))
    {
        cout << "YAY!" << endl;
    }

    cout << Elements::EMPTY << " " << Elements::TYPE1 << " " << endl;

    Elements::GenericPieceType piece = Elements::EMPTY;
    Elements::PlayerType player = Elements::PLAYER_1;
    piece = (Elements::GenericPieceType)player;

    cout << piece << " " << player << endl;

//    trainXOR();

    MoveTableBuilder builder;

 /*   Grid *first = new TicTacToeGrid();
  first->squares[0][0] = Elements::TYPE2;
    first->squares[1][0] = Elements::TYPE2;
    first->squares[2][0] = Elements::TYPE1;

    first->squares[0][1] = Elements::TYPE2;
    first->squares[1][1] = Elements::EMPTY;
    first->squares[2][1] = Elements::EMPTY;

    first->squares[0][2] = Elements::TYPE1;
    first->squares[1][2] = Elements::TYPE1;
    first->squares[2][2] = Elements::EMPTY;
 /*   //Build complete tree.
    //BoardState *tree = new BoardState(first, NULL, Elements::PLAYER_1, re, 12);
    //builder.createMoveTableFile(tree, "/Users/samuel/Documents/test.mt", re);

    map<string, string> hashMap;

    Elements::PlayerType currentPlayer = Elements::PLAYER_1;

    builder.generateHashTable("/Users/samuel/Documents/test.mt", hashMap);

    while(re->testBoard(first) == Elements::NORMAL)
    {
       // int x[1] = { 100 };
        sleep(1);

        string test = first->toString();
        string result = hashMap[test];
        int index = atoi(result.c_str());

        if(currentPlayer == Elements::PLAYER_1)
        {
            first->squares[index / 3][index % 3] = Elements::TYPE1;
            currentPlayer = Elements::PLAYER_2;
        }
        else
        {
            first->squares[index / 3][index % 3] = Elements::TYPE2;
            currentPlayer = Elements::PLAYER_1;
        }

        cout << test << ": " << index << endl;

        //cout << *first << "\n";
    }*/

#endif
}

void trainXOR()
{
    int numInputs = 2;
    int numOutputs = 1;
    int numHiddenLayers = 1;

    int *numHidden = new int[1];
    double *momentums = new double[2];
    double *learnRates = new double[2];

    numHidden[0] = 3;
    momentums[0] = MOMENTUM;
    momentums[1] = MOMENTUM;
    learnRates[0] = LEARN_RATE;
    learnRates[1] = LEARN_RATE;

    double **inputs = new double*[4];
    for(int x = 0; x < 4; ++x)
    {
        inputs[x] = new double[2];
    }

    inputs[0][0] = 0;
    inputs[0][1] = 0;
    inputs[1][0] = 0;
    inputs[1][1] = 1;
    inputs[2][0] = 1;
    inputs[2][1] = 0;
    inputs[3][0] = 1;
    inputs[3][1] = 1;

    double **expected = new double*[4];
    for(int x = 0; x < 4; ++x)
    {
        expected[x] = new double[1];
    }

    expected[0][0] = 0;
    expected[1][0] = 1;
    expected[2][0] = 1;
    expected[3][0] = 0;

    double *outputs = new double[1];

    NeuralNetwork *net = new NeuralNetwork("xor.nnp", numInputs, numOutputs, numHiddenLayers, numHidden, momentums, learnRates);

    ActivationFunctor *newFunctor = new Sigmoid();

    setSpecificWeights(net);

    net->setLayerActivation(0, newFunctor);
    net->setLayerActivation(1, newFunctor);

    for(int x = 0; x < 1; ++x)
    {
        for(int y = 0; y < 4; ++y)
        {
            cout << "\tinputs: " << inputs[y][0] << ", " << inputs[y][1];
            net->getResults(inputs[y], outputs);
            cout << "\toutputs: " << outputs[0];// ", " << outputs[1];
            net->backpropagate(outputs, expected[y]);
            cout << "\texpected: " << expected[y][0] << "\n";// << expected[y][1] << "\n";
        }
    }

    delete net;
}

void setSpecificWeights(NeuralNetwork *network)
{
    //The hidden layer.
    double **weights = new double*[3];

    for(int x = 0; x < 3; ++x)
    {
        weights[x] = new double[3];
    }

    //Neuron 1
    weights[0][0] = -0.544684;
    weights[0][1] = -0.507625;
    weights[0][2] = -0.654222;

    //neuron 2
    weights[1][0] = -0.504912;
    weights[1][1] = -0.556793;
    weights[1][2] = -0.514353;

    //Neuron 3
    weights[2][0] = -0.732203;
    weights[2][1] = -0.63855;
    weights[2][2] = -0.612833;

#ifdef DEBUG_NEURALNETWORK
    printLine("Setting hidden weights");
#endif
    network->setLayerWeights(0, weights);

    for(int x = 0; x < 3; ++x)
    {
        delete [] weights[x];
    }
    delete [] weights;

    //The output layer.
    weights = new double*[1];

    weights[0] = new double[4];

    weights[0][0] = -0.892227;
    weights[0][1] = -0.657057;
    weights[0][2] = -0.657068;
    weights[0][3] = -0.846713;

#ifdef DEBUG_NEURALNETWORK
    printLine("Setting output weights");
#endif
    network->setLayerWeights(1, weights);

    delete [] weights[0];
    delete [] weights;
}
