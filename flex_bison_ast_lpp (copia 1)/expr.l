%option noyywrap
%x comment
%{
	#include <stdio.h>
	#include "./build/expr_tokens.h"
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
"<-"		{ return OP_ASIGN; }
"\n"	 	{ return TK_EOL; }
")"		{ return TK_RIGHT_PAR; }
"("		{ return TK_LEFT_PAR; }
"si"		 				{ return KW_SI; }
"entonces"		 			{ return KW_ENTONCES; }
"Escriba"		 			{ return KW_ESCRIBA; }
"fin si"					{ return KW_FIN_SI; }
"else"	 					{ return KW_ELSE; }
"inicio" 					{ return KW_INICIO; }
"fin"	 					{ return KW_FIN; }
[0-9]+						{ yylval.int_t = atoi(yytext); return TK_NUMBER; }
(_[A-Za-z]+)?[A-Za-z0-9_]* 	{ yylval.string_t = strdup(yytext); return TK_IDENT; }
"=="	 					{ return OP_EQUAL; }
"!="	 					{ return OP_DISTINCT; }
">="	 					{ return OP_GRATER_EQUAL_THAN; }
"<="	 					{ return OP_LESS_EQUAL_THAN; }
">"	 						{ return OP_GRATER_THAN; }
"<"	 						{ return OP_LESS_THAN; }
";"	{ return TK_SEMICOLON; }
.		{ return TK_ERROR; }
%%
