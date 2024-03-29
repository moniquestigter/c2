#ifndef _EXPR_LEXER_H
#define _EXPR_LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <unordered_map>
#include "expr_tokens.h"
#include "stack.hh"
enum class Token{
    OpSub,
    OpAdd,
    OpMul,
    OpDiv,
    Num,
    Id,
    Eof,
    OpenPar,
    ClosePar,
    Semicolon
};

using yyscan_t = void*;
class ExprLexer {
public:
    using semantic_type = Expr::Parser::semantic_type;
    using token = Expr::Parser::token;

    ExprLexer(std::istringstream &in);
    ~ExprLexer();
    int makeToken(const char * txt, int len, int tk){
        std::string tt(txt,len);
        text = std::move(tt);
        return tk;
    }  
    int getLineNo();

    Token getNextToken(semantic_type *yylval) { return _getNextToken(*yylval,scanner); }
    std::string getText() { return text; }

private:
    Token _getNextToken(semantic_type &yylval,yyscan_t yyscanner);
  
    std::istream &in;
    std::string text;
    yyscan_t scanner;
};  
#endif
