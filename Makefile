CXXFLAGS	= -O1 -Wall -Werror $(DEPFLAGS) -I$(INC_DIR)
DEPFLAGS	= -MD -MP
LDFLAGS		= -lglut -lGL
CC		= $(CXX)

SRC_DIR		:= src
INC_DIR		:= include

SRCS		= $(wildcard $(SRC_DIR)/*.cpp)
OBJS		= $(SRCS:$(SRC_DIR)/%.cpp=%.o)
EXE		= view
VPATH		= src

.PHONY: clean all

all: $(EXE)

$(EXE): $(OBJS)

clean:
	$(RM) *.o *.d $(EXE) tmp.txt

-include $(wildcard *.d)
