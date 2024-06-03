#pragma once

#include <algorithm>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <array>

namespace ticktack {

     static constexpr auto decode(auto const & expr, auto A, auto B, auto ... args) {
        if (expr == A)
            return B;
        return decode(expr, args...);
    }

     static constexpr auto decode(auto const & expr, auto A, auto B, auto C) {
        if (expr == A)
            return B;
        return C;
    }

    class Board {
    public:
        using data_type = std::array<char, 3 * 3>;
        static constexpr char CHAR_VOID = ' ';
        static constexpr char CHAR_X = 'X';
        static constexpr char CHAR_O = 'O';

        struct Position {
            uint8_t x_, y_;

            Position(Position const &) = default;
            Position& operator=(Position const &) = default;

            [[nodiscard]] auto left() const noexcept -> Position {
                return {static_cast<uint8_t>(std::max((uint8_t)1, x_) - 1), y_};
            }
            [[nodiscard]] auto right() const noexcept -> Position {
                return {static_cast<uint8_t>(std::min((uint8_t)1, x_) + 1), y_};
            }
            [[nodiscard]] auto up() const noexcept -> Position {
                return {x_, static_cast<uint8_t>(std::max((uint8_t)1, y_) - 1)};
            }
            [[nodiscard]] auto down() const noexcept -> Position {
                return {x_, static_cast<uint8_t>(std::min((uint8_t)1, y_) + 1)};
            }
            bool operator==(Position const &) const noexcept = default;

            static Position const top_left;
            static Position const top_middle;
            static Position const top_right;
            static Position const middle_left;
            static Position const middle_middle;
            static Position const middle_right;
            static Position const bottom_left;
            static Position const bottom_middle;
            static Position const bottom_right;
            static std::array<Position, 9> const all_positions;

        private:
            Position(uint8_t x, uint8_t y) : x_{x}, y_{y} {
                if (x > 2 || y > 2)
                    [[unlikely]] throw std::out_of_range("Coordinates must be within [0; 2]");
            };

        };

        auto at(Position const & p) noexcept -> char& {
            return at(p.x_, p.y_);
        }
        auto at(Position const & p) const noexcept -> char const & {
            return data_[p.x_ + 3 * p.y_];
        }
        auto at(unsigned x, unsigned y) -> char& {
            if (x > 2 || y > 2)
                [[unlikely]] throw std::out_of_range("Coordinates must be within [0; 2]");
            return data_[y * 3 +x];
        }

        static constexpr char valid_chars[] = {CHAR_VOID, CHAR_O, CHAR_X};
        void set(Position const & position, char c) {
            validate_char(c);
            at(position) = c;
        }

        void clear() {
            std::ranges::fill(data_, CHAR_VOID);
        }

        auto set_state(std::array<char, 3 * 3> const & data) {
            std::ranges::copy(data, data_.begin());
        }

        static Position const triplets[8][3];
        auto check_winner() const -> std::optional<typename data_type::value_type> {
            for(auto const & t : triplets) {
                unsigned x_cnt = 0, o_cnt = 0;
                for(auto const & p : t) {
                    switch(at(p)) {
                    case CHAR_O:
                        o_cnt++;
                        break;
                    case CHAR_X:
                        x_cnt++;
                        break;
                        // none
                    }
                }
                if (x_cnt == 3)
                    return std::make_optional(CHAR_X);
                if (o_cnt == 3)
                    return std::make_optional(CHAR_O);
            }
            return {};
        }
        static constexpr void validate_char(char c) {
            auto found = std::ranges::find(valid_chars, c);
            if (found == std::end(valid_chars)) {
                [[unlikely]] throw std::out_of_range("Characters must be ' ', 'O', or 'X'.");
            }
        }
        static constexpr char other_player(char player) {
            validate_char(player);
            switch (player) {
            case CHAR_O:
                return CHAR_X;
                break;
            case CHAR_X:
                return CHAR_O;
                break;
            }
            return player;
        }
    protected:
    private:
        data_type data_; // space ' ' | 'X' | 'O'
    };

    class BoardTUIControllerView {
    public:
        void print(Board const & board) {

        }
    protected:
    private:
    };

    class ComputerPlayer
    {
    public:
        ComputerPlayer();
        ~ComputerPlayer();
        int evaluate_triplet(Board const & board, Board::Position const (&triplet)[3], char player);
        int evaluate(Board const & board, char player);
        Board::Position next_move(Board const & board);
    };

} // namespace ticktack
