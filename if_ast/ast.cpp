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

int LogicalExpression::eval(){
	int v1 = expr1->eval();
	int v2 = expr2->eval();
	switch(comparator){
		case 1: return v1 == v2;
		case 2: return v1 != v2;
		case 3: return v1 >= v2;
		case 4: return v1 <= v2;
		case 5: return v1 > v2;
		case 6: return v1 < v2;
		default: printf("Not suported operation!\n"); return 0;
	}
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

void IfStatement::exec(){
	int v = expr->eval();
	if(v!=0)
		block->exec();
	else if(else_block!=NULL){
		else_block->exec();
	}
}

void ElseStatement::exec(){
	block->exec();
}