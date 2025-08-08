#pragma once
#include "Token.h"

class Lexer {
public:
    Lexer(const std::string& source);
    Token nextToken(); // 다음 토큰을 읽어 반환하는 함수

private:
    std::string source;
    size_t position = 0; // 현재 읽고 있는 위치
    char currentChar;    // 현재 문자

    void advance();      // 다음 문자로 이동
    void skipWhitespace(); // 공백 문자 건너뛰기
    Token makeIdentifier(); // 식별자 또는 키워드 토큰 생성
    Token makeNumber();     // 숫자 토큰 생성
};