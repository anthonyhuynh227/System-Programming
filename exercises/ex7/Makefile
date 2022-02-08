CC = g++
CFLAGS = -Wall -g -std=c++17

# default target
ex7: ex7.o Vector.o
	$(CC) $(CFLAGS) -o ex7 ex7.o Vector.o

# individual source files
Vector.o: Vector.cc Vector.h
	$(CC) $(CFLAGS) -c Vector.cc

ex7.o: ex7.cc Vector.h
	$(CC) $(CFLAGS) -c ex7.cc

# phony target - delete built files (including OS X debug files)
clean:
	rm -rf ex7 *.o *~ *.dSYM