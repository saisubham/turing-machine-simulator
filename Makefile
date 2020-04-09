CC=g++
CFLAGS=-c

all: tm

tm: main.o turing_machine.o
	$(CC) main.o turing_machine.o -o tm

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

turing_machine.o: turing_machine.cpp
	$(CC) $(CFLAGS) turing_machine.cpp

clean:
	rm -rf *.o tm
