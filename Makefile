CXX=g++
CXXFLAGS=

SRC=main.cpp turing_machine.cpp
OBJ=${SRC:.cpp=.o}
OUTPUT=tm

all: ${OUTPUT}

tm: ${OBJ}
	${LINK.cpp} main.o turing_machine.o -o ${OUTPUT}

%.o: %.c
	${COMPILE.cpp} -c $+ -o $@

clean:
	rm -rf ${OBJ} ${OUTPUT}
