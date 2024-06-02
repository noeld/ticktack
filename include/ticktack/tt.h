#pragma once

#include <algorithm>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <array>

namespace ticktack {

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
    protected:
    private:
        data_type data_; // space ' ' | 'X' | 'O'
    };

    class BoardDisplay {
    public:
        void print(Board const & board) {

        }
    protected:
    private:
    };

} // namespace ticktack
