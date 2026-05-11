cmake_minimum_required(VERSION 3.10)

project(therizinosauridae LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(therizinosauridae
               tests/test_main.c
               tests/test_lexer.c
               tests/test_parser.c
               src/lexer.c
               src/parser.c
               src/ast.c

               tests/test.h
               include/lexer.h
              )

target_include_directories(therizinosauridae PRIVATE ./include)


