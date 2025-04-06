cmake_minimum_required(VERSION 3.16)
project(ChessFinalYearProjectAgain)

set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "C:/Users/tomha/Downloads/SFML-2.6.2-windows-gcc-13.1.0-mingw-64-bit/SFML-2.6.2/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(${PROJECT_NAME} main.cpp
        Pawn.h
        Piece.cpp
        Queen.cpp
        Rook.cpp
        King.cpp
        Knight.cpp
        Bishop.cpp
        chessBoard.cpp
        Pawn.cpp
        precomputeMoves.cpp
        precomputeMoves.h
        precomputeMoves.h
        MoveScore.h
        Move.h
        PrecomputeMovesGlobal.cpp
        PrecomputeMovesGlobal.h
        OpeningBook.cpp
        OpeningBook.h
)

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
