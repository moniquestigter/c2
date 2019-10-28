#include "ast.h"
int vars[8];

int AddExpr::eval(){
	int v1 = expr1->eval();
	int v2 = expr2->eval();
	return v1 + v2;
}

int SubExpr::eval(){
	int v1 = expr1->eval();
	int v2 = expr2->eval();
	return v1 - v2;
}

int MulExpr::eval(){
	int v1 = expr1->eval();
	int v2 = expr2->eval();
	return v1 * v2;
}

int DivExpr::eval(){
	int v1 = expr1->eval();
	int v2 = expr2->eval();
	return v1 / v2;
}

int VarExpr::eval(){
	return vars[index];
}

void AsignStatement::exec(){
	vars[varIndex] = expr->eval();
}

void PrintStatement::exec(){
	switch(format){
		case 1: printf("HEX: %x\n", expr->eval()); break;
		case 2: printBin(expr->eval()); break;
		default: printf("DEC: %d\n", expr->eval());
	}
}

void BlockStatement::exec(){
	for(list<Statement*>::iterator i = statementList.begin(); i!=statementList.end(); i++)
	{
		(*i)->exec();
	}
}