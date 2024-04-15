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
SDL_Texture *staticElementsTexture;
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
enum class Response
{
  REMOVED_COLOR,
  REMOVED_SHAPE,
  LOSE,
  WIN,
  NONE
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
Response check_win();
void winOrlose(const char *msg);
void clear_tetriste();
void setup_static_UI();

int main()
{
  if (game_init() == -1)
  {
    return -1;
  }

  setup_static_UI();
  start_tetriste();

    while (isRunning)
  {
    if (check_win() == Response::WIN)
    {
      cout << "You Win" << endl;
      winOrlose("You Win !!!");
      isRunning = false;
    }
    else if (check_win() == Response::LOSE)
    {
      cout << "You Lose" << endl;
      winOrlose("You Lose !!!");
      isRunning = false;
    }
    while (SDL_PollEvent(&event))
    { // While there are events in the queue
      handel_events();
    }
    // *render
    // Render static elements texture
    if (staticElementsTexture)
    {
      SDL_RenderCopy(renderer, staticElementsTexture, NULL, NULL); // Draw the texture to the screen
    }
    //* rander all piece in list

    for (Shape_Node *shapeNode : pieces_list)
    {
      shapeNode->shape->render();
    }

    rand_pieces_list.back()->shape->render();

    SDL_RenderPresent(renderer);

    // SDL_Delay(1000 / 60); // 60 fps
  }

  clear_tetriste();
  return 0;
}

void setup_static_UI()
{
  SDL_Rect NextPieceR = {padding, 30, 80, 80};

  // Create a texture for static elements
  staticElementsTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIN_WIDTH, WIN_HEIGHT);
  if (!staticElementsTexture)
  {
    SDL_Log("Failed to create texture: %s", SDL_GetError());
    return;
  }

  // Set rendering target to the static texture
  SDL_SetRenderTarget(renderer, staticElementsTexture);

  // Clear with white background
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  // Draw border line
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  thickLineRGBA(renderer, padding, WIN_HEIGHT - 70, WIN_WIDTH - padding, WIN_HEIGHT - 70, 4, 0, 0, 0, 255);

  // Render "Next Piece" text
  SDL_Color textColor = {0, 0, 0, 255};
  SDL_Surface *textSurface = TTF_RenderText_Blended(font, "Next Piece :", textColor);
  if (!textSurface)
  {
    SDL_Log("Failed to create text surface: %s", TTF_GetError());
    SDL_SetRenderTarget(renderer, NULL); // Reset the render target before returning
    return;
  }

  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
  if (!textTexture)
  {
    SDL_Log("Failed to create text texture: %s", SDL_GetError());
    SDL_FreeSurface(textSurface);        // Free the surface before returning
    SDL_SetRenderTarget(renderer, NULL); // Reset the render target before returning
    return;
  }

  SDL_RenderCopy(renderer, textTexture, NULL, &NextPieceR);

  // Reset rendering target to default
  SDL_SetRenderTarget(renderer, NULL);

  // Clean up
  SDL_FreeSurface(textSurface);
  SDL_DestroyTexture(textTexture); // Ensure texture is destroyed after use
}
void winOrlose(const char *msg)
{
  SDL_Surface *textSurface;
  SDL_Rect textRect;
  textRect.w = 200;
  textRect.h = 100;
  textRect.x = WIN_WIDTH / 2 - textRect.w / 2;
  textRect.y = WIN_HEIGHT / 2 - textRect.h / 2;
  SDL_Color textColor = {0, 0, 0, 255};
  textSurface = TTF_RenderText_Blended(font, msg, textColor);
  // Create texture from surface pixels
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
  SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
  SDL_RenderPresent(renderer);
  SDL_Delay(2000);
  SDL_FreeSurface(textSurface);
  SDL_DestroyTexture(textTexture);
}
Response check_win()
{
  if (pieces_list.size() == 0)
  {
    return Response::WIN;
  }
  else if (pieces_list.size() > 14)
  {
    return Response::LOSE;
  }
  return Response::NONE;
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
Response removeConsecutiveMatchingPieces()
{
  for (int i = 0; i < pieces_list.size() - 2;)
  {
    if (pieces_list.size() < 3)
    {
      return Response::NONE;
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

      if (isSameColor)
        return Response::REMOVED_COLOR;
      else if (isSameShape)
        return Response::REMOVED_SHAPE;
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

  return Response::NONE;
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
void clear_tetriste()
{
  freeShapeNodeLists(pieces_list);
  freeShapeNodeLists(rand_pieces_list);
  TTF_CloseFont(font);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyTexture(staticElementsTexture);
  // SDL_DestroyTexture(textTexture);
  // textTexture = nullptr;
  staticElementsTexture = nullptr;
  font = nullptr;
  window = nullptr;
  renderer = nullptr;
  SDL_Quit();
}