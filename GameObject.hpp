#pragma once
#include <SDL2/SDL2_gfxPrimitives.h>
const short int SHAPES_NUM = 4;
const short int COLOR_NUM = 4;
const short int SHAPES_SIZE = 50;

enum class Color
{
  BLUE,
  YELLOW,
  RED,
  GREEN
};
enum class Shape
{
  SQUARE,
  RHAMBUS,
  CIRCLE,
  TRIANGLE
};

class GameObject
{

public:
  int x, y;
  int weight, height;
  SDL_Renderer *renderer;
  Color color;
  Shape shape;

  GameObject(SDL_Renderer *renderer, Shape shape, Color color, int w, int h, int x, int y);
  // ~GameObject();
  void drawSquare(SDL_Renderer *renderer, Color color, int w, int h, int x, int y);
  void drawRhambus(SDL_Renderer *renderer, Color color, int w, int h, int x, int y);
  void drawCircle(SDL_Renderer *renderer, Color color, int w, int h, int x, int y);
  void drawTriangle(SDL_Renderer *renderer, Color color, int w, int h, int x, int y);
  void update();
  void render();
};