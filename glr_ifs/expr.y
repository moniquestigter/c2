%code requires {
	#include "ast.h"
}

%{
	#include <stdio.h>

	#define EQUAL 1
	#define DISTINCT 2
	#define GRATER_EQUAL_THAN 3
	#define LESS_EQUAL_THAN 4
	#define GRATER_THAN 5
	#define LESS_THAN 6

	int yylex();
	extern int yylineno;

	void yyerror(const char *msg)
	{
		printf("Line %d %s\n",yylineno,msg);
	}

	#define YYERROR_VERBOSE 1
	#define YYDEBUG 1

%}

%glr-parser
%expect 1

%union {
	Statement *statement_t;
	Expr *expr_t;
	char *string_t;
	int int_t;
}

%type <statement_t> input_src stmt asign print stmts if_stmt optional_block_stmt
%type <expr_t> expr term factor logic_expression
%type <int_t> print_format logic_operator

%token OP_ADD OP_SUB OP_MUL OP_DIV OP_ASIGN 
%token TK_RIGHT_PAR TK_LEFT_PAR TK_RIGHT_CURLY_BRAKET TK_LEFT_CURLY_BRAKET
%token OP_EQUAL OP_DISTINCT OP_GRATER_EQUAL_THAN OP_LESS_EQUAL_THAN OP_GRATER_THAN OP_LESS_THAN
%token KW_IF KW_ELSE
%token KW_PRINT 
%token<string_t> KW_VARINDEX
%token<int_t> TK_NUMBER
%token PT_COMMA
%token<int_t> KW_HEX KW_DEC KW_BIN
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

optional_eols: eols
			|
;

stmt: asign					{ $$ = $1; }
	| print					{ $$ = $1; }
	| if_stmt
;

if_stmt: KW_IF TK_LEFT_PAR logic_expression TK_RIGHT_PAR eols optional_block_stmt eols KW_ELSE eols optional_block_stmt  %dprec 1	{ $$ = new IfStatement($3,$6,$10); }
	|	 KW_IF TK_LEFT_PAR logic_expression TK_RIGHT_PAR eols optional_block_stmt 										 %dprec 2	{ $$ = new IfStatement($3,$6,NULL); }
;

optional_block_stmt: TK_LEFT_CURLY_BRAKET optional_eols stmts optional_eols TK_RIGHT_CURLY_BRAKET { $$ = $3; }
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

asign: KW_VARINDEX OP_ASIGN expr	{ $$ = new AsignStatement($1,$3); }
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