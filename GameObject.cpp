#include "GameObject.hpp"

GameObject::GameObject(SDL_Renderer *renderer, Shape shape, Color color, int w, int h, int x, int y)
{
  this->renderer = renderer;
  this->shape = shape;
  this->color = color;
  this->weight = w;
  this->height = h;
  this->x = x;
  this->y = y;
}

void GameObject::drawSquare(SDL_Renderer *renderer, Color color, int w, int h, int x, int y)
{
  switch (color)
  {
  case Color::BLUE:
    boxRGBA(renderer, x, y, x + w, y + h, 0, 0, 255, 255);
    break;
  case Color::YELLOW:
    boxRGBA(renderer, x, y, x + w, y + h, 255, 255, 0, 255);
    break;
  case Color::RED:
    boxRGBA(renderer, x, y, x + w, y + h, 255, 0, 0, 255);
    break;
  case Color::GREEN:
    boxRGBA(renderer, x, y, x + w, y + h, 0, 255, 0, 255);
    break;

  default:
    break;
  }
}

void GameObject::drawTriangle(SDL_Renderer *renderer, Color color, int w, int h, int x, int y)
{
  Sint16 x1 = x - w / 2;
  Sint16 y1 = y + h / 2;
  Sint16 x2 = x + w / 2;
  Sint16 y2 = y + h / 2;
  Sint16 x3 = x;
  Sint16 y3 = y - h / 2;
  switch (color)
  {
  case Color::BLUE:
    filledTrigonRGBA(renderer, x1, y1, x2, y2, x3, y3, 0, 0, 255, 255);
    break;
  case Color::YELLOW:
    filledTrigonRGBA(renderer, x1, y1, x2, y2, x3, y3, 255, 255, 0, 255);
    break;
  case Color::RED:
    filledTrigonRGBA(renderer, x1, y1, x2, y2, x3, y3, 255, 0, 0, 255);
    break;
  case Color::GREEN:
    filledTrigonRGBA(renderer, x1, y1, x2, y2, x3, y3, 0, 255, 0, 255);
    break;

  default:
    break;
  }
}

void GameObject::drawCircle(SDL_Renderer *renderer, Color color, int w, int h, int x, int y)
{
  switch (color)
  {
  case Color::BLUE:
    filledEllipseRGBA(renderer, x, y, w, h, 0, 0, 255, 255);
    break;
  case Color::YELLOW:
    filledEllipseRGBA(renderer, x, y, w, h, 255, 255, 0, 255);
    break;
  case Color::RED:
    filledEllipseRGBA(renderer, x, y, w, h, 255, 0, 0, 255);
    break;
  case Color::GREEN:
    filledEllipseRGBA(renderer, x, y, w, h, 0, 255, 0, 255);
    break;

  default:
    break;
  }
}

void GameObject::drawRhambus(SDL_Renderer *renderer, Color color, int w, int h, int x, int y)
{
  Sint16 vx[4], vy[4];

  // Calculate the vertices of the rhombus based on width (w), height (h), and center position (x, y)
  vx[0] = x;
  vy[0] = y - h / 2; // Top
  vx[1] = x + w / 2;
  vy[1] = y; // Right
  vx[2] = x;
  vy[2] = y + h / 2; // Bottom
  vx[3] = x - w / 2;
  vy[3] = y; // Left
  switch (color)
  {
  case Color::BLUE:
    filledPolygonRGBA(renderer, vx, vy, 4, 0, 0, 255, 255);
    break;
  case Color::YELLOW:
    filledPolygonRGBA(renderer, vx, vy, 4, 255, 255, 0, 255);
    break;
  case Color::RED:
    filledPolygonRGBA(renderer, vx, vy, 4, 255, 0, 0, 255);
    break;
  case Color::GREEN:
    filledPolygonRGBA(renderer, vx, vy, 4, 0, 255, 0, 255);
    break;

  default:
    break;
  }
}

void GameObject::render()
{
  switch (shape)
  {
  case Shape::SQUARE:
    drawSquare(renderer, color, weight, height, x, y);
    break;
  case Shape::TRIANGLE:
    drawTriangle(renderer, color, weight, height, x, y);
    break;
  case Shape::CIRCLE:
    drawCircle(renderer, color, weight, height, x, y);
    break;
  case Shape::RHAMBUS:
    drawRhambus(renderer, color, weight, height, x, y);
    break;
  default:
    break;
  }
}