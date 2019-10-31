%code requires {
	#include "ast.h"
}
%{
	#include <stdio.h>
	#include <iostream>
	int yylex();

	void yyerror(const char *msg)
	{
		printf("%s\n",msg);
	}

	#define YYERROR_VERBOSE 1
%}

%union{
	Expr *astnode_t;
	int number_t;
	char *str_t;
}

%token OP_ADD OP_SUB OP_MUL OP_DIV TK_RIGHT_PAR TK_LEFT_PAR TK_SEMICOLON 
%token <str_t> TK_IDENT
%token <number_t> TK_NUMBER
%token  TK_EOL
%token TK_ERROR

%type <astnode_t> expr term factor

%%

input: expr_list 
;

expr_list:	expr_list expr TK_SEMICOLON {std::cout<<$2->eval()<<std::endl;}
	| expr TK_SEMICOLON {std::cout<<$1->eval()<<std::endl;}
;
	
expr: expr OP_ADD term	{ $$ = new AddExpr($1,$3); }
	| expr OP_SUB term	{ $$ = new SubExpr($1,$3); }
	| term				{ $$ = $1; }
;

term: term OP_MUL factor	{ $$ = new MulExpr($1,$3); }
	| term OP_DIV factor	{ $$ = new DivExpr($1,$3); }
	| factor				{ $$ = $1; }
;

factor:	TK_NUMBER						{ $$ = new NumberExpr($1); }
	  | TK_IDENT						{ $$ = new VarExpr($1);free($1);  }
	  | TK_LEFT_PAR expr TK_RIGHT_PAR	{ $$ = $2; }
;