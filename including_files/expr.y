%{
	#include <stdio.h>

	int yylex();
	int vars[8];
	extern int yylineno;

	void yyerror(const char *msg)
	{
		printf("Line %d %s\n",yylineno,msg);
	}

	#define YYERROR_VERBOSE 1
%}

%token OP_ADD OP_SUB OP_MUL OP_DIV OP_EQUAL TK_RIGHT_PAR TK_LEFT_PAR 
%token KW_PRINT KW_VARINDEX
%token TK_NUMBER
%token TK_EOL
%token TK_ERROR

%%

stmts: stmts eofs stmt eofs 	{ $$ = $3; }
	|	stmt 			{ $$ = $1; }
	|
;

eofs: eofs TK_EOL	{ $$ = $2; }
	| TK_EOL
	|
;

stmt: asign					{ $$ = $1; }
	| KW_PRINT expr			{ printf("%d\n", $2); }
;

asign: KW_VARINDEX OP_EQUAL expr	{ vars[$1] = $3; }
;

expr: expr OP_ADD term	{ $$ = $1 + $3; }
	| expr OP_SUB term	{ $$ = $1 - $3; }
	| term				{ $$ = $1; }
;

term: term OP_MUL factor	{ $$ = $1 * $3; }
	| term OP_DIV factor	{ $$ = $1 / $3; }
	| factor				{ $$ = $1; }
;

factor:	TK_NUMBER						{ $$ = $1; }
	  | KW_VARINDEX						{ $$ = vars[$1]; }
	  | TK_LEFT_PAR expr TK_RIGHT_PAR	{ $$ = $2; }
;