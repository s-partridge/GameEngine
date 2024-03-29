#define SHOW_GUI
//#define QUICK_TEST

#define TICTACTOE
//#define CONNECTFOUR

#include <QtGui/QApplication>

#include "tictactoegameenginebuilder.h"
#include "connectfourgameenginebuilder.h"
#include "macros.h"
#include "movetablebuilder.h"
#include "unitTests.h"
#include "gamedatabase.h"

void trainXOR();

void setSpecificWeights(NeuralNetwork *network);

void tryWriteFiles(string filename);

int main(int argc, char **argv)
{
#ifdef SHOW_GUI
    QApplication a(argc, argv);

    MainWindow *mainWindow;
    ViewController *viewController;
    GameController *gameController;
    DataController *dataController;
    StatisticsData *statisticsData;
    GameData *gameData;

#ifdef TICTACTOE
    TicTacToeGameEngineBuilder gameEngineBuilder;
#else
    #ifdef CONNECTFOUR
    ConnectFourGameEngineBuilder gameEngineBuilder;
    #endif
#endif
    gameEngineBuilder.generateGameEngine(mainWindow, viewController, gameController,
                                         dataController, statisticsData, gameData);

    print("Finished generating objects\n");

    //gameController->createNNPlayer(Elements::PLAYER_1, "r.nnp");
    //gameController->trainAI(Elements::PLAYER_1);

    mainWindow->show();

    print("Showing main window\n");

    static int retVal = a.exec();

    delete mainWindow;
    delete viewController;
    delete gameController;
    delete dataController;
    delete statisticsData;
    delete gameData;

    return retVal;
#endif

#ifdef QUICK_TEST
    trainXOR();
   /* std::string game = "0101010";

    ConnectFourRulesEngine *re = new ConnectFourRulesEngine;
    Grid *newGrid = re->createGameSpecificGrid();

    BoardState *root = new BoardState(newGrid, NULL, Elements::PLAYER_1, re);
    BoardState *last = root->fromString(game, 0, re);

    for(int x = 0; x < C4_HEIGHT; ++x)
    {
        for(int y = 0; y < C4_WIDTH; ++y)
        {
            cout << last->getCurrentGrid()->squares[y][x] << ' ';
        }
        cout << '\n';
    }
    cout << endl;

    GameDatabase &db = GameDatabase::getDatabase();

    db.storeGame(last);
    db.storeGame(last);
    db.storeGame(last);

    cout << db.size() << ' ';
    db.removeDuplicateGames();
    cout << db.size() << '\n';

    delete root;

    GameDatabase &db2 = GameDatabase::getDatabase();
    int retVal = db2.loadGameFromIndex(root, last, 0, re);

    if(retVal == 0)
    {
        for(int x = 0; x < C4_HEIGHT; ++x)
        {
            for(int y = 0; y < C4_WIDTH; ++y)
            {
                cout << last->getCurrentGrid()->squares[y][x] << ' ';
            }
            cout << '\n';
        }
        cout << endl;
    }
    else
    {
        cerr << "Invalid game index\n";
    }


    db.setDBFile("c4test.db");
    db.storeDBToFile();

    retVal = db.generateDBFromFile();

    cout << db.size() << '\n';

    for(int x = 0; x < db.size(); ++x)
    {
        db.loadStringFromIndex(x, game);
        cout << game << '\n';
    }

    retVal = db.loadStringFromIndex(0, game);
    cout << game.length() << endl;

    if(retVal != 0)
    {
        switch(retVal)
        {
        case ERROR_EMPTY:
            cerr << "File empty" << endl;
            break;
        case ERROR_BAD_INDEX:
            cerr << "Database index requested is not valid\n" << endl;
            break;
        default:
            cerr << "Unknown error occurred\n";
        }
    }*/
#endif
}


void tryWriteFiles(string filename)
{
    fstream io;
    ifstream i;
    ofstream o;

    io.open(filename.c_str(), ios::in | ios::out);

    if(io.is_open())
    {
        cout << "File exists\n";
        io.close();
        io.open(filename.c_str(), ios::out | ios::binary | ios::app);
    }
    else
    {
        cout << "File does not exist.  What do I do???\n";
        io.open(filename.c_str(), ios::in | ios::out | ios::binary | ios::trunc);
    }
    string str = "abc.";
    string output = "";

    io << str;
    /*
    for(int x = 0; x < str.length(); ++x)
    {
        io.write((char *)&str[x], sizeof(char));
    }
*/
    io.close();

    i.open(filename.c_str(), ios::binary);

    i >> output;

    cout << output << endl;
}

void trainXOR()
{
    int numInputs = 2;
    int numOutputs = 1;
    int numHiddenLayers = 2;

    int *numHidden = new int[2];
    double *momentums = new double[3];
    double *learnRates = new double[3];

    numHidden[0] = 3;
    numHidden[1] = 3;
    momentums[0] = 0.9;
    momentums[1] = 0.9;
    momentums[2] = 0.9;
    learnRates[0] = 0.02;
    learnRates[1] = 0.02;
    learnRates[2] = 0.02;

    double **inputs = new double*[4];
    for(int x = 0; x < 4; ++x)
    {
        inputs[x] = new double[2];
    }

    inputs[0][0] = -1;
    inputs[0][1] = -1;
    inputs[1][0] = -1;
    inputs[1][1] = 1;
    inputs[2][0] = 1;
    inputs[2][1] = -1;
    inputs[3][0] = 1;
    inputs[3][1] = 1;

    double **expected = new double*[4];
    for(int x = 0; x < 4; ++x)
    {
        expected[x] = new double[1];
    }

    expected[0][0] = -1;
    expected[1][0] = 1;
    expected[2][0] = 1;
    expected[3][0] = -1;

    double *outputs = new double[1];

    string filename = "xor2.p";
    fstream io;

    io.open(filename.c_str(), ios::in | ios::out);

    NeuralNetwork *net;

    if(io.is_open())
    {
        cout << "File exists\n";
        io.close();
        net = new NeuralNetwork(filename);
    }
    else
    {
        cout << "File does not exist.  What do I do???\n";
        net = new NeuralNetwork(filename, numInputs, numOutputs, numHiddenLayers, numHidden, momentums, learnRates);
    }


    ActivationFunctor *newFunctor = new Sigmoid();

    //setSpecificWeights(net);

    net->setLayerActivation(0, newFunctor);

    newFunctor = new Sigmoid();
    net->setLayerActivation(1, newFunctor);

    newFunctor = new Sigmoid();
    ((Sigmoid *)newFunctor)->setVerticalStretchFactor(2);
    ((Sigmoid *)newFunctor)->setVerticalShiftFactor(-1);
    ((Sigmoid *)newFunctor)->setHorizontalStretchFactor(2);
    net->setLayerActivation(2, newFunctor);

    for(int x = 0; x < 500; ++x)
    {
        double delta;
        double rms = 0.0;

        for(int y = 0; y < 4; ++y)
        {
            //int z = rand() % 4;
            cout << "\tinputs: " << inputs[y][0] << ", " << inputs[y][1];
            net->getResults(inputs[y], outputs);
            cout << "\toutputs: " << outputs[0];
            net->backpropagate(outputs, expected[y]);
            cout << "\texpected: " << expected[y][0] << "\n";

            //Find mean square error.
            delta = expected[y][0] - outputs[0];
            rms += delta * delta;
        }
        net->applyWeightChanges();

        rms = sqrt(rms / 4);

        cout << "\tRoot mean square: " << rms << endl;
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
    weights[0][0] = -0.5;
    weights[0][1] = -0.5;
    weights[0][2] = -0.5;

    //neuron 2
    weights[1][0] = -0.5;
    weights[1][1] = -0.5;
    weights[1][2] = -0.5;

    //Neuron 3
    weights[2][0] = -0.5;
    weights[2][1] = -0.5;
    weights[2][2] = -0.5;

    /*
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
*/
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


    weights[0][0] = -0.5;
    weights[0][1] = 0.5;
    weights[0][2] = -0.5;
    weights[0][3] = 0.5;

    /*
    weights[0][0] = -0.892227;
    weights[0][1] = -0.657057;
    weights[0][2] = -0.657068;
    weights[0][3] = -0.846713;
*/
#ifdef DEBUG_NEURALNETWORK
    printLine("Setting output weights");
#endif
    network->setLayerWeights(1, weights);

    delete [] weights[0];
    delete [] weights;
}
