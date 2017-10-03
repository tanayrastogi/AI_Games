#include "player.hpp"
#include <cstdlib>
#include <algorithm>
#include <math.h>

namespace TICTACTOE3D
{

GameState Player::play(const GameState &pState,const Deadline &pDue)
{
    //std::cerr << "Processing " << pState.toMessage() << std::endl;
	// Utility value
    double v;
    int depth = 2;
    double alpha = -100000000;
    double beta  =  100000000;

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

    // Otherwise running alphabeta for max player
    double bestValue = -1000000;
    for(unsigned int i = 0; i<lNextStates.size(); i++)
    {
        std::cerr<<"\nRunning the alpha beta";
        v = alphabeta(lNextStates[i], min_p, depth-1, alpha, beta);
        if(v > bestValue)
        {
            bestValue = v;
            bestState = lNextStates[i];
            std::cerr<<"\nThe best value is: "<<bestValue;
            std::cerr<<"\nThe best state is: "<<i;
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
	if (!depth || !childStates.size())
	{
		v = evaluation(pState);
		std::cerr<<"\n The evaluation we get: "<<v;
	}

	// If player is MAX (X-player). We want X to win.
	else if (player == max_p)
	{

        v = -100000000;
        // For every child
        for(unsigned int i = 0; i<childStates.size(); i++)
        {
            std::cerr<<"\nFor the max player";
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
        v = +100000000;
        // For every child
        for(unsigned int i = 0; i<childStates.size(); i++)
        {
            std::cerr<<"\n For the min player";
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
	/* Cells are numbered as follows:
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

//    int total_score = 0; // Total score of the evaluation function

    // i: row index in current layer
    // j: col index in current layer
    // k: layer index (3rd dimension)

    double score = 0;

    int num_x = 0;  // To check how many X
    int num_o = 0;  // To check how many O

    //############################################################
    //#######SCAN k-WISE##########################################
    //############################################################
    for(int k = 0; k<4; k++)
    {
		// Check every row
	    num_x = 0;
	    num_o = 0;
	    for(int i = 0; i<4; i++)
	    {
	        for(int j = 0; j<4; j++)
	    	{
                // Checking how many X are there in each row
                if(pState.at(i, j, k) == CELL_X)
                num_x++;
                // Checking how many O are there in each column
                if(pState.at(i, j, k) == CELL_O)
                num_o++;
	    		//pState.at(i, j, k) == CELL_X ? num_x++ : num_o++;
	    	}

	        // Update score
	        score += pow(10, num_x - 1) - pow(10, num_o - 1);
	    }

	    // Check every column
	    num_x = 0;
	    num_o = 0;
	    for(int j = 0; j<4; j++)
	    {
	        for(int i = 0; i<4; i++)
	    	{
                // Checking how many X are there in each row
                if(pState.at(i, j, k) == CELL_X)
                num_x++;
                // Checking how many O are there in each column
                if(pState.at(i, j, k) == CELL_O)
                num_o++;
	    		//pState.at(i, j, k) == CELL_X ? num_x++ : num_o++;
	    	}

	        // Update score
	        score += pow(10, num_x - 1) - pow(10, num_o - 1);
	    }

	    // Check every layer-diagonal
	    num_x = 0;
	    num_o = 0;
        for(int i = 0; i<4; i++)
    	{
            // Checking how many X are there in each row
            if(pState.at(i, j, k) == CELL_X)
            num_x++;
            // Checking how many O are there in each column
            if(pState.at(i, j, k) == CELL_O)
            num_o++;
    		//pState.at(i, i, k) == CELL_X ? num_x++ : num_o++;
    	}

        // Update score
        score += pow(10, num_x - 1) - pow(10, num_o - 1);

	   	// Check secondary diagonal
	    num_x = 0;
	    num_o = 0;
        for(int i = 0; i<4; i++)
    	{
            // Checking how many X are there in each row
            if(pState.at(i, 3-i, k) == CELL_X)
            num_x++;
            // Checking how many O are there in each column
            if(pState.at(i, 3-i, k) == CELL_O)
            num_o++;
    		//pState.at(i, 3-i, k) == CELL_X ? num_x++ : num_o++;
    	}

        // Update score
        score += pow(10, num_x - 1) - pow(10, num_o - 1);
	}


    //############################################################
    //#######SCAN i-WISE##########################################
    //############################################################
    for(int i = 0; i<4; i++)
    {
		// Check every row
	    num_x = 0;
	    num_o = 0;
	    for(int k = 0; k<4; k++)
	    {
	        for(int j = 0; j<4; j++)
	    	{
                // Checking how many X are there in each row
                if(pState.at(i, j, k) == CELL_X)
                num_x++;
                // Checking how many O are there in each column
                if(pState.at(i, j, k) == CELL_O)
                num_o++;
	    		//pState.at(i, j, k) == CELL_X ? num_x++ : num_o++;
	    	}
	    }

        // Update score
        score += pow(10, num_x - 1) - pow(10, num_o - 1);

	    // Check every column
	    num_x = 0;
	    num_o = 0;
	    for(int j = 0; j<4; j++)
	    {
	        for(int k = 0; k<4; k++)
	    	{
                // Checking how many X are there in each row
                if(pState.at(i, j, k) == CELL_X)
                num_x++;
                // Checking how many O are there in each column
                if(pState.at(i, j, k) == CELL_O)
                num_o++;
	    		//pState.at(i, j, k) == CELL_X ? num_x++ : num_o++;
	    	}
	    }

        // Update score
        score += pow(10, num_x - 1) - pow(10, num_o - 1);

	    // Check main diagonal
	    num_x = 0;
	    num_o = 0;
        for(int k = 0; k<4; k++)
    	{
            // Checking how many X are there in each row
            if(pState.at(i, k, k) == CELL_X)
            num_x++;
            // Checking how many O are there in each column
            if(pState.at(i, k, k) == CELL_O)
            num_o++;
    		//pState.at(i, k, k) == CELL_X ? num_x++ : num_o++;
    	}

        // Update score
        score += pow(10, num_x - 1) - pow(10, num_o - 1);

	   	// Check secondary diagonal
	    num_x = 0;
	    num_o = 0;
        for(int k = 0; k<4; k++)
    	{
            // Checking how many X are there in each row
            if(pState.at(i, k, 3-k) == CELL_X)
            num_x++;
            // Checking how many O are there in each column
            if(pState.at(i, k, 3-k) == CELL_O)
            num_o++;
    		//pState.at(i, k, 3-k) == CELL_X ? num_x++ : num_o++;
    	}

        // Update score
        score += pow(10, num_x - 1) - pow(10, num_o - 1);

	}

    //############################################################
    //#######SCAN j-WISE##########################################
    //############################################################
    for(int j = 0; j<4; j++)
    {
		// Check every row
	    num_x = 0;
	    num_o = 0;
	    for(int i = 0; i<4; i++)
	    {
	        for(int k = 0; k<4; k++)
	    	{
                // Checking how many X are there in each row
                if(pState.at(i, j, k) == CELL_X)
                num_x++;
                // Checking how many O are there in each column
                if(pState.at(i, j, k) == CELL_O)
                num_o++;
	    		//pState.at(i, j, k) == CELL_X ? num_x++ : num_o++;
	    	}
	    }

        // Update score
        score += pow(10, num_x - 1) - pow(10, num_o - 1);

	    // Check every column
	    num_x = 0;
	    num_o = 0;
	    for(int k = 0; k<4; k++)
	    {
	        for(int i = 0; i<4; i++)
	    	{
                // Checking how many X are there in each row
                if(pState.at(i, j, k) == CELL_X)
                num_x++;
                // Checking how many O are there in each column
                if(pState.at(i, j, k) == CELL_O)
                num_o++;
	    		//pState.at(i, j, k) == CELL_X ? num_x++ : num_o++;
	    	}
	    }

        // Update score
        score += pow(10, num_x - 1) - pow(10, num_o - 1);

	    // Check main diagonal
	    num_x = 0;
	    num_o = 0;
        for(int i = 0; i<4; i++)
    	{
            // Checking how many X are there in each row
            if(pState.at(i, j, i) == CELL_X)
            num_x++;
            // Checking how many O are there in each column
            if(pState.at(i, j, i) == CELL_O)
            num_o++;
    		//pState.at(i, j, i) == CELL_X ? num_x++ : num_o++;
    	}

        // Update score
        score += pow(10, num_x - 1) - pow(10, num_o - 1);

	   	// Check secondary diagonal
	    num_x = 0;
	    num_o = 0;
        for(int i = 0; i<4; i++)
    	{
            // Checking how many X are there in each row
            if(pState.at(3-i, j, i) == CELL_X)
            num_x++;
            // Checking how many O are there in each column
            if(pState.at(3-i, j, i) == CELL_O)
            num_o++;
    		//pState.at(3-i, j, i) == CELL_X ? num_x++ : num_o++;
    	}

        // Update score
        score += pow(10, num_x - 1) - pow(10, num_o - 1);
	}

    //############################################################
    //#######EVALUATE 3D DIAGONALS################################
    //############################################################
    //3D-Diagonal 1
    num_x = 0;
    num_o = 0;
	for(int i = 0; i<4; i++)
	{
        // Checking how many X are there in each row
        if(pState.at(i, i, i) == CELL_X)
        num_x++;
        // Checking how many O are there in each column
        if(pState.at(i, i, i) == CELL_O)
        num_o++;
		//pState.at(i, i, i) == CELL_X ? num_x++ : num_o++;
	}

    // Update score
    score += pow(10, num_x - 1) - pow(10, num_o - 1);

    //3D-Diagonal 2
    num_x = 0;
    num_o = 0;
	for(int i = 0; i<4; i++)
	{
        // Checking how many X are there in each row
        if(pState.at(3-i, i, i) == CELL_X)
        num_x++;
        // Checking how many O are there in each column
        if(pState.at(3-i, i, i) == CELL_O)
        num_o++;
		//pState.at(3-i, i, i) == CELL_X ? num_x++ : num_o++;
	}

    // Update score
    score += pow(10, num_x - 1) - pow(10, num_o - 1);

    //3D-Diagonal 3
    num_x = 0;
    num_o = 0;
	for(int i = 0; i<4; i++)
	{
        // Checking how many X are there in each row
        if(pState.at(i, 3-i, i) == CELL_X)
        num_x++;
        // Checking how many O are there in each column
        if(pState.at(i, 3-i, i) == CELL_O)
        num_o++;
		//pState.at(i, 3-i, i) == CELL_X ? num_x++ : num_o++;
	}

    // Update score
    score += pow(10, num_x - 1) - pow(10, num_o - 1);

    //3D-Diagonal 4
    num_x = 0;
    num_o = 0;
	for(int i = 0; i<4; i++)
	{
        // Checking how many X are there in each row
        if(pState.at(i, i, 3-i) == CELL_X)
        num_x++;
        // Checking how many O are there in each column
        if(pState.at(i, i, 3-i) == CELL_O)
        num_o++;
		//pState.at(i, i, 3-i) == CELL_X ? num_x++ : num_o++;
	}

    // Update score
    score += pow(10, num_x - 1) - pow(10, num_o - 1);


	// This is the final score of the current table
	return score;
}


/*namespace TICTACTOE3D*/ }
