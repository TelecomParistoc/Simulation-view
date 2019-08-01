CXXFLAGS = -Wall -c
LDFLAGS = -lglut -lGL -o
OFILES = view.o queue.o
PROG = view
CC = g++
LD = g++
RM = rm -fr

all: $(OFILES)
	$(LD) $^ $(LDFLAGS) view

queue.o: queue.cpp queue.h
	$(CC) $(CXXFLAGS) $<

view.o: view.cpp view.h
	$(CC) $(CXXFLAGS) $<

clean:
	$(RM) $(OFILES) $(PROG) tmp.txt
