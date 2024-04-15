all:
		g++ -g main.cpp Game.cpp GameObject.cpp -o main -lSDL2 -lSDL2_gfx -lSDL2_ttf
		gdb ./main