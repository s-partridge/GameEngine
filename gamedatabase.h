#ifndef GAMEDATABASE_H
#define GAMEDATABASE_H

#include <vector>
#include <string>
#include <fstream>
#include "boardstate.h"
#include "rulesengine.h"

using namespace std;

#define BUILD_DB    true

#define ERROR_FILE_OPEN     1
#define ERROR_FILE_IO       2
#define ERROR_BAD_INDEX     4
#define ERROR_EMPTY         8
#define ERROR_FULL          16

class GameDatabase
{
    string m_filename;
    vector<string> m_games;
    bool m_saveGames;

    GameDatabase() : m_filename("default.db"), m_saveGames(BUILD_DB) {}

    void purge() { m_games.clear(); }

    ~GameDatabase();

public:

    static GameDatabase &getDatabase();
    //Generate string from end of game.  Store in m_games.
    int storeGame(const BoardState *endState);
    //Generate move tree from string in DB.  Use re to create correct states.
    //Returns bad_index if databaseIdx is too high.
    int loadGameFromIndex(BoardState *&root, BoardState *&last, int databaseIdx, const RulesEngine *re) const;

    const string &loadStringFromIndex(int databaseIdx) { return m_games.at(databaseIdx); }
    //Fill database with entries from a specified file.
    int generateDBFromFile();
    //Write database entries to a specified file.
    int storeDBToFile();
    //Remove duplicate games from the collection.
    void removeDuplicateGames();

    //Return the current size of the collection.
    int size() { return m_games.size(); }

    void setDBFile(string filename);
    void setSaveGames(bool saveGames) { m_saveGames = saveGames; }
};

#endif // GAMEDATABASE_H
