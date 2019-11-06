#ifndef _EXPR_PARSER_H
#define _EXPR_PARSER_H
#include "expr_lexer.h"
class ExprParser {
public:
    ExprParser(ExprLexer& lex,std::unordered_map<std::string,int> vars, std::vector<int> values): lexer(lex), vars(vars),values(values){}
    ~ExprParser(){};

    void parser();
    void error(const char *msg);
    
private:
    std::unordered_map<std::string, int> vars;
    std::vector<int> values;
    ExprLexer& lexer;

};
#endif