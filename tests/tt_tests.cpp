#include <algorithm>
#include <initializer_list>
#include <random>
#include <stdexcept>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "ticktack/tt.h"

auto make_randomizer(std::string&& s) {
    std::mt19937_64 rnd(std::random_device{}());
    std::uniform_int_distribution<size_t> dist(0, s.length() - 1);
    return [=, s=std::move(s)]() mutable { return s.at(dist(rnd)); };
}

auto board_randomizer() {
    static std::string chars = std::string() + ticktack::Board::CHAR_VOID
                    + ticktack::Board::CHAR_X + ticktack::Board::CHAR_O;
    static auto board_random = make_randomizer(std::move(chars));
    return board_random;
}

auto random_board() {
    ticktack::Board::data_type dt;
    std::ranges::generate(dt, board_randomizer());
    return dt;
}

TEST_CASE("Test class Board") {

    ticktack::Board b;

    SUBCASE("Test at() throws") {
        std::pair<unsigned, unsigned> test_cases[] = {
            {3 , 3}, {0, 3}, { 3, 0}, {3333, 0}
        };
        for(auto const & e: test_cases)
            CHECK_THROWS_AS(b.at(e.first, e.second), std::out_of_range);
    }
    SUBCASE("Test at() nothrows") {
        for(unsigned y : { 0u, 1u, 2u})
            for(unsigned x : {0u, 1u, 2u})
                CHECK_NOTHROW(b.at(x, y));
    }
}

TEST_CASE("Check set_state") {
    ticktack::Board::data_type dt;
    ticktack::Board b;
    static constexpr size_t test_cnt = 10;

    for(size_t i = 0; i < test_cnt; ++i) {
        dt = random_board();

        b.set_state(dt);
        for(unsigned y : { 0u, 1u , 2u})
            for(unsigned x : {0u, 1u, 2u})
                CHECK(dt[y * 3 + x ] == b.at(x, y));
    }
}

TEST_CASE("Check clear") {
    ticktack::Board::data_type dt;
    ticktack::Board b;
    static constexpr size_t test_cnt = 10;

    for(size_t i = 0; i < test_cnt; ++i) {
        dt = random_board();
        // std::ranges::generate(dt, board_randomizer());

        b.set_state(dt);
        b.clear();
        for(unsigned y : { 0u, 1u, 2u})
            for(unsigned x : {0u, 1u, 2u})
                CHECK(b.at(x, y) == ticktack::Board::CHAR_VOID);
    }
}

TEST_CASE("Check win condition") {
    ticktack::Board b;
    auto three = [](ticktack::Board& b, char player, int dx, int dy, unsigned initial_x = 0, unsigned initial_y = 0) {
        unsigned x = initial_x;
        unsigned y = initial_y;
        for(auto i = 0u; i < 3u;
            ++i, x = (unsigned)((int)(x) + dx), y = (unsigned)((int)(y) + dy))
        {
            b.at(x, y) = player;
        }
    };
    SUBCASE("negative win condition") {
        b.clear();
        auto result = b.check_winner();
        CHECK(result == false);
        CHECK(result.has_value() == false);
    }
    SUBCASE("positive win condition horizontal") {
        for(auto player : {ticktack::Board::CHAR_O, ticktack::Board::CHAR_X}) {
            for(auto x: { 0u, 1u, 2u}) {
                b.clear();
                three(b, player, 0, 1, x);
                auto result = b.check_winner();
                CHECK(result == true);
                CHECK(result.value() == player);
            }
        }
    }
    SUBCASE("positive win condition vertical") {
        for(auto player : {ticktack::Board::CHAR_O, ticktack::Board::CHAR_X}) {
            for(auto y: { 0u, 1u, 2u}) {
                b.clear();
                three(b, player, 1, 0, 0, y);
                auto result = b.check_winner();
                CHECK(result == true);
                CHECK(result.value() == player);
            }
        }
    }
    SUBCASE("positive win condition diagonal") {
        for(auto player : {ticktack::Board::CHAR_O, ticktack::Board::CHAR_X}) {
            {
                // Check diagonal
                b.clear();
                three(b, player, 1, 1);
                auto result = b.check_winner();
                CHECK(result == true);
                CHECK(result.value() == player);
            }
            {
                // Check other diagonal
                b.clear();
                three(b, player, 1, -1, 0, 2);
                auto result = b.check_winner();
                CHECK(result == true);
                CHECK(result.value() == player);
            }
        }
    }

}

// TEST_CASE("Check win condition") {
//     ticktack::Board b;
//     ticktack::Board::data_type dt;
//     for(unsigned x = 0; x < 2; ++x) {
//         std::ranges::fill(dt, ' ');
//         for(unsigned y = 0; y < 2; ++y)
//             dt
//     }
// }