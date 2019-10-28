%code requires {
	#include "ast.h"
}

%{
	#include <stdio.h>

	int yylex();
	extern int yylineno;

	void yyerror(const char *msg)
	{
		printf("Line %d %s\n",yylineno,msg);
	}

	#define YYERROR_VERBOSE 1
%}

%union {
	Statement *statement_t;
	Expr *expr_t;
	int int_t;
}

%type <statement_t> input_src stmt asign print stmts
%type <expr_t> expr term factor
%type <int_t> print_format

%token OP_ADD OP_SUB OP_MUL OP_DIV OP_EQUAL TK_RIGHT_PAR TK_LEFT_PAR 
%token KW_PRINT 
%token<int_t> KW_VARINDEX
%token<int_t> TK_NUMBER
%token PT_COMMA
%token<int_t> KW_HEX KW_DEC KW_BIN
%token<int_t> TK_EOL
%token TK_ERROR

%%
input_src: stmts		{ $$ = $1; $$->exec(); }
;


stmts: stmts eofs stmt eofs 	{ $$ = $1; ((BlockStatement*)$$)->addStatement($3); }
	|	stmt 			{ $$ = new BlockStatement; ((BlockStatement*)$$)->addStatement($1); }
;

eofs: eofs TK_EOL
	| TK_EOL
	|
;

stmt: asign					{ $$ = $1; }
	| print					{ $$ = $1; }
;

asign: KW_VARINDEX OP_EQUAL expr	{ $$ = new AsignStatement($1,$3); }
;

print:    KW_PRINT expr PT_COMMA print_format			{ $$ = new PrintStatement($2,$4); }
		| KW_PRINT expr 						{ $$ = new PrintStatement($2,'d'); }
;

print_format: 	  KW_HEX			{ $$ = 1; }
				| KW_BIN			{ $$ = 2; }
				| KW_DEC			{ $$ = 3; }
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
	  | KW_VARINDEX						{ $$ = new VarExpr($1); }
	  | TK_LEFT_PAR expr TK_RIGHT_PAR	{ $$ = $2; }
;