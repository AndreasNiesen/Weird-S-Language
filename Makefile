CC=g++
CFLAGS=-std=c++14

weirdslang: weirdS.cpp
	$(CC) $(CFLAGS) -o weirdSLang weirdS.cpp
