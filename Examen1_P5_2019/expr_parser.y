%require "3.0"
%language "c++"
%parse-param {ExprLexer& lexer}
%parse-param {std::unordered_map<std::string, int> &vars}
%parse-param {std::vector<int>& values}
%define parse.error verbose 
%define api.value.type variant
%define parser_class_name {Parser}
%define api.namespace {Expr}

%code requires{
    #include <vector>
    #include <unordered_map>
    class ExprLexer;
}

%{
    #include <stdio.h>
    #include <iostream>
    #include <vector>
    #include <unordered_map>
    #include <stdio.h>
    #include <string.h>
    #include <iostream>
    #define yylex(arg) lexer.getNextToken(arg)
    #include "expr_tokens.h"
    #include "expr_parser.h"
    namespace Expr{
        void Parser::error(const std::string& msg){
            std::cerr << msg << "\n";
        }

   

    }

    extern int yylineno;

%}

%token Num OpSub OpAdd OpenPar ClosePar Semicolon EOf Id OpDiv OpMult

%%

input: expr_list 
;

expr_list:	expr_list expr opt_semicolon {values.push_back($2);}
	| %empty {values.clear();};
;
opt_semicolon:  ';'{}
             | %empty;
             ;
expr: expr OpAdd term {$$ = $1 + $3;}
    | expr OpSub term {$$ = $1 - $3;}
    | term {$$ = $1;}
    ;

term: term '*' factor {$$ = $1 * $3;}
    | term '/' factor {$$ = $1 / $3;}
    | factor {$$ = $1;}
    ;

factor: Num {}
    | OpenPar expr ClosePar { $$ = $2;};
    ;
%%