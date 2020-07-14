# Variables
CC = gcc
CCFLAGS = -W -Wall -pedantic -O0 -g -fsanitize=address

all: main

# Object files
main.o: ./src/main.c
	$(CC) $(CCFLAGS) -c ./src/main.c

# Executable
main: ./src/main.o
	$(CC) $(CCFLAGS) -o ./build/chip8 ./src/main.o

# Clean object files
clean:
	rm -f ./src/*.o ./src/*~

# Clean object files and build files
mrproper: clean
	rm -f ./build/chip8