#include "player.hpp"
#include <cstdlib>
#include <algorithm>
#include <math.h>

namespace TICTACTOE
{

GameState Player::play(const GameState &pState,const Deadline &pDue)
{
    int v = 0;
    int depth = 3;
    int bestValue = 0;
    std::vector<GameState> lNextStates;
    GameState bestState;

    // Define max and min player
    max_p = pState.getNextPlayer();
    min_p = max_p ^ (CELL_X | CELL_O);


    // Finds all the possible children states
    pState.findPossibleMoves(lNextStates);


    // If the state is a terminal state then
    if (lNextStates.size() == 0)
        return GameState(pState, Move());

    // Otherwise running Minimax for max player
    bestValue = -1000000;
    for(unsigned int i = 0; i<lNextStates.size(); i++)
    {
        v = minimax(lNextStates[i], min_p, depth - 1);
        if(v > bestValue)
        {
            bestValue = v;
            bestState = lNextStates[i];
//            std::cerr<<"\nThe best state is at position: "<<i;
//            std::cerr<<"\nThe best value for state: "<<bestValue << std::endl;
        }
    }


    return bestState;
//    return lNextStates[rand() % lNextStates.size()];
}

int Player::minimax(const GameState &state, uint8_t player, int depth)
{
    int bestPossible;
    int v;
    std::vector<GameState> childStates;
//    std::vector<GameState>::iterator it;

    // Finds all the possible children states
    state.findPossibleMoves(childStates);


    // If terminal state or the depth , then give back the evaluation sum
    if (childStates.size() == 0 || depth == 0)
    {
        int eval = evaluation(state);
        return eval;
    }


    //Else for the children for the state
    else
    {
        // For Player max_p we are maximizing
        if (player == max_p)
        {
            bestPossible = -1000000;
            for(unsigned int i = 0; i<childStates.size(); i++)
            {
                v = minimax(childStates[i], min_p, depth - 1);
                bestPossible = std::max(bestPossible, v);
            }
            return bestPossible;
        }

        // For Player min_p we are minimizing
        else
        {
            bestPossible = 1000000;
            for(unsigned int i = 0; i<childStates.size(); i++)
            {
                v = minimax(childStates[i], max_p, depth - 1);
                bestPossible = std::min(bestPossible, v);
            }
            return bestPossible;
        }
    }
}

int Player::evaluation(const GameState &state)
{
    /** Evaluation function: Search for the number of X or O in a line.
    for X (MAX_turn),
    +1000 points for 4-in-line
     +100 points for 3-in-line
      +10 points for 2-in-line
       +1 points for 1-in line
    for O (MIN_turn),
    -1000 points for 4-in-line
     -100 points for 3-in-line
      -10 points for 2-in-line
       -1 points for 1-in line

    Sum the score for all possible lines.
    Check for all rows, column, and diagonals.
    4-rows, 4-column, 2-diagonal
    and return that

 *    col 0  1  2  3
 * row  ---------------
 *  0  |  .  .  .  .  |  0
 *  1  |  .  .  .  .  |  1
 *  2  |  .  .  .  .  |  2
 *  3  |  .  .  .  .  |  3
 *      ---------------
 *        0  1  2  3

    */

    /** Problem with the above method that it cannot check for
    3 consecutive X. It will give them same score to all 3 X
    Have to change to give more weight to consecutive X
    */

    int score = 0; // Total score of the evaluation function

    int num_x = 0;  // To check how many X
    int num_o = 0;  // To check how many O



    // Row-wise check
    for(int i = 0; i<4; i++)
    {
        num_x = 0;
        num_o = 0;
        for(int j = 0; j<4; j++)
        {
            // Checking how many X are there in each row
            if(state.at(i, j) == CELL_X)
                num_x++;
            // Checking how many O are there in each column
            if(state.at(i, j)== CELL_O)
                num_o++;
        }

        // Calculation of score
        if(num_x == 4)
            score += 1000;
        else if(num_x == 3)
            score += 100;
        else if(num_x == 2)
            score += 10;
        else if(num_x == 1)
            score += 1;

        if(num_o == 4)
            score -=1000;
        else if(num_o == 3)
            score -= 100;
        else if(num_o == 2)
            score -= 10;
        else if(num_o == 1)
            score -= 1;
    }
    // Make num again to zero so as to use again
    num_o = 0;
    num_x = 0;







    // Column wise check
    for(int i = 0; i<4; i++)
    {
        num_x = 0;
        num_o = 0;
        for(int j = 0; j<4; j++)
        {
            // Checking how many X are there in each row
            if(state.at(j, i) == CELL_X)
                num_x++;
            // Checking how many O are there in each column
            if(state.at(j, j)== CELL_O)
                num_o++;
        }

        // Calculation of score
        if(num_x == 4)
            score += 1000;
        else if(num_x == 3)
            score += 100;
        else if(num_x == 2)
            score += 10;
        else if(num_x == 1)
            score += 1;

        if(num_o == 4)
            score -=1000;
        else if(num_o == 3)
            score -= 100;
        else if(num_o == 2)
            score -= 10;
        else if(num_o == 1)
            score -= 1;
    }
    // Make num again to zero so as to use again
    num_o = 0;
    num_x = 0;








    // Left diagonal check
    for(int i = 0; i<4; i++)
    {
        // Checking how many X are there in diagonal
        if(state.at(i, i) == CELL_X)
            num_x++;
        // Checking how many O are there in diagonal
        if(state.at(i, i)== CELL_O)
            num_o++;
    }
    // Calculation of score
    if(num_x == 4)
        score += 1000;
    else if(num_x == 3)
        score += 100;
    else if(num_x == 2)
        score += 10;
    else if(num_x == 1)
        score += 1;

    if(num_o == 4)
        score -=1000;
    else if(num_o == 3)
        score -= 100;
    else if(num_o == 2)
        score -= 10;
    else if(num_o == 1)
        score -= 1;

    // Make num again to zero so as to use again
    num_o = 0;
    num_x = 0;








    // Right diagonal check
    for(int i = 0; i<4; i++)
    {
        // Checking how many X are there in diagonal
        if(state.at(i, 3-i) == CELL_X)
            num_x++;
        // Checking how many O are there in diagonal
        if(state.at(i, 3-i)== CELL_O)
            num_o++;
    }
    // Calculation of score
    if(num_x == 4)
        score += 1000;
    else if(num_x == 3)
        score += 100;
    else if(num_x == 2)
        score += 10;
    else if(num_x == 1)
        score += 1;

    if(num_o == 4)
        score -=1000;
    else if(num_o == 3)
        score -= 100;
    else if(num_o == 2)
        score -= 10;
    else if(num_o == 1)
        score -= 1;





    return score;
}


/*namespace TICTACTOE*/ }
