#include <stdio.h>

int expr();
int yylex();
void yyparse();

int main()
{
	yyparse();
	// printf("Result = %d\n", r);
}