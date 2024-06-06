add_executable(tt_tests tt_tests.cpp)

find_package(doctest CONFIG REQUIRED)
target_link_libraries(tt_tests PRIVATE doctest::doctest tt)

add_test(NAME tt_tests COMMAND tt_tests)

add_executable(fixed_stack_tests fixed_stack_tests.cpp ${PROJECT_SOURCE_DIR}/include/ticktack/fixed_stack.h)
target_link_libraries(fixed_stack_tests PRIVATE doctest::doctest)

add_test(NAME fixed_stack_tests COMMAND fixed_stack_tests)
