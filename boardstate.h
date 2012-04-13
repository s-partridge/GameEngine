#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include <fstream>
#include <iomanip>

using namespace std;

#include "grid.h"
#include "rulesengine.h"

class BoardState
{
public:
    //Generates one board state.
    BoardState(Grid *currentGrid, BoardState *parent, Elements::PlayerType currentPlayer, const RulesEngine *rulesEngine);
    //Generates a board state and child nodes for numLayersToBuild iterations.
    BoardState(Grid *currentGrid, BoardState *parent, Elements::PlayerType currentPlayer, const RulesEngine *rulesEngine, int numLayersToBuild);

    void purge();

    ~BoardState() { purge(); }

    //Add a state from a pre-generated grid.
    void addNextState(Grid *nextGrid, int lastMove, const RulesEngine *rulesEngine);
    //Generate child nodes from this state.
    void genNextStates(int numLayers, const RulesEngine *rulesEngine);

    //Set next best move based on the moveWorth of each state in nextStates.
    void setNextBestMove();
    void setLastMove(int lastMove) { m_lastSquare = lastMove; }

    //Recursively create a string containing the moves leading up to this state.
    void toString(string &str) const;
    //Generate child states from an input string.
    //Returns a pointer to the last child created.
    BoardState *fromString(const string &str, int strIdx, const RulesEngine *re);

    //Get the current player.
    Elements::PlayerType getCurrentPlayer() { return m_currentPlayer; }

    //Drill down through the move tree, finding the worth of the current state.
    //If it is an end state, and that end state is a win for the current player,
    //increment the worth of the state.
    //Should replace getMoveWorth in the future.
    virtual int getStateWorth(const RulesEngine *rulesEngine, fstream &toWrite);
    //Uses the same algorithm as getStateWorth(), but creates and destroys each layer.
    //In this way, a huge amount of memory is not exhausted in trying to build the table.
    //Assumes that nextStates is null for the purpose of speed.
    int getStateWorthRecurse(const RulesEngine *rulesEngine, fstream &toWrite);

    int getP1StateWorth() { return m_P1StateWorth; }
    int getP2StateWorth() { return m_P2StateWorth; }
    int getLastSquareMoved() { return m_lastSquare; }

    void printMemoryAddresses(int indentation) const;

    double getMoveWorth() { return m_moveWorth; }
    const Grid *getCurrentGrid() const { return m_currentGrid; }
    BoardState *getParent() { return m_parent; }
    BoardState **getNextStates() { return m_nextStates; }
    BoardState *getNextBestState() { return m_nextBestMove; }
    int getNumNextStates() const { return m_numNextStates; }
    //Return the index of the next best move.
    int getNextBestStateIndex() const { return getIndexOfState(m_nextBestMove->getCurrentGrid()); }
    BoardState *getStateWithIndex(int index) const { return m_nextStates[index]; }
    //Search nextStates for a matching grid.  Return the index if it exists, -1 otherwise.
    BoardState *getState(const Grid *grid);
    //Search nextStates for a matching grid.  Return the index if it exists, -1 otherwise.
    int getIndexOfState(const Grid *grid) const;

    void deleteNextStates();

private:
    Grid *m_currentGrid;
    BoardState *m_parent;
    BoardState **m_nextStates;
    BoardState *m_nextBestMove;
    int m_numNextStates;
    Elements::PlayerType m_currentPlayer;
    double m_moveWorth;

    int m_lastSquare;


protected:
    int m_P1StateWorth;
    int m_P2StateWorth;
};

#endif // BOARDSTATE_H
