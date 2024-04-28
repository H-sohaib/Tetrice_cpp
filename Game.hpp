#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "GameObject.hpp"
#include <ctime>
#include <vector>
const int WIN_WIDTH = 1000;
const int WIN_HEIGHT = 500;
const int SHAPES_LINE_Y = WIN_HEIGHT - 105;

typedef struct Shape_Node
{
  GameObject *shape;
  Shape_Node *next;
  Shape_Node *prev;
} Shape_Node;

class Game
{
private:
public:
  // Load a font
  TTF_Font *font;
  GameObject *next_piece;
  std::vector<Shape_Node *> shapes_list;
  int win_width, win_height;
  bool isRunning = false;
  SDL_Window *window;
  SDL_Renderer *renderer;
  Game(const char *win_title, int width, int height);
  ~Game();
  void handel_events();
  void render();
  void update();
  void clear();
};

class Tetriste : public Game
{
public:
  Tetriste(const char *win_title, int width, int height) : Game(win_title, width, height) { srand(static_cast<unsigned int>(time(0))); }
  GameObject *return_random_shape(int x, int y);
  void show_random_shape(int x, int y);
  void start_tetriste();
};
