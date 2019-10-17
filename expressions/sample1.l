%option noyywrap


%{ 
#include <stdio.h>

%}

%% 
"//".*                   /*nada*/;
"/*"                     BEGIN(comment); 

[a-z][a-z0-9]* {RETURN ID; printf("ID" %s\n,yytext;)}
[0-9]+ { return INT; printf("NUMBER %s\n", yytext); }
"+"    { return ADD; printf("PLUS\n"); }
"-"    { RETURN SUBTRACT; printf("MINUS\n"); }
"*"    { RETURN MULTIPLY; printf("TIMES\n"); }
"/"    { RETURN DIVIDE; printf("DIVIDE\n"); }
";"    { RETURN DIVIDE; printf("DIVIDE\n"); }
\n     { printf("NEWLINE\n"); }
[ \t]  { }
.      { printf("Mystery character %s\n", yytext); }

<comment>"*/" BEGIN(INITIAL);
<comment>.|\n /*nada*/;
<comment><<EOF>> BEGIN(INITIAL);
%% 