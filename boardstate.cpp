#include "boardstate.h"

#define PRINT_ITERATIONS
#ifdef PRINT_ITERATIONS
int layerNumber = 0;
#endif
//Generates one board state.
BoardState::BoardState(Grid *currentGrid, BoardState *parent, Elements::PlayerType currentPlayer, const RulesEngine *rulesEngine)
    : m_parent(parent), m_currentPlayer(currentPlayer)
{
#ifdef DEBUG_BOARDSTATE
    printLine4("The board state at address ", this, " contains grid with address: ", currentGrid);
#endif
    m_currentGrid = currentGrid;
    m_nextBestMove = NULL;
    m_nextStates = NULL;
    m_numNextStates = 0;
    m_P1StateWorth = 0;
    m_P2StateWorth = 0;

    m_moveWorth = rulesEngine->worthOfState(currentGrid, m_currentPlayer, rulesEngine->testBoard(currentGrid));
}

//Generates a board state and child nodes for numLayersToBuild iterations.
BoardState::BoardState(Grid *currentGrid, BoardState *parent, Elements::PlayerType currentPlayer, const RulesEngine *rulesEngine, int numLayersToBuild)
    : m_parent(parent), m_currentPlayer(currentPlayer)
{
#ifdef DEBUG_BOARDSTATE
    printLine4("The board state at address ", this, " contains grid with address: ", currentGrid);
#endif
    m_currentGrid = currentGrid;
    m_nextStates = NULL;
    m_P1StateWorth = 0;
    m_P2StateWorth = 0;

    genNextStates(numLayersToBuild - 1, rulesEngine);

    m_moveWorth = rulesEngine->worthOfState(currentGrid, m_currentPlayer, rulesEngine->testBoard(currentGrid));
}

void BoardState::purge()
{
#ifdef DEBUG_BOARDSTATE
    printLine2("Deleting child states for board state at address ", this);
#endif
    deleteNextStates();
#ifdef DEBUG_BOARDSTATE
    printLine2("Finished deleting child states for board state at address ", this);
#endif
    if(m_currentGrid != NULL)
    {
#ifdef DEBUG_BOARDSTATE
        printLine2("Deleting grid for board state at address ", this);
        printLine2("Grid has address ", m_currentGrid);
#endif
        delete m_currentGrid;
        m_currentGrid = NULL;
    }

    //nextBestMove is a reference to a state already stored in nextStates.
    m_nextBestMove = NULL;
}

void BoardState::genNextStates(int numLayers, const RulesEngine *rulesEngine)
{
#ifdef DEBUG_BOARDSTATE
    printLine3("genNextStates for ", numLayers, " layers.");
#endif

    //Make sure this is not an end state.
    if(rulesEngine->testBoard(m_currentGrid) != Elements::NORMAL)
        return;

    //Create a temporary array to contain the grids.
    Grid **nextGrids = NULL;
    //genNextMoves will set the value of m_numNextStates and nextGrids.
    rulesEngine->genNextMoves(m_currentGrid, nextGrids, m_currentPlayer, m_numNextStates);

    deleteNextStates();

    m_nextStates = new BoardState*[m_numNextStates];

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
            m_nextStates[x] = new BoardState(nextGrids[x], this, nextPlayer, rulesEngine, numLayers);

        else
            m_nextStates[x] = new BoardState(nextGrids[x], this, nextPlayer, rulesEngine);
    }

    //The grids for m_nextStates are passed by reference, but there is still an unhandled
    //reference to the array itself that must be accounted for.
    delete nextGrids;

    setNextBestMove();
}

void BoardState::setNextBestMove()
{
    int highestWorthIndex = 0;

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

int BoardState::getStateWorth(const RulesEngine *rulesEngine, fstream &toWrite)
{
    //Make sure this is not an end state.
    if(m_nextStates != NULL)
    {
        int highestStateIndex = 0;

        for(int x = 0; x < m_numNextStates; ++x)
        {
#ifdef DEBUG_STATEWORTH
            printLine4("Finding worth of state ", x, " of ", m_numNextStates);
#endif
            //Add the move worth of each successive state to that of the current state.
            m_nextStates[x]->getStateWorth(rulesEngine, toWrite);

            m_P1StateWorth += m_nextStates[x]->getP1StateWorth();
            m_P2StateWorth += m_nextStates[x]->getP2StateWorth();
            //Find the most valuable state.
            if(m_currentPlayer == Elements::PLAYER_1)
            {
                if(m_nextStates[x]->getP1StateWorth() > m_nextStates[highestStateIndex]->getP1StateWorth())
                    highestStateIndex = x;
            }
            else if(m_currentPlayer == Elements::PLAYER_2)
            {
                if(m_nextStates[x]->getP2StateWorth() > m_nextStates[highestStateIndex]->getP2StateWorth())
                    highestStateIndex = x;
            }
        }

#ifdef DEBUG_STATEWORTH
        printLine("Writing to file");
#endif
        //Only need to write non-terminal states to the file.
        //The program should never try to read a next state from an end state.
        toWrite << *m_currentGrid << " , ";
        toWrite << m_currentGrid->getFirstDifference(m_nextStates[highestStateIndex]->getCurrentGrid()) << "\n";
        //        << " " << m_P1StateWorth << " " << m_P2StateWorth << "\n";
    }
    else
    {
        if(rulesEngine->testBoard(m_currentGrid) == Elements::P1WIN)
        {
            ++m_P1StateWorth;
        }
        else if(rulesEngine->testBoard(m_currentGrid) == Elements::P2WIN)
        {
            ++m_P2StateWorth;
        }
        else if(rulesEngine->testBoard(m_currentGrid) == Elements::DRAW)
        {
            ++m_P1StateWorth;
            ++m_P2StateWorth;
        }

    }

    if(m_currentPlayer == Elements::PLAYER_1)
    {
        return m_P1StateWorth;
    }
    else
    {
        return m_P2StateWorth;
    }
}

int BoardState::getStateWorthRecurse(const RulesEngine *rulesEngine, fstream &toWrite)
{
#ifdef PRINT_ITERATIONS
    ++layerNumber;
#endif
    //Make sure this is not an end state.
    if(rulesEngine->testBoard(m_currentGrid) == Elements::NORMAL)
    {
        //Create the next layer of states.
        genNextStates(1, rulesEngine);

        //Will contain the index of the next best state.
        //Starts at zero and increments when a higher value is found.
        int highestStateIndex = 0;

        for(int x = 0; x < m_numNextStates; ++x)
        {
#ifdef DEBUG_STATEWORTH
            printLine4("Finding worth of state ", x, " of ", m_numNextStates);
#endif
            //Add the move worth of each successive state to that of the current state.
            m_nextStates[x]->getStateWorthRecurse(rulesEngine, toWrite);
#ifdef PRINT_ITERATIONS
            --layerNumber;
            if(layerNumber <= 10)
                cout << "Finished with high-level layer " << layerNumber << " state " << x << endl;
#endif
            m_P1StateWorth += m_nextStates[x]->getP1StateWorth();
            m_P2StateWorth += m_nextStates[x]->getP2StateWorth();
            //Find the most valuable state.
            if(m_currentPlayer == Elements::PLAYER_1)
            {
                if(m_nextStates[x]->getP1StateWorth() > m_nextStates[highestStateIndex]->getP1StateWorth())
                    highestStateIndex = x;
            }
            else if(m_currentPlayer == Elements::PLAYER_2)
            {
                if(m_nextStates[x]->getP2StateWorth() > m_nextStates[highestStateIndex]->getP2StateWorth())
                    highestStateIndex = x;
            }
        }

#ifdef DEBUG_STATEWORTH
        printLine("Writing to file");
#endif
        //Only need to write non-terminal states to the file.
        //The program should never try to read a next state from an end state.
        char *squares;
        char nextMove = (m_currentGrid->getFirstDifference(m_nextStates[highestStateIndex]->getCurrentGrid()));
        m_currentGrid->toCharArray(squares);
        //Write each square to the file.
        toWrite.write(squares, m_currentGrid->width * m_currentGrid->height);
        //Write the next move to the file.
        toWrite.write((char *)&nextMove, 1);
        //toWrite << *m_currentGrid << " , ";
        //toWrite << m_currentGrid->getFirstDifference(m_nextStates[highestStateIndex]->getCurrentGrid())<< "\n";
        //toWrite << " [" << m_P1StateWorth << ", " << m_P2StateWorth << "]\n";
        delete [] squares;
        //Remove the next states to preserve memory.
        deleteNextStates();
    }
    else
    {
        if(rulesEngine->testBoard(m_currentGrid) == Elements::P1WIN)
        {
            ++m_P1StateWorth;
        }
        else if(rulesEngine->testBoard(m_currentGrid) == Elements::P2WIN)
        {
            ++m_P2StateWorth;
        }
        else if(rulesEngine->testBoard(m_currentGrid) == Elements::DRAW)
        {
            ++m_P1StateWorth;
            ++m_P2StateWorth;
        }

    }

    if(m_currentPlayer == Elements::PLAYER_1)
    {
        return m_P1StateWorth;
    }
    else
    {
        return m_P2StateWorth;
    }
}

BoardState* BoardState::getState(const Grid *grid)
{
    for(int x = 0; x < m_numNextStates; ++x)
    {
        //Compare each state's grid with the grid parameter.
        //Return the state containing the matching grid.
        //Dereference both grids to compare them.
        if((*m_nextStates[x]->getCurrentGrid()) == *grid)
        {
            return m_nextStates[x];
        }
    }

    //If there is no match, return a null pointer.
    return NULL;
}

int BoardState::getIndexOfState(const Grid *grid) const
{
    for(int x = 0; x < m_numNextStates; ++x)
    {
        //Compare each state's grid with the grid parameter.
        //Return the index of the matching grid.
        if(*(m_nextStates[x]->getCurrentGrid()) == *grid)
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
            if(m_nextStates[x] != NULL)
                delete m_nextStates[x];
        }

        delete [] m_nextStates;
        m_nextStates = NULL;
    }
}

void BoardState::printMemoryAddresses(int indentation) const
{
#ifdef DEBUG_MEMORY
    print2(setw(indentation), "");
    printLine2("Board state: ", this);
    print2(setw(indentation), "");
    printLine2(" Current Grid: ", m_currentGrid);
    print2(setw(indentation), "");
    printLine2(" Grid Squares: ", m_currentGrid->squares);
    if(m_numNextStates > 0)
    {
        print2(setw(indentation), "");
        printLine("Children:");
    }

    for(int x = 0; x < m_numNextStates; ++x)
    {
        m_nextStates[x]->printMemoryAddresses(indentation + 4);
    }
#endif
}
