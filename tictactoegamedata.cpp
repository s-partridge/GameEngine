#include "tictactoegamedata.h"

void TicTacToeGameData::resetBoard(RulesEngine *rulesEngine)
{
#ifdef DEBUG_GAMEDATA
    printLine("TTTGameData->resetBoard");
#endif
    //Empty the tree and return to the first state.
    delete m_startingState;

    init(rulesEngine);
}

void TicTacToeGameData::init(RulesEngine *rulesEngine)
{
    //Generate tic-tac-toe specific states.
    TicTacToeGrid *startingGrid = new TicTacToeGrid();

    m_startingState = new BoardState(startingGrid, NULL, Elements::PLAYER_1, rulesEngine, 1);
    m_currentState = m_startingState;
    m_turnNumber = 0;
//    delete startingGrid;
}
