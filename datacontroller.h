#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include "gamedata.h"
#include "statisticsdata.h"
#include "rulesengine.h"
#include "statisticsdata.h"
class DataController
{
public:
    DataController() : m_moveTree(NULL), m_statistics(NULL), m_rulesEngine(NULL) { }

    //Run this grid through the rules engine.
    //Return true if move is valid, false otherwise.
    bool checkMove(const Grid *move, Elements::PlayerType currentPlayer) { return m_rulesEngine->isValidMove(getCurrentState()->getCurrentGrid(), move, currentPlayer); }

    //Run this grid through the rules engine.
    //Return true if the move is a final state, false otherwise.
    bool isGameOver(const Grid * move);

    void setMoveTree(GameData *moveTree) { m_moveTree = moveTree; }
    void setStatistics(StatisticsData *statistics) { m_statistics = statistics; }
    void setRulesEngine(RulesEngine *rulesEngine) { m_rulesEngine = rulesEngine; }

    //Add the stats from this structure to the stored values in statistics.
    void addToGameStats(GameStats toAdd) { m_statistics->addToGameStats(toAdd); }
    //Add training statistics to the AI user denoted by player.
    void addToTrainingStats(AITrainingStats toAdd, Elements::PlayerType player);

    //Reset individual statistics.
    void resetGameStats() { m_statistics->resetCurrentStats(); }
    void resetTotalStats() { m_statistics->resetTotalStats(); }
    void resetTrainingStats();

    //Reset all statistics.
    void resetStatistics();

    BoardState *getCurrentState() { return m_moveTree->getCurrentState(); }

    //Shifts currentState to the state matching the grid in next.
    void setNextMove(const Grid *next);

    void undoMove() { m_moveTree->undoMove(m_rulesEngine); }
    void resetTree() { m_moveTree->resetBoard(m_rulesEngine); }

    //Instruct the move tree to build more layers in the tree.
    void genereateNextLayers(int numLayers = 1) { m_moveTree->genNextMoves(numLayers, m_rulesEngine); }

private:
    GameData *m_moveTree;
    StatisticsData *m_statistics;
    RulesEngine *m_rulesEngine;

};

#endif // DATACONTROLLER_H
