#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include "gamedata.h"
#include "statisticsdata.h"
#include "rulesengine.h"
#include "statisticsdata.h"
class DataController
{
public:
    DataController();

    //Run this grid through the rules engine.
    //Return true if move is valid, false otherwise.
    bool checkMove(Grid *move);

    //Run this grid through the rules engine.
    //Return true if the move is a final state, false otherwise.
    bool isGameOver(Grid * move);

    //Add the stats from this structure to the stored values in statistics.
    void addToGameStats(StatisticsData toAdd);
    //Add training statistics to the AI user denoted by player.
    void addToTrainingStats(AITrainingStats toAdd, int player);

    void resetGameStats() { statistics->resetCurrentStats(); }
    void resetTotalStats() { statistics->resetTotalStats(); }
    void resetTrainingStats();

    BoardState *getCurrentState() { return moveTree->getCurrentState(); }
    void undoMove() { moveTree->undoMove(); }
    void resetTree() { moveTree->resetBoard(); }

    //Instruct the move tree to build more layers in the tree.
    void genereateNextLayers(int numLayers = 1) { moveTree->genNextMoves(numLayers); }


private:
    GameData *moveTree;
    StatisticsData *statistics;
    RulesEngine *rulesEngine;
};

#endif // DATACONTROLLER_H
