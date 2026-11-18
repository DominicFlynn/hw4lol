all: a.out

a.out: main.o bowling_frame.o
	g++ -std=c++11 main.o bowling_frame.o -o a.out

main.o: main.cpp bowling_frame.h
	g++ -std=c++11 -c main.cpp

bowling_frame.o: bowling_frame.cpp bowling_frame.h
	g++ -std=c++11 -c bowling_frame.cpp

clean:
	rm -f *.o a.out
