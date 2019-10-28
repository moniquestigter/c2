#ifndef _AST_H
#define _AST_H

#include <stdio.h>
#include <list>
using namespace std;

// class AST
// {
// public:
// 	AST();
// 	~AST();
	
// 	Statement *statements;
// 	Expr *expressions;
// };

class Expr
{
public:
	Expr(){}
	~Expr();
	
	virtual int eval() = 0;
};

class BinaryExpr: public Expr
{
protected:
	BinaryExpr(Expr *expr1, Expr *expr2){
		this->expr1 = expr1;
		this->expr2 = expr2;
	}
public:
	~BinaryExpr();
	
	Expr *expr1, *expr2;
};

class AddExpr: public BinaryExpr
{
public:
	AddExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1,expr2) {}
	~AddExpr();
	int eval();
};

class SubExpr: public BinaryExpr
{
public:
	SubExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1,expr2) {}
	~SubExpr();
	int eval();
};

class MulExpr: public BinaryExpr
{
public:
	MulExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1,expr2) {}
	~MulExpr();
	int eval();
};

class DivExpr: public BinaryExpr
{
public:
	DivExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1,expr2) {}
	~DivExpr();
	int eval();
};

class NumberExpr : public Expr
{
public:
	NumberExpr(int value)
	{
		this->value = value;
	}
	~NumberExpr();

	int value;

	int eval(){return value;}
};

class VarExpr : public Expr
{
public:
	VarExpr(int index)
	{
		this->index = index;
	}
	~VarExpr();

	int index;

	int eval();
};

class Statement
{
public:
	virtual void exec() = 0;
protected:
	Statement(){}
	~Statement();
	
};

class LogicalExpression : public Expr
{
public:
	LogicalExpression(Expr*expr1,int comparator, Expr * expr2){
		this->expr1=expr1;
		this->expr2=expr2;
		this->comparator=comparator;
	}
	~LogicalExpression();
	
	Expr *expr1, *expr2;
	int comparator;

	int eval();
};

class AsignStatement : public Statement
{
public:
	AsignStatement(int varIndex, Expr*expr){
		this->varIndex=varIndex;
		this->expr=expr;
	}
	~AsignStatement();

	void exec();
	
	Expr*expr;
	int varIndex;
};

class PrintStatement : public Statement
{
public:
	PrintStatement(Expr*expr,int format){
		this->expr=expr;
		this->format=format;
	}
	~PrintStatement();

	int convertBin(int decimalnum)
	{
		if (decimalnum == 0)
	    {
	        return 0;
	    }
	    else
	    {
	        return (decimalnum % 2 + 10 * convertBin(decimalnum / 2));
	    }
	}

	void printBin(int decimalnum)
	{
	    printf("BIN: %d\n", convertBin(decimalnum));
	}

	void exec();
	
	Expr *expr;
	int format;
};

class BlockStatement : public Statement
{
public:
	BlockStatement(){}
	~BlockStatement();

	void addStatement(Statement*statement){
		statementList.push_back(statement);
	}

	void exec();

	list<Statement*> statementList;
	
};

class IfStatement : public Statement
{
public:
	IfStatement(Expr*expr, Statement*block, Statement*else_block){
		this->expr = expr;
		this->block = block;
		this->else_block = else_block;
	}
	~IfStatement();
	
	Expr *expr;
	Statement *block;
	Statement *else_block;

	void exec();
};

class ElseStatement : public Statement
{
public:
	ElseStatement(Statement*block){
		this->block=block;
	}
	~ElseStatement();
	
	Statement *block;

	void exec();
};

#endif