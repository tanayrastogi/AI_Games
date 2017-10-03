#ifndef _TICTACTOE_PLAYER_HPP_
#define _TICTACTOE_PLAYER_HPP_

#include "constants.hpp"
#include "deadline.hpp"
#include "move.hpp"
#include "gamestate.hpp"
#include <vector>

namespace TICTACTOE3D
{

class Player
{
public:
    ///perform a move
    ///\param pState the current state of the board
    ///\param pDue time before which we must have returned
    ///\return the next state the board is in after our move
    uint8_t max_p;
    uint8_t min_p;

    GameState play(const GameState &pState, const Deadline &pDue);
    double alphabeta(const GameState &pState, uint8_t player, int depth, double alpha, double beta);
    double evaluation(const GameState &state);
};

/*namespace TICTACTOE*/ }

#endif
