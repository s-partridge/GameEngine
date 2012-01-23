#ifndef STATSTRUCTURES_H
#define STATSTRUCTURES_H

struct GameStats
{
    bool player1Win;
    bool player2Win;

    GameStats() : player1Win(false), player2Win(false) {}

    GameStats &operator+(GameStats &rhs);
    GameStats &operator=(GameStats &rhs);

    void init();
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

    AITrainingStats() : bestMoves(0), errors(0), wins(0), losses(0), totalMoves(0), percentageBest(0), percentageWorst(0) {}

    AITrainingStats &operator+(AITrainingStats &rhs);
    AITrainingStats &operator=(AITrainingStats &rhs);

    void init();
};

#endif // STATSTRUCTURES_H
