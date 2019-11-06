#ifndef _EXPR_PARSER_H
#define _EXPR_PARSER_H
#include "expr_lexer.h"
class Parser {
public:
    Parser(ExprLexer& lex,std::unordered_map<std::string,int> vars, std::vector<int> values): lexer(lex), vars(vars),values(values){}
    ~Parser(){};

    void parser();
    void error(const char *msg);
    
private:
    std::unordered_map<std::string, int> vars;
    std::vector<int> values;
    ExprLexer& lexer;

};
#endif