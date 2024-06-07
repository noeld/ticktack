# ticktack

A simple tick-tack-toe implementation. I was interested in the things I would
learn along the way, especially testing and test driven development
(TDD) with *doctest*.

I found that in writing test cases first I was forced to think about *usage* of
each function/class/API upfront and development was much more relaxed, since
the test would reliably show any regressions. The simplicity of [doctest](https://github.com/doctest/doctest) and good integration with `ctest` is great!


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

## Play

Human is `X`, computer is `O`.

Start with `./tictack -c` to let the computer move first; otherwise you will move first.

    ./ticktack -h
    Usage: ./ticktack [options]

        Options:
        -h    show help
        -c    computer moves first (default: human)
        -<n>  n = 1..9 : set strength of computer (1 = weak, 9 = strong)
              default=4

    ./ticktack -9 -c
       |   |
       |   |
       |   |

       |   |
       |   |
       |   | O

     0 | 1 | 2
     3 | 4 | 5
     6 | 7 |
    Press number of your next move for X:
    0
     X |   |
       |   |
       |   | O

     X |   |
       |   |
     O |   | O

       | 1 | 2
     3 | 4 | 5
       | 7 |
    Press number of your next move for X:
    7
     X |   |
       |   |
     O | X | O

     X |   | O
       |   |
     O | X | O

       | 1 |
     3 | 4 | 5
       |   |
    Press number of your next move for X:
    5
     X |   | O
       |   | X
     O | X | O

     X |   | O
       | O | X
     O | X | O

    *** Winner is: 😛 O 😛 ***


## Notes

    ~/.vcpkg-clion/vcpkg/vcpkg x-update-baseline --add-initial-baseline

https://de.wikipedia.org/wiki/Minimax-Algorithmus
