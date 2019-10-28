%option noyywrap

%{
	#include <stdio.h>
	#include "tokens.h"
%}

%%
[ \t]	{}
"+"		{ return OP_ADD; }
"-"		{ return OP_SUB; }
"*"		{ return OP_MUL; }
"/"		{ return OP_DIV; }
")"		{ return TK_RIGHT_PAR; }
"("		{ return TK_LEFT_PAR; }
[0-9]	{ yylval = atoi(yytext); return TK_NUMBER; }
"\n"	{ return TK_EOL; }
.		{ return TK_ERROR; }

%%
