cmake_minimum_required(VERSION 3.20)

project(ticktack DESCRIPTION "A simple tick-tack-toe" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Enable vcpkg
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()

include_directories(include)


add_library(tt SHARED src/tt.cpp include/ticktack/tt.h)
target_compile_options(tt INTERFACE -Wall -Wconversion -Werror)

file(GLOB SRC "src/main.cpp" "include/tt.h")
add_executable(${PROJECT_NAME} ${SRC})
# target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wconversion -Werror)
target_link_libraries(${PROJECT_NAME} tt)

enable_testing()

add_subdirectory(tests)


