#include "ticktack/tt.h"
#include "ticktack/fixed_stack.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <unordered_set>

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
            return score_win;
        else if (result.value_or(Board::CHAR_VOID) != Board::CHAR_VOID)
            return score_loss;
        int score = 0;
        for(auto const & t: Board::triplets) {
            score += evaluate_triplet(board, t, player);
        }
        return score;
    }

    // https://de.wikipedia.org/wiki/Minimax-Algorithmus
    int ComputerPlayer::next_move(Board& board, char player, int return_depth, int depth, std::optional<Board::Position>& best_move)
    {
        if (depth == 0 || board.check_winner().has_value() || board.possible_moves().empty())
            return evaluate(board, player);
        int max_score = std::numeric_limits<int>::lowest();
        ticktock::fixed_stack<Board::Position, 9> moves;
        board.possible_moves(std::back_inserter(moves));
        while(!moves.empty()) {
            auto move = moves.top();
            moves.pop();
            board.set(move, player);
            auto score = -next_move(board, Board::other_player(player), return_depth, depth - 1, best_move);
            board.set(move, Board::CHAR_VOID);
            if (score > max_score) {
                max_score = score;
                if (depth == return_depth)
                    best_move = move;
            }
        }
        return max_score;
    }

    auto ComputerPlayer::next_move(Board board, char player) -> std::optional<Board::Position> {
        std::optional<Board::Position> best_move;
        [[maybe_unused]] auto score = next_move(board, player, depth_, depth_, best_move);
        return best_move;
    }

    void BoardTUIControllerView::print(Board const & board) {
        auto c = [&](int i) -> char const & { return board.at(Board::Position::all_positions[i]); };
        std::cout << ' ' << c(0) << " | " << c(1) << " | " << c(2) << '\n';
        std::cout << ' ' << c(3) << " | " << c(4) << " | " << c(5) << '\n';
        std::cout << ' ' << c(6) << " | " << c(7) << " | " << c(8) << '\n';
        std::cout << '\n';
    }

    auto BoardTUIControllerView::next_human_move(Board const & board)
    -> std::optional<Board::Position> {
        std::unordered_set<Board::Position> possible_moves;
        board.possible_moves(std::inserter(possible_moves, possible_moves.end()));
        if (possible_moves.size() == 0) {
            std::cout << "No more possible moves!\n";
            return {};
        }
        auto p = [&](int i) -> char {
            return possible_moves.contains(Board::Position::all_positions[i]) ? i + '0' : ' ';
        };
        std::cout << ' ' << p(0) << " | " << p(1) << " | " << p(2) << '\n';
        std::cout << ' ' << p(3) << " | " << p(4) << " | " << p(5) << '\n';
        std::cout << ' ' << p(6) << " | " << p(7) << " | " << p(8) << '\n';
        std::cout << "Press number of your next move for X: ";
        std::cout << '\n';
        int move_idx;
        do {
            std::cin >> move_idx;
            move_idx = std::clamp(move_idx, 0, (int)Board::Position::all_positions.size());
        } while(!possible_moves.contains(Board::Position::all_positions[move_idx]));
        return Board::Position::all_positions[move_idx];
    }

    void BoardTUIControllerView::announce_winner(char player) {
        std::cout << "*** Winner is: " << player << " ***\n";
    }

    Game Game::create_game() {
        Game game;
        game.board_ = std::make_unique<Board>();
        game.controller_view_ = std::make_unique<BoardTUIControllerView>();
        game.computer_player_ = std::make_unique<ComputerPlayer>();
        game.board().clear();
        return game;
    }

}