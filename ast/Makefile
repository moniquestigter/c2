TARGET=sample5
C_SRCFILES=main.cpp expr_parser.cpp expr_lexer.cpp ast.cpp
OBJ_FILES=${C_SRCFILES:.cpp=.o}
.PHONY:	clean

$(TARGET):	$(OBJ_FILES)
	g++ -g -o $@ $(OBJ_FILES)

expr_lexer.cpp:	expr.l
	flex -o $@ $^

expr_parser.cpp: expr.y
	bison --defines=tokens.h -o $@ $<

%.o: %.cpp tokens.h
	g++ -c -g -o $@ $<

run: $(TARGET)
	./$(TARGET) input.txt

clean:
	rm -f expr_parser.cpp expr_lexer.cpp
	rm -f *.o
	rm -f tokens.h
	rm -f $(TARGET)