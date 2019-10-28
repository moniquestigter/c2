#include <stdio.h>

int expr();
int yylex();
void yyparse();

extern FILE * yyin;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
		return 1;
	}
	int c = 1;
	while(c<argc)
	{
		yyin = fopen(argv[c],"r");
		if (yyin == NULL)
		{
			fprintf(stderr, "Cannot open file %s\n", argv[c]);
			return 1;
		}
		printf("Executing file: %s\n", argv[c]);
		yyparse();
		fclose(yyin);
		c++;
	}
	// printf("Result = %d\n", r);
}