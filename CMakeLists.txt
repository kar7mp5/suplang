cmake_minimum_required(VERSION 3.10)
project(SupLang)
# 소스 파일 목록에 Parser.cpp 추가
set(SOURCES
    src/main.cpp
    src/Lexer/Lexer.cpp
    src/Parser/Parser.cpp
)

# 실행 파일 생성
add_executable(suplang ${SOURCES})
# 헤더 파일 경로 설정
target_include_directories(suplang PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

