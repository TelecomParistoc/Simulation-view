CXXFLAGS = -Wall -c -I$(INCLUDE_DIR)
LDFLAGS = -lglut -lGL -o
OFILES = view.o queue.o
PROG = view
CC = g++
LD = g++
RM = rm -f
EXEC = view

SRC_DIR= ./src
INCLUDE_DIR=./include

all: $(EXEC)

depend:
	$(CC) -MM $(SRC_DIR)/*.cpp -I$(INCLUDE_DIR) > depend

-include depend


$(EXEC): $(OFILES) depend
	$(LD) $(OFILES) $(LDFLAGS) $@

queue.o:
	$(CC) $(CXXFLAGS) $<

view.o:
	$(CC) $(CXXFLAGS) $<

clean:
	$(RM) $(OFILES) $(PROG) tmp.txt depend
