%option noyywrap
%x comment
%{ 
#include <stdio.h>

int block_comment=0;
int line_comment=0;

%}

%% 
"//".*                   ++line_comment; 
"/*"                     BEGIN(comment); 

<comment>"*/" ++block_comment; BEGIN(INITIAL);
<comment>.|\n /*nada*/;
<comment><<EOF>> BEGIN(INITIAL);
%% 



int main(int argc, char **argv) 
{ 
  
yylex(); 
printf("Comentarios de bloque = %d, Comentarios de linea = %d\n", 
        block_comment, line_comment ); 
  
return 0; 
} 