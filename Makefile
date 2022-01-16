all: file.cpp
	g++ -std=c++11 -O3 -Wall file.cpp -lm -o main

clean:
	rm main
