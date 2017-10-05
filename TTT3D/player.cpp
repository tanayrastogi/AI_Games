#include "player.hpp"
#include <cstdlib>
#include <algorithm>
#include <math.h>

namespace TICTACTOE3D
{

double infinity = 100000000;
double update(int num_x, int num_o);

GameState Player::play(const GameState &pState,const Deadline &pDue)
{
    //std::cerr << "Processing " << pState.toMessage() << std::endl;
	// Utility value
    double v;
    int depth = 1;
    double alpha = -infinity;
    double beta  = +infinity;

    // Define best state
    GameState bestState;

    // Find available actions given the current player and his action
    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);

    std::cerr<<"\nNumber of possible states are: "<<lNextStates.size();

    // Define max and min player
    max_p = pState.getNextPlayer();
    min_p = max_p ^ (CELL_X | CELL_O);

    if (lNextStates.size() == 0)
        return GameState(pState, Move());

    // Otherwise running alpha-beta for max player
    double bestValue = -infinity;
    for(unsigned int i = 0; i<lNextStates.size(); i++)
    {
        v = alphabeta(lNextStates[i], min_p, depth-1, alpha, beta);
        if(v > bestValue)
        {
            bestValue = v;
            bestState = lNextStates[i];
            alpha = bestValue;
        }
    }

    return bestState;
}

// Minimax algorithm with alpha-beta pruning
double Player::alphabeta(const GameState &pState, uint8_t player, int depth, double alpha, double beta)
{
    std::vector<GameState> childStates;
    double v = 0;

    // Finds all the possible children states
    pState.findPossibleMoves(childStates);

	// If depth is 0 or node is a leaf-node
	if (depth ==0 || childStates.size()==0)
	{
		v = evaluation(pState);
		std::cerr<<"\n The evaluation we get: "<<v;
	}

	// If player is MAX (X-player). We want X to win.
	else if (player == max_p)
	{

        v = -infinity;
        // For every child
        for(unsigned int i = 0; i<childStates.size(); i++)
        {
            v = std::max(v, alphabeta(childStates[i], min_p, depth-1, alpha, beta));
            alpha = std::max(alpha, v);
            // Prune if branch is not useful
            if (beta<=alpha)
                break;
        }
    }

    // If player is MIN (Y-player). We want Y to lose.
    else
    {
        v = +infinity;
        // For every child
        for(unsigned int i = 0; i<childStates.size(); i++)
        {
            v = std::min(v, alphabeta(childStates[i], max_p, depth-1, alpha, beta));
            beta = std::min(beta, v);
            // Prune if branch is not useful
            if (beta<=alpha)
                break;
        }
    }

    return v;
}

double Player::evaluation(const GameState &pState)
{
    const int Possible_wins = 76;

    static const int win_state[Possible_wins][4] = {
    {0,1,2,3},
    {4,5,6,7},
    {8,9,10,11},
    {12,13,14,15},
    {0,4,8,12},
    {1,5,9,13},
    {2,6,10,14},
    {3,7,11,15},
    {0,5,10,15},
    {3,6,9,12},

    {16,17,18,19},
    {20,21,22,23},
    {24,25,26,27},
    {28,29,30,31},
    {16,20,24,28},
    {17,21,25,29},
    {18,22,26,30},
    {19,23,27,31},
    {16,21,26,31},
    {19,22,25,28},

    {32,33,34,35},
    {36,37,38,39},
    {40,41,42,43},
    {44,45,46,47},
    {32,36,40,44},
    {33,37,41,45},
    {34,38,42,46},
    {35,39,43,47},
    {32,37,42,47},
    {35,38,41,44},

    {48,49,50,51},
    {52,53,54,55},
    {56,57,58,59},
    {60,61,62,63},
    {48,52,56,60},
    {49,53,57,61},
    {50,54,58,62},
    {51,55,59,63},
    {48,53,58,63},
    {51,54,57,60},

    {0,16,32,48},
    {1,17,33,49},
    {2,18,34,50},
    {3,19,35,51},
    {4,20,36,52},
    {5,21,37,53},
    {6,22,38,54},
    {7,23,39,55},
    {8,24,40,56},
    {9,25,41,57},
    {10,26,42,58},
    {11,27,43,59},
    {12,28,44,60},
    {13,29,45,61},
    {14,30,46,62},
    {15,31,47,63},

    {0,20,40,60},
    {1,21,41,61},
    {2,22,42,62},
    {3,23,43,63},

    {12,24,36,48},
    {13,25,37,49},
    {14,26,38,50},
    {15,27,39,51},

    {0,17,34,51},
    {3,18,33,48},
    {4,21,38,55},
    {7,22,37,52},
    {8,25,42,59},
    {11,26,41,56},
    {12,29,46,63},
    {15,30,45,60},

    {0,21,42,63},
    {3,22,41,60},
    {12,25,38,51},
    {15,26,37,48}};

    const int heuristic[5][5] = {
    {      1,   -10,  -100, -1000, -10000 },
    {     10,     0,     0,     0, 0      },
    {    100,     0,     0,     0, 0      },
    {   1000,     0,     0,     0, 0      },
    {  10000,     0,     0,     0, 0      } };


    double score = 0;
    int num_x = 0;
    int num_o = 0;
    uint8_t piece;   // Location on the board

    max_p = pState.getNextPlayer();
    min_p = max_p ^ (CELL_X | CELL_O);

    for(unsigned int i=0; i<Possible_wins; i++)
    {
        num_x = 0;
        num_o = 0;
        for(int j=0; j<4;j++)
        {

            piece = pState.at(win_state[i][j]);
            if(piece == CELL_X)
                num_x++;
            else if(piece == CELL_O)
                num_o++;
        }
        score = score + heuristic[num_x][num_o];

    }

    return score;

}


/*namespace TICTACTOE3D*/ }
