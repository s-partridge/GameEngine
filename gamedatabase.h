#ifndef GAMEDATABASE_H
#define GAMEDATABASE_H

#include <vector>
#include <string>
#include <fstream>
#include "boardstate.h"
#include "rulesengine.h"
#include "databaseConstants.h"

using namespace std;

#define BUILD_DB    true

#define ERROR_FILE_OPEN     1
#define ERROR_FILE_IO       2
#define ERROR_BAD_INDEX     4
#define ERROR_EMPTY         8
#define ERROR_FULL          16

class GameDatabase
{
    string m_directory;
    string m_filename;
    vector<string> m_games;
    bool m_saveGames;
    bool m_writeToFile;
    bool m_writeLogs;

    void purge() { m_games.clear(); }

public:
    GameDatabase() : m_filename("default.db"), m_saveGames(BUILD_DB), m_writeToFile(true), m_writeLogs(true) { }
    ~GameDatabase();

    static GameDatabase &getDatabase();
    //Generate string from end of game.  Store in m_games
    int storeGame(const BoardState *endState);
    //Generate move tree from string in DB.  Use re to create correct states.
    //Returns bad_index if databaseIdx is too high.
    int loadGameFromIndex(BoardState *&root, BoardState *&last, int &gameLength, int databaseIdx, const RulesEngine *re) const;

    int loadStringFromIndex(int databaseIdx, string &game);
    //Fill database with entries from a specified file.
    int generateDBFromFile();
    //Write database entries to a specified file.
    int storeDBToFile();
    //Remove duplicate games from the collection.
    void removeDuplicateGames();

    //Return the current size of the collection.
    int size() { return m_games.size(); }

    void setDBFile(string filename);
    void setDirectory(string directory) { m_directory = directory; }
    void setSaveGames(bool saveGames) { m_saveGames = saveGames; }
    void setWriteToFile(bool write) { m_writeToFile = write; }
    void setPrintLogs(bool printLogs) { m_writeLogs = printLogs; }
};

#endif // GAMEDATABASE_H
