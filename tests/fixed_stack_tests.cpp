#include <stdexcept>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include "ticktack/fixed_stack.h"

TEST_CASE("Check fixed_stack<int, 3>") {
    ticktock::fixed_stack<int, 3> fs;
    REQUIRE(fs.capacity() == 3);
    REQUIRE(fs.size() == 0);
    CHECK(fs.empty() == true);
    CHECK(fs.full() == false);
    CHECK_THROWS_AS(fs.top(), std::underflow_error);
    CHECK_THROWS_AS(fs.pop(), std::underflow_error);

    CHECK_NOTHROW(fs.push(1));
    CHECK(fs.capacity() == 3);
    CHECK(fs.size() == 1);
    CHECK_NOTHROW(fs.top());
    CHECK(fs.top() == 1);
    CHECK(fs.empty() == false);
    CHECK(fs.full() == false);

    CHECK_NOTHROW(fs.push(2));
    CHECK(fs.capacity() == 3);
    CHECK(fs.size() == 2);
    CHECK_NOTHROW(fs.top());
    CHECK(fs.top() == 2);
    CHECK(fs.empty() == false);
    CHECK(fs.full() == false);

    CHECK_NOTHROW(fs.push(3));
    CHECK(fs.capacity() == 3);
    CHECK(fs.size() == 3);
    CHECK_NOTHROW(fs.top());
    CHECK(fs.top() == 3);
    CHECK(fs.empty() == false);
    CHECK(fs.full() == true);

    CHECK_THROWS_AS(fs.push(4), std::overflow_error);
    CHECK(fs.capacity() == 3);
    CHECK(fs.size() == 3);
    CHECK_NOTHROW(fs.top());
    CHECK(fs.top() == 3);
    CHECK(fs.empty() == false);
    CHECK(fs.full() == true);

    CHECK_NOTHROW(fs.pop());
    CHECK(fs.capacity() == 3);
    CHECK(fs.size() == 2);
    CHECK_NOTHROW(fs.top());
    CHECK(fs.top() == 2);
    CHECK(fs.empty() == false);
    CHECK(fs.full() == false);

    CHECK_NOTHROW(fs.pop());
    CHECK(fs.capacity() == 3);
    CHECK(fs.size() == 1);
    CHECK_NOTHROW(fs.top());
    CHECK(fs.top() == 1);
    CHECK(fs.empty() == false);
    CHECK(fs.full() == false);

    CHECK_NOTHROW(fs.pop());
    REQUIRE(fs.capacity() == 3);
    REQUIRE(fs.size() == 0);
    CHECK_THROWS_AS(fs.top(), std::underflow_error);
    CHECK(fs.empty() == true);
    CHECK(fs.full() == false);

    CHECK_THROWS_AS(fs.pop(), std::underflow_error);
    REQUIRE(fs.capacity() == 3);
    REQUIRE(fs.size() == 0);
    CHECK_THROWS_AS(fs.top(), std::underflow_error);
    CHECK(fs.empty() == true);
    CHECK(fs.full() == false);
}

TEST_CASE("Check fixed_stack<std::string, 2>") {
    ticktock::fixed_stack<std::string, 2> fs;
    REQUIRE(fs.capacity() == 2);
    REQUIRE(fs.size() == 0);
    CHECK(fs.empty() == true);
    CHECK(fs.full() == false);
    CHECK_THROWS_AS(fs.top(), std::underflow_error);
    CHECK_THROWS_AS(fs.pop(), std::underflow_error);

    CHECK_NOTHROW(fs.push("1"));
    CHECK(fs.capacity() == 2);
    CHECK(fs.size() == 1);
    CHECK_NOTHROW(fs.top());
    CHECK(fs.top() == "1");
    CHECK(fs.empty() == false);
    CHECK(fs.full() == false);

    CHECK_NOTHROW(fs.push("2"));
    CHECK(fs.capacity() == 2);
    CHECK(fs.size() == 2);
    CHECK_NOTHROW(fs.top());
    CHECK(fs.top() == "2");
    CHECK(fs.empty() == false);
    CHECK(fs.full() == true);

    CHECK_THROWS_AS(fs.push("3"), std::overflow_error);
    CHECK(fs.capacity() == 2);
    CHECK(fs.size() == 2);
    CHECK_NOTHROW(fs.top());
    CHECK(fs.top() == "2");
    CHECK(fs.empty() == false);
    CHECK(fs.full() == true);

    CHECK_NOTHROW(fs.pop());
    CHECK(fs.capacity() == 2);
    CHECK(fs.size() == 1);
    CHECK_NOTHROW(fs.top());
    CHECK(fs.top() =="1");
    CHECK(fs.empty() == false);
    CHECK(fs.full() == false);

    CHECK_NOTHROW(fs.pop());
    CHECK(fs.capacity() == 2);
    CHECK(fs.size() == 0);
    CHECK_THROWS_AS(fs.top(), std::underflow_error);
    CHECK(fs.empty() == true);
    CHECK(fs.full() == false);

    CHECK_THROWS_AS(fs.pop(), std::underflow_error);
    CHECK(fs.capacity() == 2);
    CHECK(fs.size() == 0);
    CHECK_THROWS_AS(fs.top(), std::underflow_error);
    CHECK(fs.empty() == true);
    CHECK(fs.full() == false);
}