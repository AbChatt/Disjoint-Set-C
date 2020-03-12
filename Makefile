CC = gcc
CLAGS = -Wall -Werror -g -lm
DEPS = MST.h
OBJ = TestMST.o MST.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

TestMST: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm $(OBJ) ./TestMST
