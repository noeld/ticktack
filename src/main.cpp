#include "ticktack/tt.h"
#include <cctype>
#include <exception>
#include <iostream>
#include <string>


void usage(char const * prg) {
    std::cout << "Usage: " << prg << " [options]\n";
    std::cout << R"*(
    Options:
    -h    show help
    -c    computer moves first (default: human)
    -<n>  n = 1..9 : set strength of computer (1 = weak, 9 = strong)
          default=4
)*";
}

int main(int argc, char const *argv[])
{
    static constexpr int exit_win = 0;
    static constexpr int exit_loss = 1;
    static constexpr int exit_draw = 2;
    static constexpr int exit_usage = 3;
    static constexpr int exit_exception = 4;
    int ret = 0;

    char initial_player = ticktack::Board::CHAR_X;

    try {

        ticktack::Game game = ticktack::Game::create_game();

        for(auto i = 1; i < argc; ++i) {
            std::string_view a(argv[i]);
            if (a == std::string("-c"))
                initial_player = ticktack::Board::CHAR_O;
            else if (a == std::string("-h")) {
                usage(argv[0]);
                return exit_usage;
            } else if ((a.size() == 2) && (a[0] == '-') and std::isdigit(a[1]) ) {
                int new_depth = -std::stoi(std::string(a));
                game.computer_player().set_depth(new_depth);
            } else {
                usage(argv[0]);
                return exit_usage;
            }
        }

        for(char player = initial_player;
            !game.board().check_winner().has_value();
            player = ticktack::Board::other_player(player))
        {
            game.controller_view().print(game.board());
            auto next_move = (player == ticktack::Board::CHAR_X)
            ? game.controller_view().next_human_move(game.board())
            : game.computer_player().next_move(game.board(), player);
            if (next_move.has_value())
                game.board().set(next_move.value(), player);
            else
                break;
        }

        game.controller_view().print(game.board());

        auto winner = game.board().check_winner();
        game.controller_view().announce_result(winner);
        if (winner.has_value()) {
            switch(winner.value()) {
            case ticktack::Board::CHAR_X:
                ret = exit_win;
                break;
            case ticktack::Board::CHAR_O:
                ret = exit_loss;
                break;
            }
        } else {
            ret = exit_draw;
        }
    } catch(std::exception const & e) {
        std::cout << "Exception: " << e.what() << '\n';
        ret = exit_exception;
    }
    return ret;
}