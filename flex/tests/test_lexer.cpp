#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <sstream>
#include <memory>
#include <cstring>
#include "doctest.h"
#include "expr_lexer.h"

const char *test1 = "(_var_1 + _var_2) * __long_variable_name__1234567889";
const char *test2 = "(0.56 + 1.54) - (10.34 + 333.789);";
const char *test_lc = "%% Line comment\n"
                    "45 %% Line Comment\n"
                    "+ %% Line Comment\n"
                    "10 %% Line Comment\n"
                    "%%";

const char *test_bc1 = "{ Block comment\n"
                    "{ Nested block comment }} 45 { Block comment } + { Block comment } 10\n";

const char *test_bc2 = "{ Block comment {{ \n"
                    " Nested block comment }} %%% }45{ Block comment {{ \n"
                    " Nested block comment }} %%% } + 10{ Block comment {%{%{ \n"
                    " Block comment %}%}%} }";

doctest::String toString(Token tk) {
    switch (tk) {
        case Token::Num: return "Number";     
        case Token::Id:  return "Identifier";
        case Token::OpAdd: return "OpAdd";
        case Token::OpSub: return "OpSub";
        case Token::OpMul: return "OpMul";
        case Token::OpenPar:  return "OpenPar";
        case Token::ClosePar: return "ClosePar";
        case Token::OpDiv:    return "OpDiv";
        case Token::Semicolon: return "Semicolon";
        case Token::Error:     return "Error";
        case Token::Eof:       return "EOF";
        default:
            return "Unknown";
    }
}

TEST_CASE("Expr with variables") {
    std::istringstream in;

    in.str(test1);
    ExprLexer l(in);
    Token tk = l.getNextToken();

    REQUIRE( tk == Token::OpenPar );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Id );
    tk = l.getNextToken();
    REQUIRE( tk == Token::OpAdd );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Id );
    tk = l.getNextToken();
    REQUIRE( tk == Token::ClosePar );
    tk = l.getNextToken();
    REQUIRE( tk == Token::OpMul );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Id );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Eof );
}

TEST_CASE("Big buffer") {
    std::istringstream in;
    const int sizeKb = 64;
    int lastPos = sizeKb * 1024 - 1;
    std::unique_ptr<char[]> buff(new char[sizeKb * 1024]);
    char *p = buff.get();

    memset(p, ' ', sizeKb * 1024);
    int pos = lastPos - strlen(test1);
    strcpy(&p[pos], test1);
    p[lastPos] = '\0';

    in.str(p);
    ExprLexer l(in);
    Token tk = l.getNextToken();

    REQUIRE( tk == Token::OpenPar );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Id );
    tk = l.getNextToken();
    REQUIRE( tk == Token::OpAdd );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Id );
    tk = l.getNextToken();
    REQUIRE( tk == Token::ClosePar );
    tk = l.getNextToken();
    REQUIRE( tk == Token::OpMul );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Id );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Eof );
}

TEST_CASE("Complex expression with real numbers") {
    std::istringstream in;

    in.str(test2);
    ExprLexer l(in);
    Token tk = l.getNextToken();

    REQUIRE( tk == Token::OpenPar );
    REQUIRE( l.getText() == "(" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Num );
    REQUIRE( l.getText() == "0.56" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::OpAdd );
    REQUIRE( l.getText() == "+" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Num );
    REQUIRE( l.getText() == "1.54" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::ClosePar );
    REQUIRE( l.getText() == ")" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::OpSub );
    REQUIRE( l.getText() == "-" );

    tk = l.getNextToken();
    REQUIRE( tk == Token::OpenPar );
    REQUIRE( l.getText() == "(" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Num );
    REQUIRE( l.getText() == "10.34" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::OpAdd );
    REQUIRE( l.getText() == "+" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Num );
    REQUIRE( l.getText() == "333.789" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::ClosePar );
    REQUIRE( l.getText() == ")" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Semicolon );
    REQUIRE( l.getText() == ";" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Eof );
}

TEST_CASE("Line comments") {
    std::istringstream in;

    in.str(test_lc);
    ExprLexer l(in);
    Token tk = l.getNextToken();

    REQUIRE( tk == Token::Num );
    REQUIRE( l.getText() == "45" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::OpAdd );
    REQUIRE( l.getText() == "+" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Num );
    REQUIRE( l.getText() == "10" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Eof );
}

TEST_CASE("Block comments 1") {
    std::istringstream in;

    in.str(test_bc1);
    ExprLexer l(in);
    Token tk = l.getNextToken();

    REQUIRE( tk == Token::Num );
    REQUIRE( l.getText() == "45" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::OpAdd );
    REQUIRE( l.getText() == "+" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Num );
    REQUIRE( l.getText() == "10" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Eof );
}

TEST_CASE("Block comments 2") {
    std::istringstream in;

    in.str(test_bc2);
    ExprLexer l(in);
    Token tk = l.getNextToken();

    REQUIRE( tk == Token::Num );
    REQUIRE( l.getText() == "45" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::OpAdd );
    REQUIRE( l.getText() == "+" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Num );
    REQUIRE( l.getText() == "10" );
    tk = l.getNextToken();
    REQUIRE( tk == Token::Eof );
}
