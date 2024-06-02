#include "ticktack/tt.h"

namespace ticktack {

    Board::Position const Board::Position::top_left      {0, 0};
    Board::Position const Board::Position::top_middle    {1, 0};
    Board::Position const Board::Position::top_right     {2, 0};
    Board::Position const Board::Position::middle_left   {0, 1};
    Board::Position const Board::Position::middle_middle {1, 1};
    Board::Position const Board::Position::middle_right  {2, 1};
    Board::Position const Board::Position::bottom_left   {0, 2};
    Board::Position const Board::Position::bottom_middle {1, 2};
    Board::Position const Board::Position::bottom_right  {2, 2};

    const Board::Position Board::triplets[8][3] = {
        { Position::top_left, Position::top_middle, Position::top_right},
        { Position::middle_left, Position::middle_middle, Position::middle_right},
        { Position::bottom_left, Position::bottom_middle, Position::bottom_right},
        { Position::top_left, Position::middle_left, Position::bottom_left},
        { Position::top_middle, Position::middle_middle, Position::bottom_middle},
        { Position::top_right, Position::middle_right, Position::bottom_right},
        { Position::top_left, Position::middle_middle, Position::bottom_right},
        { Position::top_right, Position::middle_middle, Position::bottom_left}
    };

}