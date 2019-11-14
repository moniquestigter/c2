%option noyywrap
%option reentrant
%x comment
%{
#include <cstdio>
#include <sstream>
#include "expr_tokens.h"
#include "expr_lexer.h"
#define YY_EXTRA_TYPE std::istream*
#define YY_DECL int ExprLexer::_getNextToken(ExprLexer::semantic_type& yyval, yyscan_t yyscanner)
#define YY_INPUT(buf,result,max_size)\
    do{\
        std::istream* in = yyget_extra(yyscanner);\
        in->read(buf,max_size);\
        result = in->gcount();\
    }while(0)

%}
letter [_a-z_A-Z]
DIGIT [0-9]
%%
"//".*                   /*nada*/;
"/*"                     BEGIN(comment); 

<comment>"*/" BEGIN(INITIAL);
<comment>.|\n /*nada*/;
<comment><<EOF>> BEGIN(INITIAL);
[ \t]	{}
"+"		{ return OP_ADD; }
"-"		{ return OP_SUB; }
"*"		{ return OP_MUL; }
"/"		{ return OP_DIV; }
"<-"		{ return OP_ASIGN; }
"\n"	 	{ return TK_EOL; }
")"		{ return TK_RIGHT_PAR; }
"("		{ return TK_LEFT_PAR; }
"si"		 				{ return KW_SI; }
"entonces"		 			{ return KW_ENTONCES; }
"Escriba"		 			{ return KW_ESCRIBA; }
"fin si"					{ return KW_FIN_SI; }
"else"	 					{ return KW_ELSE; }
"inicio" 					{ return KW_INICIO; }
"fin"	 					{ return KW_FIN; }
"mientras"					{return KW_MIENTRAS;}
"haga"						{return KW_HAGA;}
"fin mientras"				{return KW_FINMIENTRAS;}
[0-9]+ {yyval.emplace<int>(std::strtol(yytext,nullptr,10));return TK_NUMBER;}
({letter}|_)({DIGIT}|{letter}|_)+ {yyval.emplace<std::string>(yytext,yyleng);return TK_IDENT;}
"=="	 					{ return OP_EQUAL; }
"!="	 					{ return OP_DISTINCT; }
">="	 					{ return OP_GRATER_EQUAL_THAN; }
"<="	 					{ return OP_LESS_EQUAL_THAN; }
">"	 						{ return OP_GRATER_THAN; }
"<"	 						{ return OP_LESS_THAN; }
";"	{ return TK_SEMICOLON; }
.		{ return TK_ERROR; }
<<EOF>> { return 0;}

%%

ExprLexer::ExprLexer(std::istream &in):in(in) {
    yylex_init_extra(&in,&scanner);
}

int ExprLexer::getLineNo() {
    return yyget_lineno(scanner);
}

ExprLexer::~ExprLexer() {
    yylex_destroy(scanner);
}
