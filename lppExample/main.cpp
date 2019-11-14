#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include "expr_lexer.h"
#include "expr_parser.h"
int yyparse();

int main(int argc, char **argv){
    std::filebuf fb;
    if (fb.open (argv[1],std::ios::in)) {
        std::istream is(&fb);
        YYNODESTATE state;
        ASTNode * stmtList;
        ExprLexer lex(is);
        Expr::Parser pars(lex,state,stmtList);
        pars.parse();
    }
    return 0;
}