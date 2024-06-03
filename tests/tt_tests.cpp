#include <algorithm>
#include <initializer_list>
#include <limits>
#include <optional>
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
        CHECK(result.has_value() == false);
    }
    SUBCASE("positive win condition horizontal") {
        for(auto player : {ticktack::Board::CHAR_O, ticktack::Board::CHAR_X}) {
            for(auto x: { 0u, 1u, 2u}) {
                b.clear();
                three(b, player, 0, 1, x);
                auto result = b.check_winner();
                CHECK((bool)(result) == true);
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
                CHECK((bool)(result) == true);
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
                CHECK((bool)(result) == true);
                CHECK(result.value() == player);
            }
            {
                // Check other diagonal
                b.clear();
                three(b, player, 1, -1, 0, 2);
                auto result = b.check_winner();
                CHECK((bool)(result) == true);
                CHECK(result.value() == player);
            }
        }
    }

}

TEST_CASE("Check Position") {
    auto const &top_left =  ticktack::Board::Position::top_left;
    auto const &top_middle = ticktack::Board::Position::top_middle;
    auto const &top_right = ticktack::Board::Position::top_right;
    auto const &middle_left = ticktack::Board::Position::middle_left;
    auto const &middle_middle = ticktack::Board::Position::middle_middle;
    auto const &middle_right = ticktack::Board::Position::middle_right;
    auto const &bottom_left = ticktack::Board::Position::bottom_left;
    auto const &bottom_middle = ticktack::Board::Position::bottom_middle;
    auto const &bottom_right = ticktack::Board::Position::bottom_right;

    CHECK(top_left.up()         == top_left);
    CHECK(top_left.right()      == top_middle);
    CHECK(top_left.down()       == middle_left);
    CHECK(top_left.left()       == top_left);
    CHECK(top_middle.up()       == top_middle);
    CHECK(top_middle.right()    == top_right);
    CHECK(top_middle.down()     == middle_middle);
    CHECK(top_middle.left()     == top_left);
    CHECK(top_right.up()        == top_right);
    CHECK(top_right.right()     == top_right);
    CHECK(top_right.down()      == middle_right);
    CHECK(top_right.left()      == top_middle);
    CHECK(middle_left.up()      == top_left);
    CHECK(middle_left.right()   == middle_middle);
    CHECK(middle_left.down()    == bottom_left);
    CHECK(middle_left.left()    == middle_left);
    CHECK(middle_middle.up()    == top_middle);
    CHECK(middle_middle.right() == middle_right);
    CHECK(middle_middle.down()  == bottom_middle);
    CHECK(middle_middle.left()  == middle_left);
    CHECK(middle_right.up()     == top_right);
    CHECK(middle_right.right()  == middle_right);
    CHECK(middle_right.down()   == bottom_right);
    CHECK(middle_right.left()   == middle_middle);
    CHECK(bottom_left.up()      == middle_left);
    CHECK(bottom_left.right()   == bottom_middle);
    CHECK(bottom_left.down()    == bottom_left);
    CHECK(bottom_left.left()    == bottom_left);
    CHECK(bottom_middle.up()    == middle_middle);
    CHECK(bottom_middle.right() == bottom_right);
    CHECK(bottom_middle.down()  == bottom_middle);
    CHECK(bottom_middle.left()  == bottom_left);
    CHECK(bottom_right.up()     == middle_right);
    CHECK(bottom_right.right()  == bottom_right);
    CHECK(bottom_right.down()   == bottom_right);
    CHECK(bottom_right.left()   == bottom_middle);

}

TEST_CASE("Check validate_char") {
    for (int c = std::numeric_limits<char>::lowest();
        c <= std::numeric_limits<char>::max();
        ++c) {
        char cc = static_cast<char>(c);
        switch(cc) {
        case ticktack::Board::CHAR_VOID: [[ fallthrough]];
        case ticktack::Board::CHAR_O: [[fallthrough]];
        case ticktack::Board::CHAR_X:
            CHECK_NOTHROW(ticktack::Board::validate_char(cc));
            break;
        default:
            CHECK_THROWS_AS(ticktack::Board::validate_char(cc), std::out_of_range);
        }
    }
}

TEST_CASE("Check set") {
    ticktack::Board b;
    SUBCASE("Positive set test") {
        for(auto const & pos : ticktack::Board::Position::all_positions) {
            for(auto c : ticktack::Board::valid_chars) {
                b.clear();
                REQUIRE(b.at(pos) == ticktack::Board::CHAR_VOID);
                CHECK_NOTHROW(b.set(pos, c));
                CHECK(b.at(pos) == c);
            }
        }
    }
    SUBCASE("Negative set test") {
        auto pos = ticktack::Board::Position::all_positions[0];
        b.clear();
        REQUIRE(b.at(pos) == ticktack::Board::CHAR_VOID);
        char const invalid_char = 'k';
        CHECK_THROWS_AS(b.set(pos, invalid_char), std::out_of_range);
        CHECK(b.at(pos) == ticktack::Board::CHAR_VOID);
    }
}

TEST_CASE("Check other_player") {
    for (int c = std::numeric_limits<char>::lowest();
        c <= std::numeric_limits<char>::max();
        ++c) {
        char cc = static_cast<char>(c);
        switch(cc) {
        case ticktack::Board::CHAR_VOID:
            CHECK_NOTHROW(ticktack::Board::other_player(cc));
            CHECK(ticktack::Board::CHAR_VOID == ticktack::Board::other_player(cc));
            break;
        case ticktack::Board::CHAR_O:
            CHECK_NOTHROW(ticktack::Board::other_player(cc));
            CHECK(ticktack::Board::CHAR_X == ticktack::Board::other_player(cc));
            break;
        case ticktack::Board::CHAR_X:
            CHECK_NOTHROW(ticktack::Board::other_player(cc));
            CHECK(ticktack::Board::CHAR_O == ticktack::Board::other_player(cc));
            break;
        default:
            CHECK_THROWS_AS(ticktack::Board::other_player(cc), std::out_of_range);
        }
    }
}

TEST_CASE("Check decode") {
    CHECK(ticktack::decode(1, 1, '1', '0') == '1');
    CHECK(ticktack::decode(0, 1, '1', '0', 1, '1', ' ') == '0');
    CHECK(ticktack::decode('X', 1, '1', '0') == '1');
    CHECK(ticktack::decode(1, 1, '1', '0') == '1');
    CHECK(ticktack::decode(1, 1, '1', '0') == '1');
}