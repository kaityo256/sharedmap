all: a.out

CC=mpic++
CPPFLAGS=-std=c++11

a.out: main.cpp serializablemap.hpp
	$(CC) $(CPPFLAGS) $< -o $@

clean:
	rm -f a.out
