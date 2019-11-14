#include "expr_lexer.h"

class Parser {
public:
    Parser(ExprLexer& lex, YYNODESTATE sta): lexer(lex), state(sta){}

    void parser();
    void error(const char *msg);

    
private:
    ExprLexer& lexer;
    YYNODESTATE state;
    
};