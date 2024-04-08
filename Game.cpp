#include "Game.hpp"
#include <iostream>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <cstdlib> // For rand() and srand()

SDL_Rect *n_p_txt_src, *n_p_txt_dest;

Game::Game(const char *win_title, int width, int height)
{
  this->win_width = width;
  this->win_height = height;
  if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
  {
    std::cout << "windows initilise ..." << std::endl;
  }
  if (TTF_Init() == 0)
  {
    std::cout << "SDL_ttf initialise ..." << std::endl;
  }
  // Load a font
  font = TTF_OpenFont("./font/RedditMono-Bold.ttf", 24); // Adjust the path and font size
  if (!font)
  {
    std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
  }
  window = SDL_CreateWindow(win_title,
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            width, height,
                            SDL_WINDOW_SHOWN);

  if (window)
  {
    std::cout << "window created ..." << std::endl;
  }
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer)
  {
    std::cout << "rendrer created ..." << std::endl;
  }

  isRunning = true;
}

void Game::handel_events()
{
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type)
  {
  case SDL_QUIT:
    isRunning = false;
    break;
  default:
    break;
  }
}

void Game::render()
{
  int padding = 10;
  // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  thickLineRGBA(renderer, 0 + padding, win_height - 70, win_width - padding, win_height - 70, 4, 0, 0, 0, 255); // Example coordinates, thickness, and color
  //* rander all piece in list
  for (Shape_Node *shapeNode : shapes_list)
  {
    shapeNode->shape->render();
  }
  //
  SDL_Color textColor = {255, 255, 255, 255};
  // Render text surface
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Hello, SDL_ttf!", textColor);
  // Create texture from surface pixels
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

  SDL_RenderCopy(renderer, textTexture, NULL, n_p_txt_dest);
}

void Game::update()
{
  n_p_txt_dest->w = 100;
  n_p_txt_dest->h = 100;
}

void Game::clear()
{
  // delete next_piece;
  // delete n_p_txt_src;
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}

/**********************************************************************/

GameObject *Tetriste::return_random_shape(int x, int y)
{
  int shape = rand() % SHAPES_NUM;
  int color = rand() % COLOR_NUM;

  if (static_cast<Shape>(shape) == Shape::CIRCLE) // custom size
  {
    return new GameObject(renderer, static_cast<Shape>(shape), static_cast<Color>(color), SHAPES_SIZE / 2, SHAPES_SIZE / 2, x, y);
  }
  else if (static_cast<Shape>(shape) == Shape::SQUARE) // custom cordinnates
  {
    return new GameObject(renderer, static_cast<Shape>(shape), static_cast<Color>(color), SHAPES_SIZE, SHAPES_SIZE, x - (SHAPES_SIZE / 2), y - (SHAPES_SIZE / 2));
  }
  else // other shapes
  {
    return new GameObject(renderer, static_cast<Shape>(shape), static_cast<Color>(color), SHAPES_SIZE, SHAPES_SIZE, x, y);
  }
}
void Tetriste::show_random_shape(int x, int y)
{
  GameObject *shape = return_random_shape(x, y);
  shape->render();
  SDL_RenderPresent(shape->renderer);
}

void Tetriste::start_tetriste()
{
  Shape_Node *rand_piece = new Shape_Node;
  rand_piece->shape = return_random_shape(60, SHAPES_LINE_Y);
  shapes_list.push_back(rand_piece);
}