#ifndef DATABASECONSTANTS_H
#define DATABASECONSTANTS_H

#define DIRECTORY_TTT           "tictactoe"
#define DIRECTORY_C4            "connectfour"

//Check the operating system
//Determine what type of directory string to use.
#ifdef  Q_OS_WIN32
#define DIRECTORY_SLASH         '\\'
#elseif Q_OS_WIN64
#define DIRECTORY_SLASH         '\\'
#else
#define DIRECTORY_SLASH         '/'
#endif

//Used by game controller.
#define FILENAME_USER_GAMES     "usergames.db"

//Trainers can define their own database files, and be guaranteed that usergames.db exists.

#endif // DATABASECONSTANTS_H
