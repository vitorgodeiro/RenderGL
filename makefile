OBJS = main.cpp

CC = g++ -std=c++11

FLTK = `fltk-config --cxxflags --ldflags`

COMPILER_FLAGS = -Wall

LINKER_FLAGS = -lglfw -lGL -lGLEW -lglut -lfltk -lnanogui

OBJ_NAME = exe

all: $(OBJS)
	$(CC) $(OBJS) $(FLTK) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
