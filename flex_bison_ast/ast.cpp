#include "ast.h"
std::unordered_map<std::string, int> vars;

std::string AddExpr::toString(){
	return "(" + expr1->toString() +")" + "+" + "(" + expr2->toString() +")";	
}

std::string SubExpr::toString(){
	return "(" + expr1->toString() +")" + "-" + "(" + expr2->toString() +")";	
}

std::string MulExpr::toString(){
	return "(" + expr1->toString() +")" + "*" + "(" + expr2->toString() +")";	
}

std::string DivExpr::toString(){
	return "(" + expr1->toString() +")" + "/" + "(" + expr2->toString() +")";	
}

std::string VarExpr::toString(){
	return index;
}


std::string NumberExpr::toString(){
	return std::to_string(value); 
}


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