%option noyywrap
%option yylineno

%x multiline_comment
%x include

%{
	#include <stdio.h>
	#include "tokens.h"

	typedef struct include_stack
	{
		struct include_stack *prev;
		FILE *f;
		int lineno;
		char *filename;
		YY_BUFFER_STATE bs;
	} include_stack_t;

	include_stack_t *inc_stack = NULL;
	char *yyfilename;
	int push_file(char *filename);
	int pop_file();
%}

%%
[ \t]	 {}
^"#"[ \t]*"include"[ \t]*\"	{ BEGIN(include); }
"/*"	 					{ BEGIN(multiline_comment); }
"//".*\n 					{ return TK_EOL; }
"+"		 					{ return OP_ADD; }
"-"		 					{ return OP_SUB; }
"*"		 					{ return OP_MUL; }
"/"		 					{ return OP_DIV; }
"="		 	{ return OP_EQUAL; }
")"		 	{ return TK_RIGHT_PAR; }
"("		 	{ return TK_LEFT_PAR; }
"print"	 	{ return KW_PRINT; }
[0-9]+	 	{ yylval = atoi(yytext); return TK_NUMBER; }
"$"[0-7] 	{ yylval = yytext[1] - '0'; return KW_VARINDEX; }
"\n"	 	{ return TK_EOL; }
<INITIAL><<EOF>>	 	{ if(pop_file()!=0) yyterminate(); }
.		 	{ return TK_ERROR; }

<include>[^\"]+	{
						{
							printf("yytext: %s\n",yytext);
							int c;
							while((c = input()) && c!='\n');
							yylineno++;
							if(push_file(yytext)!=0){
								yyterminate();
							}
							BEGIN(INITIAL);
						}
					}

<multiline_comment>"*/" { BEGIN(INITIAL); }
<multiline_comment>.|\n {/*NADA*/}
<multiline_comment><<EOF>> {printf("Line %d: Block comment without close.\n",yylineno); return 0;}

%%

int push_file(char *filename){
	FILE *f = fopen(filename,"r");

	if (f == NULL)
	{
		fprintf(stderr, "File %s not found\n", filename);
		return 1;
	}

	include_stack_t *is = malloc(sizeof(include_stack_t));
	if (is == NULL)
	{
		fprintf(stderr, "Out of memory");
		return 1;
	}

	is->f = f;
	is->lineno = yylineno;
	is->filename = yyfilename;
	is->bs = yy_create_buffer(f,YY_BUF_SIZE);
	is->prev = inc_stack;
	inc_stack = is;
	yyfilename = filename;
	yylineno = 1;
	yy_switch_to_buffer(is->bs);
	return 0;
}

int pop_file(){
	include_stack_t *prev_is = inc_stack->prev;
	fclose(inc_stack->f);
	yy_delete_buffer(inc_stack->bs);

	yylineno = inc_stack->lineno;
	yyfilename = inc_stack->filename;

	free(inc_stack);
	if(prev_is == NULL) return 1;
	yy_switch_to_buffer(prev_is->bs);
	inc_stack = prev_is;
	return 0;
}