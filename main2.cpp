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
class Shape_Node
{
public:
  Shape_Node()
  {
    shape = nullptr;
    next = nullptr;
    prev = nullptr;
  }
  GameObject *shape;
  Shape_Node *next;
  Shape_Node *prev;
};

// GameObject *next_piece;
std::vector<Shape_Node *> pieces_list;
std::vector<Shape_Node *> rand_pieces_list;

int game_init();
void handel_events();
void start_tetriste();
GameObject *return_random_shape(int x, int y);
void right_insert_piece2list();
void left_insert_piece2list();
void freeShapeNodeLists(vector<Shape_Node *> &v);
void update_pieces_coordinates();

int main()
{
  if (game_init() == -1)
  {
    return -1;
  }

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

  SDL_SetRenderTarget(renderer, NULL);

  start_tetriste();
  while (isRunning)
  {
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

    rand_pieces_list.back()->shape->render();

    SDL_RenderPresent(renderer);

    // SDL_Delay(1000 / 60); // 60 fps
  }

  freeShapeNodeLists(pieces_list);
  freeShapeNodeLists(rand_pieces_list);
  TTF_CloseFont(font);
  SDL_FreeSurface(textSurface_NP);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  font = nullptr;
  window = nullptr;
  renderer = nullptr;
  SDL_Quit();
  return 0;
}

void update_pieces_coordinates()
{
  for (int i = 0; i < pieces_list.size(); i++)
  {
    pieces_list.at(i)->shape->update_coordinates(60 + i * 60, SHAPES_LINE_Y);
  }
}

Shape_Node *copyShapeNode(Shape_Node *OriginNode)
{
  Shape_Node *new_node = new Shape_Node;
  // * copy the shape node from ran list to new node pointer & insert it to pieces_list
  new_node->shape = new GameObject(*(OriginNode->shape));
  new_node->next = nullptr;
  new_node->prev = nullptr;
  return new_node;
}

void right_insert_piece2list()
{
  int PL_size = pieces_list.size();
  Shape_Node *next_rand_piece = new Shape_Node;
  Shape_Node *new_node = copyShapeNode(rand_pieces_list.back());
  //  link the node
  pieces_list.back()->next = new_node; // link the last elem to the new added elem
  new_node->prev = pieces_list.back();

  pieces_list.push_back(new_node);
  // correct the x of piece_list items
  update_pieces_coordinates();

  // add the next rand piece
  next_rand_piece->shape = return_random_shape(padding + 120, 70); // push new random piece 2 rand
  rand_pieces_list.push_back(next_rand_piece);                     // push new random piece 2 rand

  // remove the local pointers
  next_rand_piece = nullptr;
  new_node = nullptr;
}

void left_insert_piece2list()
{
  int PL_size = pieces_list.size();
  Shape_Node *next_rand_piece = new Shape_Node;
  Shape_Node *new_node = copyShapeNode(rand_pieces_list.back());

  // link the node
  pieces_list.front()->prev = new_node;
  new_node->next = pieces_list.front();

  pieces_list.insert(pieces_list.begin(), new_node);

  update_pieces_coordinates();

  // add the next rand piece
  next_rand_piece->shape = return_random_shape(padding + 120, 70); // push new random piece 2 rand
  rand_pieces_list.push_back(next_rand_piece);                     // push new random piece 2 rand

  // remove the local pointers
  next_rand_piece = nullptr;
  new_node = nullptr;
}

/*
 * u need to stop tye insertion if the pieces_list is empty
 * and say you WIN !!!!
 */
int removeConsecutiveMatchingPieces()
{
  int removedCount = 0;
  for (int i = 0; i < pieces_list.size() - 2;)
  {
    if (pieces_list.size() < 3)
    {
      return 0;
    }
    // Notice the condition and increment change
    bool isSameShape = (pieces_list[i]->shape->shape == pieces_list[i + 1]->shape->shape) &&
                       (pieces_list[i]->shape->shape == pieces_list[i + 2]->shape->shape);
    bool isSameColor = (pieces_list[i]->shape->color == pieces_list[i + 1]->shape->color) &&
                       (pieces_list[i]->shape->color == pieces_list[i + 2]->shape->color);

    if (isSameShape || isSameColor)
    {
      // Delete the GameObjects if dynamically allocated
      delete pieces_list[i]->shape;
      delete pieces_list[i + 1]->shape;
      delete pieces_list[i + 2]->shape;

      // Delete the Shape_Nodes
      delete pieces_list[i];
      delete pieces_list[i + 1];
      delete pieces_list[i + 2];

      // Erase elements: erase three consecutive elements starting from index i
      pieces_list.erase(pieces_list.begin() + i, pieces_list.begin() + i + 3);

      update_pieces_coordinates();

      // Increment removedCount
      removedCount += 3;

      // No increment here, check new 'i' against next elements
    }
    else
    {
      // Increment if no removal, to continue checking next potential sequence
      i++;
    }
  }

  // if (removedCount > 0)
  // {
  //   // Optional: Render remaining pieces after modification
  //   for (Shape_Node *shapeNode : pieces_list)
  //   {
  //     shapeNode->shape->render();
  //   }

  //   // If you maintain another list and use its back item to render
  //   if (!rand_pieces_list.empty())
  //   {
  //     rand_pieces_list.back()->shape->render();
  //   }

  //   SDL_RenderPresent(renderer);
  // }

  return removedCount; // Return count of removed elements
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
    case SDLK_RIGHT:
      right_insert_piece2list();
      removeConsecutiveMatchingPieces();
      break;
    case SDLK_LEFT:
      left_insert_piece2list();
      removeConsecutiveMatchingPieces();
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

int game_init()
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
  return 1;
}

void freeShapeNodeLists(vector<Shape_Node *> &v)
{
  for (Shape_Node *node : v)
  {
    delete node->shape;
    delete node;
  }
  v.clear();
}