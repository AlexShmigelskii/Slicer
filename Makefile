all: main

CC=clang

main: main.o math3d.o mesh.o

clean:
	rm -f main *.o house*.stl