all: view.o queue.o
	g++ -Wall -lglut -lGL $^ -o view
queue.o: queue.c
	gcc -c -Wall $<
view.o: view.cpp
	g++ -c -Wall $<

clean:
ifneq ($(wildcard view),)
	rm view
endif
ifneq ($(wildcard *.o),)
	rm *.o
endif
ifneq ($(wildcard tmp.txt),)
	rm tmp.txt
endif
