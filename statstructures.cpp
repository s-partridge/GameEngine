#include "statstructures.h"

void GameStats::init()
{
    player1Win = false;
    player2Win = false;
}

GameStats &GameStats::operator +(GameStats &rhs)
{
    player1Win += rhs.player1Win;
    player2Win += rhs.player2Win;

    return *this;
}

GameStats &GameStats::operator =(GameStats &rhs)
{
    player1Win = rhs.player1Win;
    player2Win = rhs.player2Win;

    return *this;
}

void AITrainingStats::init()
{
    bestMoves = 0;
    errors = 0;
    wins = 0;
    losses = 0;
    totalMoves = 0;
    percentageBest = 0.0;
    percentageWorst = 0.0;
}

AITrainingStats &AITrainingStats::operator +(AITrainingStats &rhs)
{
    bestMoves += rhs.bestMoves;
    errors += rhs.errors;
    wins += rhs.wins;
    losses += rhs.losses;
    totalMoves += rhs.totalMoves;

    percentageBest = bestMoves / (double)totalMoves * 100;
    percentageWorst = errors / (double)totalMoves * 100;

    return *this;
}

AITrainingStats &AITrainingStats::operator =(AITrainingStats &rhs)
{
    bestMoves = rhs.bestMoves;
    errors = rhs.errors;
    wins = rhs.wins;
    losses = rhs.losses;
    totalMoves = rhs.totalMoves;

    percentageBest = rhs.percentageBest;
    percentageWorst = rhs.percentageWorst;

    return *this;
}
