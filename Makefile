all: view.cpp
	g++ view.cpp -lglut -lGL -o view -Wall

clean:
	rm view
