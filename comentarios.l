
%{ 
#include <stdio.h>
int no_de_cb = 0; 
int no_de_cl = 0; 
%} 
  

%% 
"//".*                   ++no_de_cl; 
[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]       ++no_de_cb; 
.|\n {}
%% 
  

int yywrap(){} 
int main(int argc, char **argv) 
{ 
  
yylex(); 
printf("Comentarios de bloque = %d, Comentarios de linea = %d\n", 
       no_de_cb, no_de_cl ); 
  
return 0; 
} 