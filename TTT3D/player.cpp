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

	*/
	/** Cells are numbered as follows:
 	*			lay 0						lay 1						lay 2						lay 3
	 *    col 0  1  2  3  		    col 0  1  2  3  		    col 0  1  2  3  		    col 0  1  2  3
	 * row  ---------------		 row  ---------------		 row  ---------------		 row  ---------------
	 *  0  |  0  1  2  3  |  0	  0  | 16  17 18 19 |  0	  0  | 32  33 34 35 |  0	  0  | 48  49 50 51 |  0
	 *  1  |  4  5  6  7  |  1	  1  | 20  21 22 23 |  1	  1  | 36  37 38 39 |  1	  1  | 52  53 54 55 |  1
	 *  2  |  8  9  10 11 |  2	  2  | 24  25 26 27 |  2	  2  | 40  41 42 43 |  2	  2  | 56  57 58 59 |  2
	 *  3  | 12  13 14 15 |  3	  3  | 28  29 30 31 |  3	  3  | 44  45 46 47 |  3	  3  | 60  61 62 63 |  3
	 *      ---------------		      ---------------		      ---------------		      ---------------
	 *        0  1  2  3		        0  1  2  3		            0  1  2  3		            0  1  2  3

	 *
	 * The staring board looks like this:
	 *			lay 0           			lay 1						lay 2						lay 3
	 *    col 0  1  2  3            col 0  1  2  3              col 0  1  2  3              col 0  1  2  3
	 * row  ---------------      row  ---------------        row  ---------------        row  ---------------
	 *  0  |  .  .  .  .  |  0    0  |  .  .  .  .  |  0      0  |  .  .  .  .  |  0      0  |  .  .  .  .  |  0
	 *  1  |  .  .  .  .  |  1	  1  |  .  .  .  .  |  1	  1  |  .  .  .  .  |  1	  1  |  .  .  .  .  |  1
	 *  2  |  .  .  .  .  |  2    2  |  .  .  .  .  |  2      2  |  .  .  .  .  |  2      2  |  .  .  .  .  |  2
	 *  3  |  .  .  .  .  |  3    3  |  .  .  .  .  |  3      3  |  .  .  .  .  |  3      3  |  .  .  .  .  |  3
	 *      ---------------           ---------------             ---------------             ---------------
	 *        0  1  2  3                0  1  2  3                  0  1  2  3                  0  1  2  3
	 *
	 * O moves first.
	 */

    // i: layer index (3rd dimension)
    // j: Row in each face
    // k: Column in each face

    double score = 0;

    // If the state is winning state, return high value
    if(pState.isXWin())
        return infinity;
    else if(pState.isOWin())
        return -infinity;


    int num_x = 0;  // To check how many X
    int num_o = 0;  // To check how many O



    //### Row Wise Check for all the face ### //
    //####################################### //
    for(int i = 0; i<4; i++)
    {
		// Check every row
	    for(int j = 0; j<4; j++)
	    {
            num_x = 0;
            num_o = 0;
	        for(int k = 0; k<4; k++)
	    	{
                // Checking how many X are there in each row
                if(pState.at(i, j, k) == CELL_X)
                num_x++;
                // Checking how many O are there in each column
                if(pState.at(i, j, k) == CELL_O)
                num_o++;
	    	}

          // Update score
          score = score + update(num_x, num_o);
	    }
    }

    // Make num again to zero so as to use again
    num_o = 0;
    num_x = 0;





    //### Column Wise Check for all the face ### //
    //####################################### //
    for(int i = 0; i<4; i++)
    {
		// Check every row
	    for(int j = 0; j<4; j++)
	    {
            num_x = 0;
            num_o = 0;
	        for(int k = 0; k<4; k++)
	    	{
                // Checking how many X are there in each row
                if(pState.at(i, k, j) == CELL_X)
                num_x++;
                // Checking how many O are there in each column
                if(pState.at(i, k, j) == CELL_O)
                num_o++;
	    	}

          // Update score
          score = score + update(num_x, num_o);

	    }
    }

    // Make num again to zero so as to use again
    num_o = 0;
    num_x = 0;







    //### Check Face wise for all the face ### //
    //####################################### //
    for(int i = 0; i<4; i++)
    {
		// Check every row
	    for(int j = 0; j<4; j++)
	    {
            num_x = 0;
            num_o = 0;
	        for(int k = 0; k<4; k++)
	    	{
                // Checking how many X are there in each row
                if(pState.at(k, i, j) == CELL_X)
                num_x++;
                // Checking how many O are there in each column
                if(pState.at(k, i, j) == CELL_O)
                num_o++;
	    	}

          // Update score
          score = score + update(num_x, num_o);
	    }
    }

    // Make num again to zero so as to use again
    num_o = 0;
    num_x = 0;








    //### Check 2D diagonals for all the face ### //
    //####################################### //
    for(int i = 0; i<4; i++)
    {
        // ------------------- Face 1 ---------------- //
        // Check for left diagonal
        for(int j = 0; j<4; j++)
        {
            // Checking how many X are there in diagonal
            if(pState.at(i, j, j) == CELL_X)
                num_x++;
            // Checking how many O are there in diagonal
            if(pState.at(i, j, j)== CELL_O)
                num_o++;
        }

        // Update score
        score = score + update(num_x, num_o);

        // Make num again to zero so as to use again
        num_o = 0;
        num_x = 0;


        // Check for right diagonal
        for(int j = 0; j<4; j++)
        {
            // Checking how many X are there in diagonal
            if(pState.at(i, j, 3-j) == CELL_X)
                num_x++;
            // Checking how many O are there in diagonal
            if(pState.at(i, j, 3-j)== CELL_O)
                num_o++;
        }

        // Update score
        score = score + update(num_x, num_o);

        // Make num again to zero so as to use again
        num_o = 0;
        num_x = 0;





        // ------------------- Face 2 ---------------- //
        // Check for left diagonal
        for(int j = 0; j<4; j++)
        {
            // Checking how many X are there in diagonal
            if(pState.at(j, i, j) == CELL_X)
                num_x++;
            // Checking how many O are there in diagonal
            if(pState.at(j, i, j)== CELL_O)
                num_o++;
        }

        // Update score
        score = score + update(num_x, num_o);

        // Make num again to zero so as to use again
        num_o = 0;
        num_x = 0;

        // Check for right diagonal
        for(int j = 0; j<4; j++)
        {
            // Checking how many X are there in diagonal
            if(pState.at(j, i, 3-j) == CELL_X)
                num_x++;
            // Checking how many O are there in diagonal
            if(pState.at(j, i, 3-j)== CELL_O)
                num_o++;
        }

        // Update score
        score = score + update(num_x, num_o);

        // Make num again to zero so as to use again
        num_o = 0;
        num_x = 0;



        // ------------------- Face 3 ---------------- //
        // Check for left diagonal
        for(int j = 0; j<4; j++)
        {
            // Checking how many X are there in diagonal
            if(pState.at(j, j, i) == CELL_X)
                num_x++;
            // Checking how many O are there in diagonal
            if(pState.at(j, j, i)== CELL_O)
                num_o++;
        }

        // Update score
        score = score + update(num_x, num_o);

        // Make num again to zero so as to use again
        num_o = 0;
        num_x = 0;


        // Check for right diagonal
        for(int j = 0; j<4; j++)
        {
            // Checking how many X are there in diagonal
            if(pState.at(j, j-3, i) == CELL_X)
                num_x++;
            // Checking how many O are there in diagonal
            if(pState.at(j, j-3, i)== CELL_O)
                num_o++;
        }

        // Update score
        score = score + update(num_x, num_o);

        // Make num again to zero so as to use again
        num_o = 0;
        num_x = 0;

    }





    //### Check 3D diagonals for all the face ### //
    //####################################### //

    // Diagonal 1
    for(int i = 0; i<4; i++)
    {
        // Checking how many X are there in diagonal
        if(pState.at(i, i, i) == CELL_X)
            num_x++;
        // Checking how many O are there in diagonal
        if(pState.at(i, i, i)== CELL_O)
            num_o++;
     }

    // Update score
    score = score + update(num_x, num_o);

    // Make num again to zero so as to use again
    num_o = 0;
    num_x = 0;



    // Diagonal 2
    for(int i = 0; i<4; i++)
    {
        // Checking how many X are there in diagonal
        if(pState.at(3 - i, i, i) == CELL_X)
            num_x++;
        // Checking how many O are there in diagonal
        if(pState.at(3 - i, i, i)== CELL_O)
            num_o++;
     }

    // Update score
    score = score + update(num_x, num_o);

    // Make num again to zero so as to use again
    num_o = 0;
    num_x = 0;


    // Diagonal 3
    for(int i = 0; i<4; i++)
    {
        // Checking how many X are there in diagonal
        if(pState.at(i, 3 - i, i) == CELL_X)
            num_x++;
        // Checking how many O are there in diagonal
        if(pState.at(i, 3 - i, i)== CELL_O)
            num_o++;
     }

    // Update score
    score = score + update(num_x, num_o);

    // Make num again to zero so as to use again
    num_o = 0;
    num_x = 0;


    // Diagonal 4
    for(int i = 0; i<4; i++)
    {
        // Checking how many X are there in diagonal
        if(pState.at(i, i, 3 - i) == CELL_X)
            num_x++;
        // Checking how many O are there in diagonal
        if(pState.at(i, i, 3 - i)== CELL_O)
            num_o++;
     }

    // Update score
    score = score + update(num_x, num_o);



	// This is the final score of the current table
	return score;
}




double update(int num_x, int num_o)
{
    double score = 0;

    // Update score
    if ((num_x != 0) && (num_o == 0))
    {
        score = score + pow(10, num_x);
        if(num_x == 4)
            score = score + infinity;
    }

    else if ((num_o != 0) && (num_x == 0))
    {
        score = score - pow(10, num_x);
        if(num_x == 4)
            score = score - infinity;
    }
    return score;
}


/*namespace TICTACTOE3D*/ }
