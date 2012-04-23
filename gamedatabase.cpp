#include "gamedatabase.h"
#include "macros.h"

GameDatabase::~GameDatabase()
{
    storeDBToFile();
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

int GameDatabase::loadGameFromIndex(BoardState *&root, BoardState *&last, int &gameLength, int databaseIdx, const RulesEngine *re) const
{
    if(databaseIdx >= m_games.size() || databaseIdx < 0)
        return ERROR_BAD_INDEX;

    //Create a root node.
    if(root != NULL)
        delete root;
    root = new BoardState(re->createGameSpecificGrid(), NULL, Elements::PLAYER_1, re);
    //Generate the set of states represented by the string.
    last = root->fromString(m_games.at(databaseIdx), 0, re);

    gameLength = (m_games.at(databaseIdx)).length();

    return 0;
}

int GameDatabase::loadStringFromIndex(int databaseIdx, string &game)
{
    if(databaseIdx >= m_games.size() || databaseIdx < 0)
        return ERROR_BAD_INDEX;

    game = m_games.at(databaseIdx);

    return 0;
}

int GameDatabase::generateDBFromFile()
{
    ifstream in;

    string file = m_directory + m_filename;

    if(m_writeLogs)
        cout << "Reading from file: " << file << endl;

    in.open(file.c_str(), ios::in);

    if(!in.is_open())
    {
        //cerr << "File failed to open!" << endl;
        return ERROR_FILE_OPEN;
    }

    //Pull data from the file one line at a time.

    unsigned int length = 0;

    while(!in.eof())
    {
        string game;
        getline(in, game);

        //Ignore the string if it is empty.
        if(game.length() == 0)
        {
            continue;
        }
        m_games.push_back(game);

        ++length;
    }
    if(length == 0)
        return ERROR_EMPTY;

    if(m_writeLogs)
        cout << "Database now contains " << length << " game records." << endl;

    return 0;
}

int GameDatabase::storeDBToFile()
{
    //simply return success if m_writeToFile is false.

    if(!m_writeToFile)
    {
        //Maybe add a warning here if bugs arise.
        return 0;
    }
    ofstream out;

    string file = m_directory + m_filename;

    //cout << "Writing to file: " << file << endl;

    out.open(file.c_str(), ios::out | ios::trunc);

    unsigned int length = m_games.size();

    if(!out.is_open())
    {
        cerr << "Failed to open file for writing!" << endl;
        return ERROR_FILE_OPEN;
    }
    if(length == 0)
    {
        return ERROR_EMPTY;
    }

    removeDuplicateGames();

    //Get the new length.
    length = m_games.size();

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
    int size;
    if(m_writeLogs)
    {
        size = m_games.size();
        printLine3("Database contains ", size, " games");
    }

    //Sort the container.  Required to erase duplicate elements.
    sort(m_games.begin(), m_games.end());

    //Remove duplicate elements from the vector.
    m_games.erase(unique(m_games.begin(), m_games.end()), m_games.end());

    if(m_writeLogs)
    {
        size -= m_games.size();
        printLine3("Removed ", size, " games");
        printLine2(m_games.size(), " games remain");
    }
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
     generateDBFromFile();
}
