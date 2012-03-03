#include "movetablebuilder.h"

void MoveTableBuilder::createMoveTableFile(BoardState *startingState, string filename, RulesEngine *rulesEngine)
{
    fstream f(filename.c_str(), ios::out);

    f.close();

    //If the file contains anything, delete it.
    f.open(filename.c_str(), ios::out | ios::binary | ios::trunc);

    //Todo: Look into error codes for fstream.
    //Find a way to wrap up errors and print them out to the console.
    //Folder permissions?
    if(f.is_open()/* f.good()*/)
    {
        cout << "File successfully opened.\n";
        //Go through the entire tree and build up the values of each state.  After finding them,
        //write each grid to the file with the next best move.
        startingState->getStateWorthRecurse(rulesEngine, f);

        f.close();
    }
    else
        cout << "Unable to open file.\n";
}

void MoveTableBuilder::generateHashTable(string filename, map<string, int> &hashMap)
{
    ifstream i;
    i.open(filename.c_str(), ios::in | ios::binary);

    string key = "";
    string value = "";

    string delimiter = "";

    //Run through the entire file.
    while(!i.eof())
    {
        key = "";
        i >> delimiter;
        //Keep adding to key until the comma is found.
        while(delimiter != ",")
        {
            //Add the current value,
            key += delimiter;
            //Pull the next.
            i >> delimiter;

            //Add a space after all but the last pieces.
            if(delimiter != ",")
                key += " ";
        }

        //The next entry should be the corresponding value.
        i >> value;

        //Store the result in the map.
        hashMap[key] = atoi(value.c_str());
    }
    i.close();
}
