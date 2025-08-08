cmake_minimum_required(VERSION 3.10)
project(SupLang)

set(SOURCES
    src/main.cpp
    src/Lexer/Lexer.cpp
    src/Parser/Parser.cpp
    src/Interpreter/Environment.cpp
    src/Interpreter/Interpreter.cpp
)

add_executable(suplang ${SOURCES})
target_include_directories(suplang PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

