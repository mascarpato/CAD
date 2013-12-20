
C=g++

all: clean parser

%.o: %.cpp
	$C -c $^

parser: BinaryTree.o
	$C -o parser main.cpp $^

clean:
	rm -f *.o parser
