all: Labwork2

Labwork2: Labwork2.o
	clang Labwork2.o -o Labwork2

Labwork2.o: Labwork2.c
	clang -c Labwork2.c

clean:
	rm -f *.o
