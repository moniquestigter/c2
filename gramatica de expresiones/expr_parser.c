#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

int token;
int yylex();
extern char* yytext;

int expr();
int term();
int factor();

int expr()
{
	int result = term();
	switch(token){
		case OP_ADD:
		case OP_SUB: {
			while(token == OP_ADD || token == OP_SUB){
				int op = token;
				token = yylex();
				if(op==OP_ADD)
					result += term();
				else
					result -= term();
			}
			break;
		}
		default:
			break;
	}
	return result;
}

int term()
{
	int result = factor();
	switch(token){
		case OP_MUL: 
		case OP_DIV: {
			while(token == OP_MUL || token == OP_DIV){
				int op = token;
				token = yylex();
				if (op==OP_MUL)
					result *= factor();
				else
					result /= factor();
			}
			break;
		}
		default:
			break;
	}
	return result;
}

int factor()
{
	switch(token){
		case TK_NUMBER:{
			int result = atoi(yytext);
			token = yylex();
			return result;
		}
		case TK_LEFT_PAR: {
			token = yylex();
			int result = expr();
			if (token == TK_RIGHT_PAR)
			{
				token = yylex();
			}else
			{
				printf("Error expected token ')'\n");
			}
			return result;
		}
		default:
			printf("Error expected Number or '('\n");
			return 0;
	}
}