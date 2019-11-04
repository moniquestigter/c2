
/* Don't remove this option */
%option reentrant

%{
#include <iostream>
#include "expr_lexer.h"


#define YY_EXTRA_TYPE std::istream*
#define YY_DECL int ExprLexer::_getNextToken
#define YY_INPUT(buf, result, max_size)                 \
        do {                                            \
            std::istream *in = yyget_extra(yyscanner);  \
            yy_size_t n;                                \
            int c = '*';                                \
            for(n = 0; n < max_size; ++n ){             \
                in->get(c);                             \
                buf[n] = (char) c;                      \
                if ( c == '\n' )                                        \
			        buf[n++] = (char) c;                                \
                if ( c == EOF && ferror( yyin ) )                       \
			        YY_FATAL_ERROR( "input in flex scanner failed" );   \
		        result = n;                                             \
            }                                                           \
        } while (0);

int depth = 0;
%}

ID  [a-zA-Z_][a-zA-Z0-9_]*
NUM [0-9]

%%
"+"     { return makeToken(yytext,yyleng,Token::OpAdd); }
"-"     { return makeToken(yytext,yyleng,Token::OpSub); }
"*"     { return makeToken(yytext,yyleng,Token::OpMul); }
"/"     { return makeToken(yytext,yyleng,Token::OpDiv); }
"("     { return makeToken(yytext,yyleng,Token::OpenPar); }
")"     { return makeToken(yytext,yyleng,Token::ClosePar); }
";"     { return makeToken(yytext,yyleng,Token::Semicolon); }
{NUM}+  { return makeToken(yytext,yyleng,Token::Num); yylval.emplace<int>(std::Strtol(yytext,nullptr,10)); }
{ID}    { return makeToken(yytext,yyleng,Token::Id); yylval.emplace<std::string>(yytext,yyleng); }
<<EOF>> { return eofToken(); }
.       { return errorToken(); }
%%

ExprLexer::ExprLexer(std::istream &in) : in(in) {
    yylex_init_extra(&in, &scanner);
}

ExprLexer::~ExprLexer() {
    yylex_destroy(scanner);
}
