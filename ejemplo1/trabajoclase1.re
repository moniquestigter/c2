#include <stdio.h>
enum num_t {ERR,BIN,OCT,DEC,HEX};

static num_t lex(const char * YYCURSOR)
{
	const char *YYMARKER;
/*!re2c
re2c:define:YYCTYPE =char;
re2c:yyfill:enable=0;

end= "\x00";
bin ='0b'[01]+;
oct = "0" [0-7]*;

 *  {return ERR;}
oct end {return OCT;}
bin end {return BIN;}

*/

}

int main(int argc, char **argv)
{
	int i;
for(i=1;i<argc;++i)
{
switch(lex(argv[i])){

case BIN: printf("binario!"); break;
default: break;


}
}
}
//re2c -o integers.cpp integers.re
//g++ -o integers integers.cpp

