#include "tictactoegamedata.h"

void TicTacToeGameData::init(RulesEngine *rulesEngine)
{
    //Generate tic-tac-toe specific states.
    TicTacToeGrid *startingGrid = new TicTacToeGrid();
    startingGrid->init();

    for(int x = 0; x < 3; ++x)
    {
        for(int y = 0; y < 3; ++y)
        {
            startingGrid->squares[x][y] = Elements::EMPTY;
        }
    }

    m_startingState = new BoardState(startingGrid, NULL, Elements::PLAYER_1, rulesEngine, 1);
    m_currentState = m_startingState;

    delete startingGrid;
}
