CC = g++
LD = g++
CXXFLAGS = -Wall  $(DEPFLAGS) -c -I$(INC_DIR)
DEPFLAGS = -MT $@ -MF $(DEP_DIR)/$*.d -MMD -MP
LDFLAGS = -lglut -lGL
RM = rm -f

SRC_DIR := src
OBJ_DIR := obj
INC_DIR := include
DEP_DIR := $(OBJ_DIR)/.deps

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS := $(SRCS:$(SRC_DIR)/%.cpp=$(DEP_DIR)/%.d)
EXEC = view

all: $(EXEC)

$(EXEC): $(OBJS)
	$(LD) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d | $(DEP_DIR)
	$(CC) $(CXXFLAGS) $< -o $@

$(DEP_DIR): ; @mkdir -p $@

.PHONY: clean

clean:
	$(RM) $(OBJS) $(EXEC) $(DEPS) tmp.txt

$(DEPS):
include $(wildcard $(DEPS))
