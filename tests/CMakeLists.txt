add_executable(tt_tests tt_tests.cpp)

find_package(doctest CONFIG REQUIRED)
target_link_libraries(tt_tests PRIVATE doctest::doctest tt)

add_test(NAME tt_tests COMMAND tt_tests)
