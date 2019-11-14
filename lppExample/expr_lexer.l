%option noyywrap
%option yylineno
%option reentrant


%x comment
%{
#include <cstdio>
#include <sstream>
#include "expr_lexer.h"

#define YY_EXTRA_TYPE std::istream*
#define YY_DECL int ExprLexer::_getNextToken(ExprLexer::semantic_type& yylval, yyscan_t yyscanner)
#define YY_INPUT(buf, result, max_size)\
do{\
    std::istream* in = yyget_extra(yyscanner);\
    in->read(buf,max_size);\
    result = in->gcount();\
}while(0)

%}

%%

"Escriba" {
    //printf("%s TK_PRINT\n", yytext); 
    return makeToken(yytext, yyleng, token::TK_PRINT);
}

"Si" {
    //printf("%s TK_IF\n", yytext); 
    return makeToken(yytext, yyleng, token::TK_IF);
}

"entonces" {
     //printf("%s TK_THEN\n", yytext); 
    return makeToken(yytext, yyleng, token::TK_THEN);
}

"sino" {
    //printf("%s TK_ELSE\n", yytext);
    return makeToken(yytext, yyleng, token::TK_ELSE);
}

"Mientras" {
    //printf("%s TK_WHILE\n", yytext);
    return makeToken(yytext, yyleng, token::TK_WHILE); 
}

"haga" {
    //printf("%s TK_DO\n", yytext);
    return makeToken(yytext, yyleng, token::TK_DO); 
}

"fin mientras" {
    //printf("%s TK_ENDWHILE\n", yytext);
    return makeToken(yytext, yyleng, token::TK_ENDWHILE);  
}

"fin si" {
    //printf("%s TK_ENDIF\n", yytext);
    return makeToken(yytext, yyleng, token::TK_ENDIF); 
}

[0-9]+ {
    yylval.emplace<int>(std::strtol(yytext, nullptr,10));
    //printf("%s NUMBER\n", yytext);
    return makeToken(yytext, yyleng, token::TK_NUMBER);
    
}

[a-zA-Z_][0-9a-zA-Z_]* {
    yylval.emplace<std::string>(yytext, yyleng);
    //printf("%s IDENTIFIER\n", yytext);
    return makeToken(yytext, yyleng, token::TK_IDENT);
    
}

"<-" {
    //printf("%s TK_ASSIGN\n", yytext);
    return makeToken(yytext, yyleng, token::TK_ASSIGN);
}

"(" {
    //printf("%s TK_LEFTPAR\n", yytext);
    return makeToken(yytext, yyleng, token::TK_LEFTPAR);
}

")" {
    //printf("%s TK_RIGHTPAR\n", yytext);
    return makeToken(yytext, yyleng, token::TK_RIGHTPAR);
}

"+"|"-"|"*"|"/" {
    if(yytext[0] == '+'){
        //printf("%s TK_SUMA\n", yytext);
        return makeToken(yytext, yyleng, token::TK_SUMA);
    } else if(yytext[0] == '-'){
        //printf("%s TK_RESTA\n", yytext);
        return makeToken(yytext, yyleng, token::TK_RESTA);
    } else if(yytext[0] == '*'){
        //printf("%s TK_MULTI\n", yytext);
        return makeToken(yytext, yyleng, token::TK_MULTI);
    }else if(yytext[0] == '/'){
        //printf("%s TK_DIV\n", yytext);
        return makeToken(yytext, yyleng, token::TK_DIV);
    }
}

"==" { return makeToken(yytext, yyleng, token::OP_EQUAL); } 
"<>" { return makeToken(yytext, yyleng, token::OP_NOTEQUAL); } 
"<" { return makeToken(yytext, yyleng, token::OP_LESSTHAN); } 
">" { return makeToken(yytext, yyleng, token::OP_GREATERTHAN); }
"<=" { return makeToken(yytext, yyleng, token::OP_LESSTHANEQUAL); }  
">=" { return makeToken(yytext, yyleng, token::OP_GREATERTHANEQUAL); } 

"^" {
    //printf("%s TK_SEMICOLON\n", yytext);
    return makeToken(yytext, yyleng, token::TK_POWER); 
}

";" {
    //printf("%s TK_SEMICOLON\n", yytext);
    return makeToken(yytext, yyleng, token::TK_SEMICOLON); 
}

"\n" {
    //printf("%s TK_EOL\n", yytext);
    return makeToken(yytext, yyleng, token::TK_EOL); 
}

"//"[^\n]* BEGIN(INITIAL);
"/*" BEGIN(comment);

<comment>"*/" BEGIN(INITIAL);
<comment>.|\n /*NOTHING*/;
<comment><<EOF>> BEGIN(INITIAL);

[ |\t]+                  /* ignore whitespace */;

. { return makeToken(yytext, yyleng, token::TK_ERROR); }


%%

ExprLexer::ExprLexer(std::istream &in) : in(in){
    yylex_init_extra(&in, &scanner); 
}

ExprLexer::~ExprLexer() {
    yylex_destroy(scanner);
}

int ExprLexer::getLineNo() {
    return yyget_lineno(scanner);
}