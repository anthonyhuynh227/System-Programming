CC = g++
CFLAGS = -Wall -g -std=c++17

# default target
ex6: ex6.o Vector.o
	$(CC) $(CFLAGS) -o ex6 ex6.o Vector.o

# individual source files
Vector.o: Vector.cc Vector.h
	$(CC) $(CFLAGS) -c Vector.cc

ex6.o: ex6.cc Vector.h
	$(CC) $(CFLAGS) -c ex6.cc

# phony target - delete built files (including OS X debug files)
clean:
	rm -rf ex6 *.o *~ *.dSYM