#ifndef TDCONSTANTS_H
#define TDCONSTANTS_H

//Constants for a td-lambda AI player
#define GAMMA               1.0
#define LAMBDA              0.3
#define TD_LEARN_RATE       0.3
#define TD_NUM_OUTPUTS      1
#define TD_EXPECTED_ROUNDS  43

//There is no way that this can be zero unless the random number generated actually is zero
//if RANDOM_MOVE_INTERVAL > RAND_MAX.
#define RANDOM_MOVE_INTERVAL 90 //RAND_MAX

#define TD_WIN      1.0
#define TD_LOSS     -1.0
#define TD_DRAW     0.0

//Constants for depth-limited search
//Represents a value for states that have been directly evaluated, rather than having their children evaluated.
//Search depth determines how many moves ahead the player will look when making a move.
#define DLS_SEARCH_DEPTH    1
#define DLS_EVALUATED_STATE -1

#endif // TDCONSTANTS_H
