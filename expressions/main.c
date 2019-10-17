#include <stdio.h>
int input();
int yylex();
extern int token;

int main(){
    token = yylex();
    int r = input();
    printf("result:",r);
}