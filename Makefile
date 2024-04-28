# Sources specifies all cpp files to compile as part of the project
SOURCES = main.cpp Game.cpp GameObject.cpp

# OBJ_NAME specifies the name of our executable
OBJ_NAME = main

# Compiler flags
CFLAGS = -g

# Libraries to link
LIBS = -lSDL2 -lSDL2_gfx -lSDL2_ttf

all: $(SOURCES)
	g++ $(CFLAGS) $(SOURCES) -o $(OBJ_NAME) $(LIBS)
	gdb ./$(OBJ_NAME)
