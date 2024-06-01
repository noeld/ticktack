#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("first test") {
    CHECK_NOTHROW(1 == 1);
}
