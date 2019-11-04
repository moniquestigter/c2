#ifndef _EXPR_LEXER_H
#define _EXPR_LEXER_H
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

enum class Token {
    Num         = 100,
    Id          = 101,
    OpAdd       = 102,
    OpSub       = 103,
    OpMul       = 104,
    OpenPar     = 105,
    ClosePar    = 106,
    OpDiv       = 107,
    Semicolon   = 108,
    Error       = 998,
    Eof         = 999,
};

using yyscan_t = void*;

class ExprLexer {
public:
    ExprLexer(std::istream &in);
    ~ExprLexer();

    Token getNextToken() { return _getNextToken(scanner); }
    std::string getText() { return text; }

    /* Flex will generate this function */
    Token _getNextToken(yyscan_t yyscanner);

    Token makeToken(const char *txt, int len, Token tk) {
        std::string tt(txt, len);
        text = std::move(tt);
        return tk;
    }

    Token eofToken(){
        return Token::Eof;
    }
    
    Token errorToken(){
        return Token::Error;
    }

private:
    std::istream &in;
    std::string text;
    yyscan_t scanner;
};
#endif
