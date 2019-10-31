#ifndef _AST_H
#define _AST_H

#include <string>
#include <list>
#include <unordered_map>
using namespace std;


class Expr
{
public:
	virtual std::string toString(){};
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
	Expr *expr1, *expr2;
};

class AddExpr: public BinaryExpr
{
public:
	AddExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1,expr2) {
	}
	std::string toString();
	int eval();
};

class SubExpr: public BinaryExpr
{
public:
	SubExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1,expr2) {}
	std::string toString();
	int eval();
};

class MulExpr: public BinaryExpr
{
public:
	MulExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1,expr2) {}
	std::string toString();
	int eval();
};

class DivExpr: public BinaryExpr
{
public:
	DivExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1,expr2) {}
	std::string toString();
	int eval();
};

class NumberExpr : public Expr
{
public:
	NumberExpr(int value)
	{
		this->value = value;
	}

	int value;
	std::string toString();
	int eval(){return value;}
};

class VarExpr : public Expr
{
public:
	VarExpr(char* index)
	{
		this->index = index;
	}

	char* index;
	std::string toString();
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
	AsignStatement(char* varIndex, Expr*expr){
		this->varIndex=varIndex;
		this->expr=expr;
	}
	~AsignStatement();

	void exec();
	
	Expr*expr;
	char* varIndex;
};

class PrintStatement : public Statement
{
public:
	PrintStatement(Expr*expr,int format){
		this->expr=expr;
		this->format=format;
	}
	~PrintStatement();

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