CC=g++
CFLAGS=-std=c++2a

weirdslang: weirdS.cpp
	$(CC) $(CFLAGS) -o weirdSLang weirdS.cpp
