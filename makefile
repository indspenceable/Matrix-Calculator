CC = g++
CFLAGS = -pedantic -Wall -g
PROGS = calculator

all: $(PROGS)

calculator: Expression.o HelpTopic.o Interpreter.o Matrix.o
	$(CC) $(CFLAGS) -o calculator main.cpp Expression.o HelpTopic.o Interpreter.o Matrix.o

make clean:
	rm -f *.o $(PROGS)