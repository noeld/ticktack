# ticktack

A simple tick-tack-toe implementation. I wasn't interested in the game, but in
the things I could learn along the way, especially testing and TDD
with *doctest*.

## Build

This project uses `cmake` with `vcpkg` to build and install dependencies:

 - doctest

Run these commands to build the project.

    mkdir build
    cmake -S . -B build/ -G Ninja -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_TOOLCHAIN_FILE=~/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    cmake --build build

## Test

    cd build
    ctest

## Notes

    ~/.vcpkg-clion/vcpkg/vcpkg x-update-baseline --add-initial-baseline