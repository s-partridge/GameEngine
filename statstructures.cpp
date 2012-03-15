#include "statstructures.h"

/////////////
//GameStats//
/////////////

void GameStats::init()
{
    player1Win = false;
    player2Win = false;
}

GameStats GameStats::operator =(const GameStats &rhs)
{
    player1Win = rhs.player1Win;
    player2Win = rhs.player2Win;

    return *this;
}

GameStats GameStats::operator+=(const GameStats &rhs)
{
    player1Win += rhs.player1Win;
    player2Win += rhs.player2Win;
    return *this;
}

///////////////////
//AITrainingStats//
///////////////////

void AITrainingStats::init()
{
    bestMoves = 0;
    errors = 0;
    wins = 0;
    draws = 0;
    losses = 0;
    totalMoves = 0;
    percentageBest = 0.0;
    percentageWorst = 0.0;
}

AITrainingStats AITrainingStats::operator =(const AITrainingStats &rhs)
{
    bestMoves = rhs.bestMoves;
    errors = rhs.errors;
    wins = rhs.wins;
    draws = rhs.draws;
    losses = rhs.losses;
    totalMoves = rhs.totalMoves;

    percentageBest = rhs.percentageBest;
    percentageWorst = rhs.percentageWorst;

    return *this;
}

AITrainingStats AITrainingStats::operator+=(const AITrainingStats &rhs)
{
    bestMoves += rhs.bestMoves;
    errors += rhs.errors;
    wins += rhs.wins;
    draws += rhs.draws;
    losses += rhs.losses;
    totalMoves += rhs.totalMoves;

    if(totalMoves != 0)
    {
        percentageBest = bestMoves / (double)totalMoves * 100;
        percentageWorst = errors / (double)totalMoves * 100;
    }

    return *this;
}
