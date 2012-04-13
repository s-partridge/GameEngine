#include "gamedatabase.h"

GameDatabase::~GameDatabase()
{
    storeDBToFile();
}

GameDatabase &GameDatabase::getDatabase()
{
    //Create a database if one does not exist.
    static GameDatabase db;
    return db;
}

int GameDatabase::storeGame(const BoardState *endState)
{
    //Do nothing if this flag is false.
    if(!m_saveGames)
        return 0;

    if(m_games.size() >= m_games.max_size())
        return ERROR_FULL;

    string game;

    endState->toString(game);

    m_games.push_back(game);

    return 0;
}

int GameDatabase::loadGameFromIndex(BoardState *&root, BoardState *&last, int databaseIdx, const RulesEngine *re) const
{
    if(databaseIdx >= m_games.size() || databaseIdx < 0)
        return ERROR_BAD_INDEX;

    //Create a root node.
    root = new BoardState(re->createGameSpecificGrid(), NULL, Elements::PLAYER_1, re);
    //Generate the set of states represented by the string.
    last = root->fromString(m_games.at(databaseIdx), 0, re);

    return 0;
}

int GameDatabase::generateDBFromFile()
{
    ifstream in;

    in.open(m_filename.c_str(), ios::in);

    if(!in.is_open())
    {
        return ERROR_FILE_OPEN;
    }

    //Pull data from the file one line at a time.

    unsigned int length = 0;

    while(!in.eof())
    {
        string game;
        getline(in, game);

        m_games.push_back(game);

        ++length;
    }
    if(length == 0)
        return ERROR_EMPTY;

    return 0;
}

int GameDatabase::storeDBToFile()
{
    ofstream out;
    out.open(m_filename.c_str(), ios::out | ios::trunc);

    unsigned int length = m_games.size();

    if(!out.is_open())
    {
        return ERROR_FILE_OPEN;
    }
    if(length == 0)
    {
        return ERROR_EMPTY;
    }

    removeDuplicateGames();
    //Write each string to the file.
    unsigned int x;

    for(x = 0; x < length - 1; ++x)
    {
        out << m_games.at(x) << '\n';
    }
    //Don't want a '\n' at the end of the file.
    out << m_games.at(x);

    return 0;
}

void GameDatabase::removeDuplicateGames()
{
    //Sort the container.  Required to erase duplicate elements.
    sort(m_games.begin(), m_games.end());

    //Remove duplicate elements from the vector.
    m_games.erase(unique(m_games.begin(), m_games.end()), m_games.end());
}

void GameDatabase::setDBFile(string filename)
{
    //If the collection has any items, write them to the current file before changing.
    if(m_games.size() > 0)
    {
        storeDBToFile();
        //remove existing data.
        purge();
    }

     m_filename = filename;
}
