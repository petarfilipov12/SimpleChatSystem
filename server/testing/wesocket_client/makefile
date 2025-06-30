all: main

main: main.o
	g++ -o main main.o

main.o: main.cpp
	g++ -c main.cpp

rebuild: clean main

clean:
	rm main main.o