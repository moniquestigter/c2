%option noyywrap
%option reentrant
%x comment
%{
#include <cstdio>
#include <sstream>
#include "expr_tokens.h"
#include "expr_lexer.h"
int counter =0;
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
"#".*                   /*nada*/;
"{"                     BEGIN(comment); 

<comment>"}" BEGIN(INITIAL);
<comment>.|\n /*nada*/;
<comment><<EOF>> BEGIN(INITIAL);
DIGIT+ {yyval.emplace<int>(std::strol(yytext,nullptr,10));return makeToken(yytext,yyleng,token::Num); }
"-"     {return makeToken(yytext,yyleng,token::OpSub);}
"+"     {return makeToken(yytext,yyleng,token::OpAdd);}
"*"     {return makeToken(yytext,yyleng,token::OpDiv);}
"/"     {return makeToken(yytext,yyleng,token::OpMult);}
"("     {return makeToken(yytext,yyleng,token::OpenPar);}
")"     {return makeToken(yytext,yyleng,token::ClosePar);}
";"     {return makeToken(yytext,yyleng,token::Semicolon);}
<<EOF>> { return token::Eof; }
\n      {return makeToken(yytext,yyleng,token::Eol);}
({letter}|_)({DIGIT}|{letter}|_)+ {yyval.emplace<std::string>(yytext,yyleng);return makeToken(yytext,yyleng, token::Id);}

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
