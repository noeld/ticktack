#pragma once

#include <array>
#include <stdexcept>
namespace ticktock {

    /**
     * @brief      A stack based stack-structure with fixed capacity
     *
     * @tparam     T     { description }
     * @tparam     SIZE  { description }
     */
    template<typename T, std::size_t CAPACITY>
    class fixed_stack {
    public:
        using value_type = T;
        using reference_type = value_type &;
        using const_reference_type = value_type const &;
        using iterator = value_type*;
        using const_iterator = value_type const *;

        constexpr std::size_t capacity() const noexcept { return CAPACITY; }

        std::size_t const & size() const noexcept { return size_; }
        bool empty() const noexcept { return size() == 0; }
        bool full() const noexcept { return size() == capacity(); }

        void push(T const & value) {
            if (full())
                throw std::overflow_error("Stack overflow");
            data_[size_++] = value;
        }

        void push_back(T const & value) { push(std::forward<T const>(value)); }

        void pop() {
            if (empty())
                throw std::underflow_error("Stack underflow");
            --size_;
        }

        reference_type top() {
            if (empty())
                throw std::underflow_error("Stack is empty");
            return data_[size_ - 1];
        }

        reference_type back() { return top(); }

        const_reference_type top() const {
            if (empty())
                throw std::underflow_error("Stack is empty");
            return data_[size_ - 1];
        }

        const_reference_type back() const { return back(); }

    protected:
    private:
        std::array<T, CAPACITY> data_ {};
        std::size_t size_{0};
    };

}