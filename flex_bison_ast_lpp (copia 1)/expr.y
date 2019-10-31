%code requires {
	#include "ast.h"
}
%{
	#include <string.h>
	#include <stdio.h>
	#include <iostream>
	int yylex();
	#define EQUAL 1
	#define DISTINCT 2
	#define GRATER_EQUAL_THAN 3
	#define LESS_EQUAL_THAN 4
	#define GRATER_THAN 5
	#define LESS_THAN 6
	void yyerror(const char *msg)
	{
		printf("%s\n",msg);
	}

	#define YYERROR_VERBOSE 1
%}

%union {
	Statement *statement_t;
	Expr *expr_t;
	char *string_t;
	int int_t;
}

%type <statement_t> input_src stmt asign print stmts if_stmt optional_block_stmt
%type <expr_t> expr term factor logic_expression
%type <int_t> logic_operator

%token OP_ADD OP_SUB OP_MUL OP_DIV OP_ASIGN TK_SEMICOLON
%token TK_RIGHT_PAR TK_LEFT_PAR TK_RIGHT_CURLY_BRAKET TK_LEFT_CURLY_BRAKET
%token OP_EQUAL OP_DISTINCT OP_GRATER_EQUAL_THAN OP_LESS_EQUAL_THAN OP_GRATER_THAN OP_LESS_THAN
%token KW_SI KW_ENTONCES KW_ESCRIBA KW_FIN_SI KW_ELSE KW_INICIO KW_FIN
%token<string_t> TK_IDENT
%token<int_t> TK_NUMBER
%token<int_t> TK_EOL
%token TK_ERROR

%%
input_src: stmts		{ $$ = $1; $$->exec(); }
;


stmts: stmts eols stmt	{ $$ = $1; ((BlockStatement*)$$)->addStatement($3); }
	|	stmt		{ $$ = new BlockStatement; ((BlockStatement*)$$)->addStatement($1); }
;

eols: eols TK_EOL
	| TK_EOL
;

stmt: asign					{ $$ = $1; }
	| print					{ $$ = $1; }
	| if_stmt
;

if_stmt:KW_SI logic_expression KW_ENTONCES eols optional_block_stmt eols optional_block_stmt 	{ $$ = new IfStatement($2,$5,$7); }
;

optional_block_stmt: KW_ELSE eols stmts eols { $$ = $3; }
					| stmt { $$ = $1; }
;

logic_expression: expr logic_operator expr { $$ = new LogicalExpression($1,$2,$3); }
;

logic_operator:   OP_EQUAL 				{ $$ = EQUAL; }
				| OP_DISTINCT			{ $$ = DISTINCT; }
				| OP_GRATER_EQUAL_THAN	{ $$ = GRATER_EQUAL_THAN; }
				| OP_LESS_EQUAL_THAN	{ $$ = LESS_EQUAL_THAN; }
				| OP_GRATER_THAN 		{ $$ = GRATER_THAN; }
				| OP_LESS_THAN 			{ $$ = LESS_THAN; }
;


asign: TK_IDENT OP_ASIGN expr	{ $$ = new AsignStatement($1,$3); }
;

print: KW_ESCRIBA expr 			{ $$ = new PrintStatement($2,'d'); }
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
	  | TK_IDENT						{ $$ = new VarExpr($1);free($1); }
	  | TK_LEFT_PAR expr TK_RIGHT_PAR	{ $$ = $2; }
;
