all:
		g++ -g main2.cpp Game.cpp GameObject.cpp -o main -lSDL2 -lSDL2_gfx -lSDL2_ttf
		./main