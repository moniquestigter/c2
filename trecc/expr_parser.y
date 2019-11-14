%require "3.0"
%language "c++"
%parse-param {ExprLexer& lexer}
%parse-param {YYNODESTATE& x}
%parse-param {ASTNode*& stmtList}
%define parse.error verbose 
%define api.value.type variant
%define api.parser.class {Parser}
%define api.namespace {Expr}

%code requires{
    #include <vector>
    #include <unordered_map>
    #include "expr_ast.h"
    class ExprLexer;
}

%{
    #include <stdio.h>
    #include <iostream>
    #include <vector>
    #include <unordered_map>
    #include <stdio.h>
    #include <string.h>
    #include <iostream>
    #include "expr_tokens.h"
    #include "expr_parser.h"
    #include "expr_ast.h"
    #define yylex(arg) lexer.getNextToken(arg)    
    namespace Expr{
        void Parser::error(const std::string& msg){
            std::cerr << msg << "\n";
        }
    }

    extern int yylineno;

%}

%type<ASTNode*> expr term factor
%type<ASTNode*> if opt_else statement statement_list input assign print while condicion

%token OP_ADD OP_SUB OP_MUL OP_DIV OP_ASIGN TK_SEMICOLON
%token TK_RIGHT_PAR TK_LEFT_PAR TK_RIGHT_CURLY_BRAKET TK_LEFT_CURLY_BRAKET
%token OP_EQUAL OP_DISTINCT OP_GRATER_EQUAL_THAN OP_LESS_EQUAL_THAN OP_GRATER_THAN OP_LESS_THAN
%token KW_SI KW_ENTONCES KW_ESCRIBA KW_FIN_SI KW_ELSE KW_INICIO KW_FIN KW_MIENTRAS KW_HAGA KW_FINMIENTRAS
%token<std::string> TK_IDENT
%token<int> TK_NUMBER
%token<int> TK_EOL
%token TK_ERROR

%%
input: statement_list		{ x.BlockSTCreate($1)->exec();}
;

statement_list: statement_list statement { $$ =  x.BlockSTCreate($1); x.BlockSTCreate($$)->list_st.push_back($2); }
			|	statement				 {x.BlockSTCreate($1)->list_st.push_back($1); }
;

statement: assign TK_SEMICOLON 			 { $$ = $1; }
		|	print TK_SEMICOLON 			 { $$ = $1; }
		|	while 						 { $$ = $1; }
		|   if							 { $$ = $1; }
;

assign: TK_IDENT OP_ASIGN condicion		{$$ = x.AssignSTCreate($1,$3);}
;

print: KW_ESCRIBA condicion				{$$ = x.PrintSTCreate($2);}
;

while: KW_MIENTRAS condicion KW_HAGA statement_list KW_FINMIENTRAS	{$$ = x.WhileSTCreate($2,$4);}
;

if: KW_SI condicion KW_ENTONCES statement_list opt_else				{$$ = x.IfSTCreate($2,$4,$5);}
;

opt_else: KW_ELSE statement_list									{$$ = $2;}
		| %empty                                                    {} 
;

condicion: expr OP_EQUAL expr 								{$$ =x.EqualExprCreate($1,$3);}
		|  expr OP_DISTINCT expr							{$$ =x.DistExprCreate($1,$3);}
		|  expr OP_GRATER_EQUAL_THAN expr					{$$ =x.GreaterEqExprCreate($1,$3);}
		|  expr OP_LESS_EQUAL_THAN expr						{$$ =x.LessEqExprCreate($1,$3);}
		|  expr OP_GRATER_THAN expr 						{$$ =x.GreaterExprCreate($1,$3);}
		|  expr OP_LESS_THAN expr							{$$ =x.LessExprCreate($1,$3);}
		|  expr 											{$$ =$1;}
;

expr: expr OP_ADD term	{$$=AddExprCreate($1,$3);}
	| expr OP_SUB term	{$$=SubExprCreate($1,$3);}
	| term				{$$ =$1;}
;


term: term OP_MUL factor	{$$=MulExprCreate($1,$3);}
	| term OP_DIV factor	{$$=DivExprCreate($1,$3);}
	| factor				{$$ = $1;}
;

factor:	TK_NUMBER						{$$=NumberCreate($1);}
	  | TK_IDENT						{$$=IdenCreate($1);}
	  | TK_LEFT_PAR expr TK_RIGHT_PAR	{ $$ = $2;}
;