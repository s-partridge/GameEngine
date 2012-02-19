#include "tictactoeviewbuilder.h"

MainWindow *TicTacToeViewBuilder::generateMainView()
{
    //Create a new main window object.
    MainWindow *newMainWindow = new MainWindow(NULL);
    Elements::PlayerType &player = newMainWindow->getCurrentPlayer();
    //Create the central widget for it.
    TicTacToeMainWidget *centralWidget = new TicTacToeMainWidget(player);//, newMainWindow);
    //Assign the new widget to the main window.
    newMainWindow->setMainWidget(centralWidget);
    //Return a pointer to the window instance.
    return newMainWindow;
}
