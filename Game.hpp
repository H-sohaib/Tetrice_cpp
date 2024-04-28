#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "SimpleLinkedList.hpp"
#include "GameObject.hpp"
#include <cstdlib> // For rand() and srand()
#include <ctime>
#include <iostream>

const int WIN_WIDTH = 1000;
const int WIN_HEIGHT = 500;
const int SHAPES_LINE_Y = WIN_HEIGHT - 105;
const int padding = 10;

enum class Response
{
  REMOVED_COLOR,
  REMOVED_SHAPE,
  LOSE,
  WIN,
  NONE
};

class Game
{
public:
  SDL_Event event;
  SDL_Window *window;
  SDL_Renderer *renderer;
  TTF_Font *font;
  SDL_Texture *staticElementsTexture;

  // int win_width, win_height;
  bool isRunning = false;
  Game()
  {
    window = nullptr;
    renderer = nullptr;
    font = nullptr;
    staticElementsTexture = nullptr;
  };
  // ~Game();
  // void handel_events();
  // void render();
  // void update();
  // void clear();
};

class Tetriste : public Game
{
public:
  SimpleLinkedList pieces_list;
  SimpleLinkedList rand_pieces_list;
  // ----
  SimpleLinkedList square_nodes;
  SimpleLinkedList circle_nodes;
  SimpleLinkedList triangle_nodes;
  SimpleLinkedList Rhambus_nodes;
  SimpleLinkedList blue_nodes;
  SimpleLinkedList yellow_nodes;
  SimpleLinkedList red_nodes;
  SimpleLinkedList green_nodes;

  Tetriste() { srand(static_cast<unsigned int>(time(0))); };
  int game_init();
  void setup_static_UI();
  void start_tetriste();
  void handel_events();
  GameObject *copyShapeNode(Node *OriginNode);
  GameObject *return_random_shape(int x, int y);
  void insert2mainList(bool isRight);
  Response removeConsecutiveMatchingPieces();
  void update_pieces_coordinates();
  Response check_win();
  void winOrlose(const char *msg);

  void left_rotate_by_shapes(Shape shape);
  void distingush_shape_color(Node *node);
  void erase_node_distangush_lists(Node *node);

  void clear_tetriste();

  // void show_random_shape(int x, int y);
};
