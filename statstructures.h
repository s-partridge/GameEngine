#ifndef STATSTRUCTURES_H
#define STATSTRUCTURES_H

struct GameStats
{
    bool player1Win;
    bool player2Win;

    void init() : player1Win(false), player2Win(false) {}
};

struct AITrainingStats
{
    int bestMoves;
    int errors;
    int wins;
    int losses;
    int totalMoves;
    double percentageBest;
    double percentageWorst;

    void init() : bestMoves(0), errors(0), wins(0), losses(0), totalMoves(0), percentageBest(0), percentageWorst(0) {}
};

#endif // STATSTRUCTURES_H
