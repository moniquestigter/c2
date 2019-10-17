TARGET=sample1
.PHONY:	clean

$(TARGET):	$(TARGET).c tokens.h
	gcc -o $@ $<

$(TARGET).c:	$(TARGET).l
	flex -o $@ $^

run: $(TARGET)
	./$(TARGET) < input1.txt

clean:
	rm -f $(TARGET).c
	rm -f $(TARGET)