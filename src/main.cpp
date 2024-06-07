#include "ticktack/tt.h"

int main(int argc, char const *argv[])
{
    int ret = 0;

    ticktack::Game game = ticktack::Game::create_game();
    for(char player = ticktack::Board::CHAR_X; !game.board().check_winner().has_value(); player = ticktack::Board::other_player(player)) {
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
    if (winner.has_value()) {
        game.controller_view().announce_winner(winner.value());
    }

    return ret;
}