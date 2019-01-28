CPPFLAGS=-g

all: lz78

lz78: lz78.o
	g++ lz78.o -o lz78