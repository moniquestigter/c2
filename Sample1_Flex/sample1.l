%option noyywrap

%{
	#include <stdio.h>
	#include <token.h>
}

%%
"+"		{ return OP_ADD; }
"-"		{ return OP_SUB; }
"*"		{ return OP_MUL; }
"/"		{ return OP_DIV; }
")"		{ return TK_RIGHT_PAR; }
"("		{ return TK_LEFT_PAR; }
[0-9]	{ return TK_NUMBER; }
"\n"	{ return TK_EOL; }
<<EOF>>	{ return TK_EOF; }
.		{ return TK_ERROR; }

%%
int main()
{
	yylex();
}