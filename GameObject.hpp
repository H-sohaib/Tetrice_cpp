#pragma once
#include <SDL2/SDL2_gfxPrimitives.h>
const short int SHAPES_NUM = 4;
const short int COLOR_NUM = 4;
const short int SHAPES_SIZE = 50;

enum class Color
{
  BLUE,   // 0
  YELLOW, // 1
  RED,    // 2
  GREEN   // 3
};
enum class Shape
{
  SQUARE,  // 0
  RHAMBUS, // 1
  CIRCLE,  // 2
  TRIANGLE // 3
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
  GameObject(GameObject &other)
      : renderer(other.renderer), // Shared renderer pointer, not a deep copy.
        shape(other.shape),       // Copy shape (assuming it's a simple data type or enum)
        color(other.color),       // Copy color (assuming it's a simple data type or enum)
        weight(other.weight),     // Copy width
        height(other.height),     // Copy height
        x(other.x),               // Copy x position
        y(other.y)                // Copy y position
  {
  }
  // ~GameObject();
  void drawSquare(SDL_Renderer *renderer, Color color, int w, int h, int x, int y);
  void drawRhambus(SDL_Renderer *renderer, Color color, int w, int h, int x, int y);
  void drawCircle(SDL_Renderer *renderer, Color color, int w, int h, int x, int y);
  void drawTriangle(SDL_Renderer *renderer, Color color, int w, int h, int x, int y);
  void update_coordinates(int x, int y);
  void render();
};