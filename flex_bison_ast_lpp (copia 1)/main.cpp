#include <stdio.h>

int expr();
int yylex();
void yyparse();

int main(int argc, char *argv[])
{
	yyparse();
}
