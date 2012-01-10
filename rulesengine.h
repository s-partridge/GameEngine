#ifndef RULESENGINE_H
#define RULESENGINE_H

class RulesEngine
{
public:
    RulesEngine() {}

    //Test a board state for end game conditions.
    virtual GameState testBoard(const Grid &boardState) = 0;
    //Find the value of a board state.
    //That is, find whether it represents a good or bad position for the current player.
    //Values range from 0 (loss) to 1 (win).   0.5 is a neutral state (empty board, cat's game, etc).
    virtual float worthOfState(const Grid &boardState, PlayerType currentPlayer, GameState currentState) = 0;
};

#endif // RULESENGINE_H
