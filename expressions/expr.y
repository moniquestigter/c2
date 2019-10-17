%option noyywrap


%{ 
#include <stdio.h>
using namespace std;
int yylex();
  
void yyerror(const char *msg) 
{
    std::cerr << "Parsing Error: "<<msg<<'\n';
} 
%}

%token INT 
%token ID 
%token SEMICOLON 
%token OPENPARENTESIS 
%token CLOSEPARENTESIS 
%token ADD 
%token  SUBTRACT 
%token DIVIDE
%token MULTIPLY
%token Eof

%%

input : statement_list
;

statement_list : statement_list statement
                |statement
;

statement : expr SEMICOLON
;

expr : expr OP_ADD term { $$ = $1 + $3; }
        |expr OP_SUB term { $$ = $1 - $3; }
        |%empty
;

term : term OP_MUL factor { $$ = $1 * $3; }
        |term OP_DIV factor { $$ = $1 / $3; }
        |%empty
;

factor : TK_OPEN_PAR expr TK_CLOSE_PAR { $$ = $2; }
            |TK_NUMBER { $$ = $1; }
;