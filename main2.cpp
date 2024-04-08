#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <ctime>
#include <vector>
#include <cstdlib> // For rand() and srand()
#include "GameObject.hpp"
using namespace std;
// * const ------------------------------------
SDL_Event event;
SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font *font;
bool isRunning = false;
int padding = 10;
const int WIN_WIDTH = 1000;
const int WIN_HEIGHT = 500;
const int SHAPES_LINE_Y = WIN_HEIGHT - 105;
typedef struct Shape_Node
{
  GameObject *shape;
  Shape_Node *next;
  Shape_Node *prev;
} Shape_Node;

GameObject *next_piece;
std::vector<Shape_Node *> pieces_list;
std::vector<Shape_Node *> rand_pieces_list;

void handel_events();
void start_tetriste();
GameObject *return_random_shape(int x, int y);

int main()
{
  srand(static_cast<unsigned int>(time(0)));
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    std::cout << "windows not initilise ..." << std::endl;
    return -1;
  }
  if (TTF_Init() == -1)
  {
    std::cout << "SDL_ttf not initialise ..." << std::endl;
    return -1;
  }
  // Load a font
  font = TTF_OpenFont("./font/Oswald-Regular.ttf", 200); // Adjust the path and font size
  if (!font)
  {
    std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
    return -1;
  }
  window = SDL_CreateWindow("Tetrise",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            WIN_WIDTH, WIN_HEIGHT,
                            SDL_WINDOW_SHOWN);

  if (!window)
  {
    std::cout << "window not created ..." << std::endl;
    return -1;
  }
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer)
  {
    std::cout << "rendrer not created ..." << std::endl;
    return -1;
  }
  isRunning = true;
  // *- draw static elem
  SDL_Surface *textSurface_NP;
  SDL_Rect NextPieceR;
  NextPieceR.w = 80;
  NextPieceR.h = 80;
  NextPieceR.x = padding;
  NextPieceR.y = 30;
  // Before your main loop, create a texture for static elements
  SDL_Texture *staticElementsTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIN_WIDTH, WIN_HEIGHT);
  SDL_SetRenderTarget(renderer, staticElementsTexture);
  // * draw the pieces border
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  thickLineRGBA(renderer, 0 + padding, WIN_HEIGHT - 70, WIN_WIDTH - padding, WIN_HEIGHT - 70, 4, 0, 0, 0, 255);
  //  * draw next piece txt
  SDL_Color textColor = {0, 0, 0, 255};
  textSurface_NP = TTF_RenderText_Blended(font, "Next Piece :", textColor);
  // Create texture from surface pixels
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface_NP);
  SDL_RenderCopy(renderer, textTexture, NULL, &NextPieceR);
  // celan up
  delete textSurface_NP;

  SDL_SetRenderTarget(renderer, NULL);

  start_tetriste();
  while (isRunning)
  {
    // Event loop
    while (SDL_PollEvent(&event))
    { // While there are events in the queue
      handel_events();
    }
    // *render
    SDL_RenderCopy(renderer, staticElementsTexture, NULL, NULL);
    //* rander all piece in list
    for (Shape_Node *shapeNode : pieces_list)
    {
      shapeNode->shape->render();
    }

    for (Shape_Node *shapeNode : rand_pieces_list)
    {
      shapeNode->shape->render();
    }

    SDL_RenderPresent(renderer);

    // SDL_Delay(1000 / 60); // 60 fps
  }
  SDL_FreeSurface(textSurface_NP);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  return 0;
}

void insert_piece2list()
{
  pieces_list.push_back(rand_pieces_list.back()); // push the ran_piec from rand_list 2 piece_list
  Shape_Node *rand_piece = new Shape_Node;
  rand_piece->shape = return_random_shape(60, SHAPES_LINE_Y); // push new random piece 2 rand
  // rand_piece->prev =  ;
  // rand_piece->next = ;
  rand_pieces_list.push_back(return_random_shape(padding + 120, 70)); // push new random piece 2 rand
}

void handel_events()
{
  switch (event.type)
  {
  case SDL_QUIT:
    isRunning = false;
    break;
  case SDL_KEYDOWN: // Key has been pressed
    switch (event.key.keysym.sym)
    {
    case SDLK_ESCAPE: // Escape key
      isRunning = false;
      break;
    }
    break;
  default:
    break;
  }
}
void start_tetriste()
{
  Shape_Node *rand_piece = new Shape_Node;
  rand_piece->shape = return_random_shape(60, SHAPES_LINE_Y);
  pieces_list.push_back(rand_piece);
  Shape_Node *rand = new Shape_Node;
  rand->shape = return_random_shape(padding + 120, 70);
  rand_pieces_list.push_back(rand);
}

GameObject *return_random_shape(int x, int y)
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