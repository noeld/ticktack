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

    const std::array<Board::Position, 9> Board::Position::all_positions = {
        Board::Position::top_left,
        Board::Position::top_middle,
        Board::Position::top_right,
        Board::Position::middle_left,
        Board::Position::middle_middle,
        Board::Position::middle_right,
        Board::Position::bottom_left,
        Board::Position::bottom_middle,
        Board::Position::bottom_right
    };


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

    int ComputerPlayer::evaluate_triplet(Board const & board, Board::Position const (&triplet)[3], char player) {
        char const other = Board::other_player(player);
        int score = 0;
        for(auto const & pos : triplet) {
            score += decode(board.at(pos), player, 1, other, -1, 0);
        }
        return score;
    }

    int ComputerPlayer::evaluate(Board const & board, char player) {
        auto result = board.check_winner();
        if (result.value_or(Board::CHAR_VOID) == player)
            return 10;
        else if (result.value_or(Board::CHAR_VOID) != Board::CHAR_VOID)
            return -10;

        return 0;
    }

    Board::Position ComputerPlayer::next_move(Board const & board) {
        return Board::Position::bottom_left;
    }

}