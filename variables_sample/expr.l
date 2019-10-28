%option noyywrap
%option yylineno

%x multiline_comment

%{
	#include <stdio.h>
	#include "tokens.h"
%}

%%
[ \t]	 {}
"/*"	 { BEGIN(multiline_comment); }
"//".*\n { return TK_EOL; }
"+"		 { return OP_ADD; }
"-"		 { return OP_SUB; }
"*"		 { return OP_MUL; }
"/"		 { return OP_DIV; }
"="		 { return OP_EQUAL; }
")"		 { return TK_RIGHT_PAR; }
"("		 { return TK_LEFT_PAR; }
"print"	 { return KW_PRINT; }
[0-9]+	 { yylval = atoi(yytext); return TK_NUMBER; }
"$"[0-7] { yylval = yytext[1] - '0'; return KW_VARINDEX; }
"\n"	 { return TK_EOL; }
.		 { return TK_ERROR; }

<multiline_comment>"*/" { BEGIN(INITIAL); }
<multiline_comment>.|\n {/*NADA*/}
<multiline_comment><<EOF>> {printf("Line %d: Block comment without close.\n",yylineno); return 0;}

%%
