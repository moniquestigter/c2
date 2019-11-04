%code requires{
    #define YYSTYPE double
    #define YYSTYPE_IS_DECLARED 1
    
}

%{
    #include <unordered_map>
    #include "expr_lexer.h"
    #include <sstream>



    extern ExprLexer yylex;
    std::vector<double> map;

    #define YYERROR_VERBOSE 1

    void yyerror(const char* msg) {
        std::cerr << msg;
    }
    void exprInit(){};

    int getExprCount() {
        return map.size();
    }
    
    double getExprValue(int index) {
        return map[index];
    }
%}

%token NUMBER
%%
input: stmt_list;
stmt_list: stmt_list expr opt_semicolon {map.push_back($2);}
|%empty {map.clear();};

opt_semicolon: ';' | %empty;

expr: expr '+' term {$$=$1+$3;}
    | expr '-' term {$$=$1-$3;}
    | term {$$=$1;};


term: term '*' factor {$$=$1*$3;}
    | term '/' factor {$$=$1/$3;}
    | factor {$$=$1;};

factor: NUMBER {$$=$1;}
    | '(' expr  ')' {$$=$2;}
    ;




%%