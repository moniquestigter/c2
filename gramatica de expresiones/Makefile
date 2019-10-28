TARGET=sample1
C_SRCFILES=main.c expr_parser.c sample1.c
OBJ_FILES=${C_SRCFILES:.c=.o}
.PHONY:	clean

$(TARGET):	$(OBJ_FILES)
	gcc -g -o $@ $(OBJ_FILES)

$(TARGET).c:	$(TARGET).l
	flex -o $@ $^

%.o: %.c tokens.h
	gcc -c -g -o $@ $<

run: $(TARGET)
	./$(TARGET) < input1.txt

clean:
	rm -f $(TARGET).c
	rm -f $(TARGET)