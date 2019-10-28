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
#endif