#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <vector>
#include <sstream>
#include "doctest.h"
#include "expr_tokens.h"
#include "expr_lexer.h"
#include <unordered_map>
/* 1 */ std::string test_add = "45 + 10";
/* 1 */ std::string test_sub = "45 - 10";
/* 1 */ std::string test_mul = "45 * 9";
/* 1 */ std::string test_div = "45 / 7";
/* 1 */ std::string test_mod = "45 % 7";

/* 2 */ std::string test_vars = "(variable1 + var_2) * multiplier";

/* 3 */ std::string test_multiple = "(53 + 35 ) * 100; {Expression 1}\n"
                            "(43 + 65) / 4; {Expression 2}\n"
                            "(variable1 - var_2) % divisor # Last expression";

TEST_CASE("Add operation") {
    std::unordered_map<std::string, int> vars;
    std::vector<int> values;

    std::istringstream iss(test_add);
    ExprLexer lexer(iss);
    Expr::Parser parser(lexer, vars, values);
    
    parser();
    CHECK(values.size() == 1);
    CHECK(values[0] == 55);
}

TEST_CASE("Sub operation") {
    std::unordered_map<std::string, int> vars;
    std::vector<int> values;

    std::istringstream iss(test_sub);
    ExprLexer lexer(iss);
    Expr::Parser parser(lexer, vars, values);
    
    parser();
    CHECK(values.size() == 1);
    CHECK(values[0] == 35);
}

TEST_CASE("Mult operation") {
    std::unordered_map<std::string, int> vars;
    std::vector<int> values;

    std::istringstream iss(test_mul);
    ExprLexer lexer(iss);
    Expr::Parser parser(lexer, vars, values);
    
    parser();
    CHECK(values.size() == 1);
    CHECK(values[0] == 405);
}

TEST_CASE("Div operation") {
    std::unordered_map<std::string, int> vars;
    std::vector<int> values;

    std::istringstream iss(test_div);
    ExprLexer lexer(iss);
    Expr::Parser parser(lexer, vars, values);
    
    parser();
    CHECK(values.size() == 1);
    CHECK(values[0] == 6);
}

TEST_CASE("Mod operation") {
    std::unordered_map<std::string, int> vars;
    std::vector<int> values;

    std::istringstream iss(test_mod);
    ExprLexer lexer(iss);
    Expr::Parser parser(lexer, vars, values);
    
    parser();
    CHECK(values.size() == 1);
    CHECK(values[0] == 3);
}

TEST_CASE("Expression with variables") {
    std::unordered_map<std::string, int> vars;
    std::vector<int> values;
    std::istringstream iss(test_vars);
    ExprLexer lexer(iss);
    Expr::Parser parser(lexer, vars, values);
    
    vars["variable1"] = 37;
    vars["var_2"] = 23;
    vars["multiplier"] = 91;
    parser();
    CHECK(values.size() == 1);
    CHECK(values[0] == 5460);
}

TEST_CASE("Multiple expressions") {
    std::unordered_map<std::string, int> vars;
    std::vector<int> values;
    std::istringstream iss(test_multiple);
    ExprLexer lexer(iss);
    Expr::Parser parser(lexer, vars, values);
    
    vars["variable1"] = 97;
    vars["var_2"] = 22;
    vars["divisor"] = 37;
    parser();
    CHECK(values.size() == 3);
    CHECK(values[0] == 8800);
    CHECK(values[1] == 27);
    CHECK(values[2] == 1);
}

