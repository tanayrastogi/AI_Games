#include "player.hpp"
#include <cstdlib>
#include <algorithm>

int iter = 0;

namespace TICTACTOE
{

GameState Player::play(const GameState &pState,const Deadline &pDue)
{
    std::cerr << "Processing " << pState.toMessage() << std::endl;

    int sum = 0;
    std::vector<GameState> lNextStates;



    pState.findPossibleMoves(lNextStates);
    if (lNextStates.size() == 0) return GameState(pState, Move());

    /*
     * Here you should write your clever algorithms to get the best next move, ie the best
     * next state. This skeleton returns a random move instead.
     */

     sum = minimax(pState, pState.getNextPlayer(), 10);
     std::cerr << "\nThe sum value is: "<< sum << std::endl;



    return lNextStates[rand() % lNextStates.size()];
}


int Player::minimax(const GameState &state, uint8_t player, int depth)
{
    int bestPossible;
    int v;
    int eval = 0;
    std::vector<GameState> childStates;
    std::vector<GameState>::iterator it;

    // Fins all the possible children states
    state.findPossibleMoves(childStates);
//    std::cerr<<"\nNumber of children for state:"<<childStates.size();


    // If terminal state, then give back the evaluation sum
    if (childStates.size() == 0 || depth == 0)
    {
//        iter++;
//        std::cerr<<"\nTerminal Node:"<<iter;
        eval = evaluation(state);
        return eval;
    }


    //Else for the children for the state
    else
    {
        // For Player X we are maximizing
        if (player == CELL_X)
        {
            bestPossible = -1000000;
            for(unsigned int i = 0; i<childStates.size(); i++)
            {
                v = minimax(childStates[i], CELL_O, depth - 1);
                bestPossible = std::max(bestPossible, v);
                std::cerr<<"\nThe best possible for X is: "<<bestPossible;
            }
            return bestPossible;
        }

        // For Player O we are minimizing
        else
        {
            bestPossible = 1000000;
            for(unsigned int i = 0; i<childStates.size(); i++)
            {
                v = minimax(childStates[i], CELL_X, depth - 1);
                bestPossible = std::min(bestPossible, v);
                std::cerr<<"\nThe best possible for O is: "<<bestPossible;
            }
            return bestPossible;
        }
    }
}

int Player::evaluation(const GameState &state)
{
    /** Evaluation function: Search for the number of X or O in a line.
    for X (MAX_turn),
    +100 points for 3-in-line
     +10 points for 2-in-line
      +1 points for 1-in line
    for O (MIN_turn),
    -100 points for 3-in-line
     -10 points for 2-in-line
      -1 points for 1-in line

    Sum the score for all possible lines.
    Check for all rows, column, and diagonals.
    4-rows, 4-column, 2-diagonal

 *    col 0  1  2  3
 * row  ---------------
 *  0  |  .  .  .  .  |  0
 *  1  |  .  .  .  .  |  1
 *  2  |  .  .  .  .  |  2
 *  3  |  .  .  .  .  |  3
 *      ---------------
 *        0  1  2  3

    */

    int score = 0;
    int num = 0;  // To check how many X or O are there in a line

    // Next Player
    uint8_t player = state.getNextPlayer();

    // Checking for X
    if(player == CELL_X)
    {
        // Row-wise check
        for(int i = 0; i<4; i++)
        {
            num = 0;
            for(int j = 0; j<4; j++)
            {
                // Checking how many X are there in each row
                if(state.at(i, j) == CELL_X)
                    num++;
            }
            // Calculation of score
            if(num == 3)
                score += 100*num;
            else if(num == 2)
                score += 10*num;
            else if(num == 1)
                score += 1*num;
        }
        // Make num again to zero so as to use it for column-wise check
        num = 0;


        // Column-wise check
        for(int i = 0; i<4; i++)
        {
            num = 0;
            for(int j = 0; j<4; j++)
            {
                // Checking how many X are there in each column
                if(state.at(j, i) == CELL_X)
                    num++;
            }
            // Calculation of score
            if(num == 3)
                score += 100*num;
            else if(num == 2)
                score += 10*num;
            else if(num == 1)
                score += 1*num;
        }
        // Make num again to zero so as to use it for diagonal-wise check
        num = 0;

        // Diagonal-wise check
        for(int i = 0; i<4; i++)
        {
            // Checking how many X are there in diagonal
            if(state.at(i, i) == CELL_X)
                num++;
         }
         // Calculation of score
        if(num == 3)
            score += 100*num;
        else if(num == 2)
            score += 10*num;
        else if(num == 1)
            score += 1*num;

        // Make num again to zero so as to use it for diagonal-wise check
        num = 0;


        // Off Diagonal check
        for(int i = 0; i<4; i++)
        {
            // Checking how many X are there in diagonal
            if(state.at(i, 3-i) == CELL_X)
                num++;
        }
        // Calculation of score
        if(num == 3)
            score += 100*num;
        else if(num == 2)
            score += 10*num;
        else if(num == 1)
            score += 1*num;

    }




    // Checking for O
    if(player == CELL_O)
    {
        // Row-wise check
        for(int i = 0; i<4; i++)
        {
            num = 0;
            for(int j = 0; j<4; j++)
            {
                // Checking how many X are there in each row
                if(state.at(i, j) == CELL_X)
                    num++;
            }
            // Calculation of score
            if(num == 3)
                score -= 100*num;
            else if(num == 2)
                score -= 10*num;
            else if(num == 1)
                score -= 1*num;
        }
        // Make num again to zero so as to use it for column-wise check
        num = 0;


        // Column-wise check
        for(int i = 0; i<4; i++)
        {
            num = 0;
            for(int j = 0; j<4; j++)
            {
                // Checking how many X are there in each column
                if(state.at(j, i) == CELL_X)
                    num++;
            }
            // Calculation of score
            if(num == 3)
                score -= 100*num;
            else if(num == 2)
                score -= 10*num;
            else if(num == 1)
                score -= 1*num;
        }
        // Make num again to zero so as to use it for diagonal-wise check
        num = 0;

        // Diagonal-wise check
        for(int i = 0; i<4; i++)
        {
            // Checking how many X are there in diagonal
            if(state.at(i, i) == CELL_X)
                num++;
         }
         // Calculation of score
        if(num == 3)
            score -= 100*num;
        else if(num == 2)
            score -= 10*num;
        else if(num == 1)
            score -= 1*num;

        // Make num again to zero so as to use it for diagonal-wise check
        num = 0;


        // Off Diagonal check
        for(int i = 0; i<4; i++)
        {
            // Checking how many X are there in diagonal
            if(state.at(i, 3-i) == CELL_X)
                num++;
        }
        // Calculation of score
        if(num == 3)
            score -= 100*num;
        else if(num == 2)
            score -= 10*num;
        else if(num == 1)
            score -= 1*num;

    }

    return score;
}


/*namespace TICTACTOE*/ }
