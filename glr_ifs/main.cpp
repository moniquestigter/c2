#include <stdio.h>

int expr();
int yylex();
void yyparse();
int push_file(char *filename);

extern FILE * yyin;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
		return 1;
	}
	if(push_file(argv[1])!=0)
		return 1;
	yyparse();
}