%option noyywrap
%x comment
%{
	#include <stdio.h>
	#include "tokens.h"
%}

%%
"//".*                   /*nada*/;
"/*"                     BEGIN(comment); 

<comment>"*/" BEGIN(INITIAL);
<comment>.|\n /*nada*/;
<comment><<EOF>> BEGIN(INITIAL);
[ \t]	{}
"+"		{ return OP_ADD; }
"-"		{ return OP_SUB; }
"*"		{ return OP_MUL; }
"/"		{ return OP_DIV; }
\n    	{};
")"		{ return TK_RIGHT_PAR; }
"("		{ return TK_LEFT_PAR; }
[0-9]+	{ yylval.number_t = atoi(yytext); return TK_NUMBER; }
[a-zA-Z]* {yylval.str_t = strdup(yytext); return TK_IDENT;}
";"	{ return TK_SEMICOLON; }
.		{ return TK_ERROR; }
%%
