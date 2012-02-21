#include "connectfourviewbuilder.h"

MainWindow *ConnectFourViewBuilder::generateMainView()
{
    //Create a new main window object.
    MainWindow *newMainWindow = new MainWindow(NULL);
    Elements::PlayerType &player = newMainWindow->getCurrentPlayer();
    //Create the central widget for it.
    ConnectFourMainWidget *centralWidget = new ConnectFourMainWidget(player);//, newMainWindow);
    //Assign the new widget to the main window.
    newMainWindow->setMainWidget(centralWidget);
    //Return a pointer to the window instance.
    return newMainWindow;
}
