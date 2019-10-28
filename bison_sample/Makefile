TARGET=sample2
C_SRCFILES=main.c expr_parser.c expr_lexer.c
OBJ_FILES=${C_SRCFILES:.c=.o}
.PHONY:	clean

$(TARGET):	$(OBJ_FILES)
	gcc -g -o $@ $(OBJ_FILES)

expr_lexer.c:	expr.l
	flex -o $@ $^

expr_parser.c: expr.y
	bison --defines=tokens.h -o $@ $<

%.o: %.c tokens.h
	gcc -c -g -o $@ $<

run: $(TARGET)
	./$(TARGET) < input.txt

clean:
	rm -f expr_parser.c expr_lexer.c
	rm -f *.o
	rm -f tokens.h
	rm -f $(TARGET)