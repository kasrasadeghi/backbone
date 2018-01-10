cmake_minimum_required(VERSION 3.5)
project(backbone C)

set(CMAKE_C_STANDARD 11)

#find_package(GTest REQUIRED)
find_program(CLANG-CHECK clang-check)
message("found: " ${CLANG-CHECK})

find_program(VALGRIND valgrind)
message("found: " ${VALGRIND})

find_program(MAKE make)
message("found: " ${MAKE})

add_executable(backbone main.c
    parser.c parser.h
    str.c str.h
    gen.c gen.h
    normalize.c normalize.h
    sexp.c sexp.h
    reader.c reader.h
    qualify.c qualify.h
    blockify.c blockify.h
)
