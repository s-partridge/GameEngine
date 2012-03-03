#ifndef MOVETABLEBUILDER_H
#define MOVETABLEBUILDER_H

#include <fstream>
#include <string>
#include <map>

#include "boardstate.h"
#include "rulesengine.h"

using namespace std;

class MoveTableBuilder
{
public:
    MoveTableBuilder() {}

    static void createMoveTableFile(BoardState *startingState, string filename, RulesEngine *rulesEngine);
    static void generateHashTable(string filename, map<string, int> &hashMap);
};

#endif // MOVETABLEBUILDER_H
