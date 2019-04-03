OBJS = main.cpp

CC = g++ -std=c++11

COMPILER_FLAGS = -Wall

LINKER_FLAGS = -lglfw -lGL -lGLEW -lglut

OBJ_NAME = exe

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
