#pragma once
#include <memory>
#include <sstream>
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <array>
#include <vector>

namespace ticktack {

    /**
     * @brief      If expr == A then return B else return C
     * The function always expects an odd number of arguments.
     *
     * @param      expr  The expression
     * @param[in]  A     { parameter_description }
     * @param[in]  B     { parameter_description }
     * @param[in]  C     { parameter_description }
     *
     * @return     { description_of_the_return_value }
     */
     constexpr auto decode(auto const & expr, auto A, auto B, auto C) {
        if (expr == A)
            return B;
        return C;
    }

     constexpr auto decode(auto const & expr, auto A, auto B, auto ... args) {
        if (expr == A)
            return B;
        return decode(expr, args...);
    }

    class Board {
    public:
        using data_type = std::array<char, 3 * 3>;
        using value_type = data_type::value_type;

        static constexpr char CHAR_VOID = ' ';
        static constexpr char CHAR_X = 'X';
        static constexpr char CHAR_O = 'O';

        static constexpr char players[] = {CHAR_O, CHAR_X};

        struct Position {
            uint8_t x_{0}, y_{0};

            Position() = default;
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

            operator std::string() const {
                std::ostringstream oss;
                oss << "(" << x_ << ", " << y_ << ")";
                return oss.str();
            }

            friend std::ostream& operator<<(std::ostream& out, Position const & pos) {
                return out << "(" << pos.x_ << ", " << pos.y_ << ")";
            }

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

            static Position const none;
            constexpr bool is_none() const noexcept { return *this == none; }

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

        constexpr auto possible_moves() const {
            return Board::Position::all_positions
                | std::views::filter([&](Position const &p) {
                    return at(p) == CHAR_VOID;
                });
        }

        constexpr void possible_moves(auto back_inserter) const {
            std::ranges::copy(possible_moves(), back_inserter);
        }

        constexpr auto end() const noexcept { return data_.end(); }

        /**
         * @brief      Turn a string into a board state.
         * Use X and O and _ for empty fields. All other characters are ignored
         *
         * @param[in]  sv    { parameter_description }
         *
         * @return     The data type.
         */
        static constexpr data_type board_state_from_string(std::string_view sv) {
            data_type tmp;
            size_t i = 0;
            for(auto c : sv) {
                c = decode(c, '_', CHAR_VOID, CHAR_VOID, '*', c);
                if (std::ranges::find(valid_chars, c) == std::end(valid_chars))
                    continue;
                tmp[i++] = c;
                if (i > tmp.size())
                    break;
            }
            return tmp;
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
            return decode(player, CHAR_O, CHAR_X, CHAR_X, CHAR_O, player);
        }
    protected:
    private:
        data_type data_; // space ' ' | 'X' | 'O'
    };

    class BoardTUIControllerView {
    public:
        std::string to_string(Board const & board) {
            std::string str;
            str.reserve(9 + 9);
            for(size_t i = 0; i < Board::Position::all_positions.size(); ++i) {
                auto const & pos = Board::Position::all_positions[i];
                str += board.at(pos);
                str += (i % 3 == 2) ? '\n' : '|';
            }
            return str;
        }
        void print(Board const & board);
        auto next_human_move(Board const & board) -> std::optional<Board::Position>;
        void announce_result(std::optional<Board::value_type> const & winner);
    protected:
    private:
    };

    class ComputerPlayer
    {
    public:
        static constexpr int score_win = 10;
        static constexpr int score_loss = -score_win;
        ComputerPlayer() = default;
        ~ComputerPlayer() = default;
        int evaluate_triplet(Board const & board, Board::Position const (&triplet)[3], char player);
        int evaluate(Board const & board, char player);
        auto next_move(Board board, char player) -> std::optional<Board::Position>;
        int depth() const { return depth_; }
        /**
         * @brief      Sets the depth
         *
         * @param[in]  new_depth  The new depth between 1 (weaker) and 9 (stronger)
         */
        void set_depth(int new_depth) { depth_ = std::clamp(new_depth, 1, 9); }
    protected:
        int next_move(Board& board, char player,  int return_depth, int depth, std::optional<Board::Position>& best_move);
        int depth_ { 4 };
    };

    class Game;


    class Game {
    public:
        Board& board() { return *board_; }
        BoardTUIControllerView& controller_view() { return *controller_view_; }
        ComputerPlayer& computer_player() { return *computer_player_; }

        static Game create_game();
    private:
        std::unique_ptr<Board> board_;
        std::unique_ptr<BoardTUIControllerView> controller_view_;
        std::unique_ptr<ComputerPlayer> computer_player_;
    };


} // namespace ticktack

namespace std {
    template<> struct hash<ticktack::Board::Position> {
        size_t operator()(ticktack::Board::Position const & p) const {
            return p.x_ + 3 * p.y_;
        }
    };
}