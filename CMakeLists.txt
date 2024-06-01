cmake_minimum_required(VERSION 3.20)

project(ticktack DESCRIPTION "A simple tick-tack-toe" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Enable vcpkg
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()

include_directories(include)

file(GLOB SRC "src/*" "include/*")
add_executable(${PROJECT_NAME} ${SRC})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wconversion -Werror)

enable_testing()

add_subdirectory(tests)


