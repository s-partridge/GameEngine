#include "boardstate.h"

//Generates one board state.
BoardState::BoardState(const Grid *currentGrid, BoardState *parent, int currentPlayer, RulesEngine *rulesEngine)
    : m_parent(parent), m_currentPlayer(currentPlayer)
{
    m_currentGrid = currentGrid;
    m_nextBestMove = NULL;
    m_nextStates = NULL;
    int m_numNextStates = 0;

    m_moveWorth = rulesEngine->worthOfState(currentGrid, m_currentPlayer, rulesEngine->testBoard(currentGrid));
}


//Generates a board state and child nodes for numLayersToBuild iterations.
BoardState::BoardState(const Grid *currentGrid, BoardState *parent, int currentPlayer, const RulesEngine *rulesEngine, int numLayersToBuild)
    : m_parent(parent), m_currentPlayer(currentPlayer)
{
    m_currentGrid = currentGrid;
    m_nextStates = NULL;

    genNextStates(numLayersToBuild - 1, rulesEngine);

    m_moveWorth = rulesEngine->worthOfState(currentGrid, m_currentPlayer, rulesEngine->testBoard(currentGrid));
}

void BoardState::purge()
{
    if(m_currentGrid != NULL)
    {
        delete m_currentGrid;
        m_currentGrid = NULL;
    }

    deleteNextStates();

    //nextBestMove is a reference to a state already stored in nextStates.
    m_nextBestMove = NULL;
}

void BoardState::genNextStates(int numLayers, const RulesEngine *rulesEngine)
{
    //Create a temporary array to contain the grids.
    Grid **nextGrids = NULL;
    rulesEngine->genNextMoves(m_currentGrid, nextGrids, m_currentPlayer, m_numNextStates);

    deleteNextStates();

    m_nextStates = new *BoardState[m_numNextStates];

    Elements::PlayerType nextPlayer;

    if(m_currentPlayer == Elements::PLAYER_1)
    {
        nextPlayer = Elements::PLAYER_2;
    }
    else
        nextPlayer = Elements::PLAYER_1;
    for(int x = 0; x < m_numNextStates; ++x)
    {
        if(numLayers > 0)
            m_nextStates[x] = new BoardState(nextGrids[x], this, nextPlayer, rulesEngine, numlayers);

        else
            m_nextStates[x] = new BoardState(nextGrids[x], this, nextPlayer, rulesEngine);
    }

    //NextStates is passed by value to the new boards, so the local reference must be deleted.
    for(int x = o; x < numNextStates; ++x)
    {
        delete nextGrids[x];
    }

    delete [] nextGrids;

    setNextBestMove();
}

void BoardState::setNextBestMove()
{
    double highestWorthIndex = 0;

    for(int x = 1; x < m_numNextStates; ++x)
    {
        //Check each state against the last state found with a higher move worth.
        //If a higher value is found, save the index of that move.
        if(m_nextStates[x]->getMoveWorth() > m_nextStates[highestWorthIndex]->getMoveWorth())
        {
            highestWorthIndex = x;
        }
    }

    m_nextBestMove = m_nextStates[highestWorthIndex];
}

BoardState* BoardState::getState(const Grid *grid)
{
    for(int x = 0; x < m_numNextStates; ++x)
    {
        //Compare each state's grid with the grid parameter.
        //Return the state containing the matching grid.
        if(m_nextStates[x]->*grid == *grid)
        {
            return m_nextStates[x];
        }
    }

    //If there is no match, return a null pointer.
    return NULL;
}

int BoardState::getIndexOfState(const Grid *grid)
{
    for(int x = 0; x < m_numNextStates; ++x)
    {
        //Compare each state's grid with the grid parameter.
        //Return the index of the matching grid.
        if(m_nextStates[x]->*grid == *grid)
        {
            return x;
        }
    }
    return -1;
}

void BoardState::deleteNextStates()
{
    //Make sure the next states are empty before generating new ones.
    if(m_nextStates != NULL)
    {
        for(int x = 0; x < m_numNextStates; ++x)
        {
            delete m_nextStates[x];
        }

        delete [] m_nextStates;
        m_nextStates == NULL;
    }
}
