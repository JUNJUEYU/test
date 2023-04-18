CC = arm-linux-gnueabihf-gcc
CXX = arm-linux-gnueabihf-g++

OBJ_PATH = ./obj
SRC_PATH = ./src
INC_PATH = ./inc
LIB_PATH = ./lib
LIB_INC_PATH = ./lib/inc
INSTALL_PATH = ./install
OUTPUT_PATH = ./out

CFLAGS = -Wall -g -I$(INC_PATH) -I$(LIB_INC_PATH)
CXXFLAGS = -Wall -g -std=c++11 -I$(INC_PATH) -I$(LIB_INC_PATH)

LIBS = -L$(LIB_PATH) -lpthread -lm

CSRC = $(wildcard $(SRC_PATH)/*.c)
CXXSRC = $(wildcard $(SRC_PATH)/*.cpp)
COBJ = $(patsubst %.c, $(OBJ_PATH)/%.o, $(notdir $(CSRC)))
CXXOBJ = $(patsubst %.cpp, $(OBJ_PATH)/%.o, $(notdir $(CXXSRC)))

TARGET = $(OUTPUT_PATH)/main

.PHONY: all
all: $(TARGET)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(COBJ) $(CXXOBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

.PHONY: clean
clean:
	rm -rf $(OBJ_PATH)/*
	rm -rf $(TARGET)

.PHONY: install
install:
	mkdir -p $(INSTALL_PATH)
	cp -rf $(TARGET) $(INSTALL_PATH)/bin
	cp -rf $(LIB_PATH) $(INSTALL_PATH)/lib