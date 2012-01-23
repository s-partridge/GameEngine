#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include "grid.h"
#include "rulesengine.h"

class BoardState
{
public:
    //Generates one board state.
    BoardState(const Grid *currentGrid, BoardState *parent, int currentPlayer, RulesEngine *rulesEngine);
    //Generates a board state and child nodes for numLayersToBuild iterations.
    BoardState(const Grid *currentGrid, BoardState *parent, int currentPlayer, const RulesEngine *rulesEngine, int numLayersToBuild);

    void purge();

    ~BoardState() { purge(); }

    //Generate child nodes from this state.
    void genNextStates(int numLayers, const RulesEngine *rulesEngine);

    //Set next best move based on the moveWorth of each state in nextStates.
    void setNextBestMove();

    double getMoveWorth() { return m_moveWorth; }
    Grid *getCurrentGrid() { return m_currentGrid; }
    BoardState *getParent() { return m_parent; }
    BoardState **getNextStates() { return m_nextStates; }
    BoardState *getNextBestState() { return m_nextBestMove; }
    int getNumNextStates() { return m_numNextStates; }
    //Return the index of the next best move.
    int getNextBestStateIndex() { return getIndexOfState(m_nextBestMove->getCurrentGrid()); }
    BoardState *getState(int index) { return m_nextStates[index]; }
    //Search nextStates for a matching grid.  Return the index if it exists, -1 otherwise.
    BoardState *getState(const Grid *grid);
    //Search nextStates for a matching grid.  Return the index if it exists, -1 otherwise.
    int getIndexOfState(const Grid *grid);

    void deleteNextStates();

private:
    Grid *m_currentGrid;
    BoardState *m_parent;
    BoardState **m_nextStates;
    BoardState *m_nextBestMove;
    int m_numNextStates;
    int m_currentPlayer;
    double m_moveWorth;
};

#endif // BOARDSTATE_H
