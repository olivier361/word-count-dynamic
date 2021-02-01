# To run:
#      $ make all 
#      $ make word_count
#      $ make vagrind
#      $ make clean 

CC=gcc -Wall -g -std=c99
CFLAGS=-lm
LIBS =-I. 

all: clean word_count valgrind 

clean:
	-rm word_count 

word_count: clean word_count_

word_count_ : src/word_count.c src/*.c 
	$(CC) $^ -o word_count $(CFLAGS) $(LIBS)

valgrind: word_count
	@valgrind ./word_count --infile cases/valgrind.txt
	@valgrind ./word_count --infile cases/valgrind.txt --sort
