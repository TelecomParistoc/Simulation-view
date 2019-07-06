all: view.o queue.o
	g++ -Wall -lglut -lGL $^ -o view
queue.o: queue.c
	gcc -c -Wall $<
view.o: view.cpp
	g++ -c -Wall $<
clean:
	rm view
	rm *.o
