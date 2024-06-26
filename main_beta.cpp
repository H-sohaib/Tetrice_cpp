#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <ctime>
// #include <vector>
#include "DynamicArray.hpp"
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
const int padding = 10;
const int WIN_WIDTH = 1000;
const int WIN_HEIGHT = 500;
const int SHAPES_LINE_Y = WIN_HEIGHT - 105;

enum class Response
{
  REMOVED_COLOR,
  REMOVED_SHAPE,
  LOSE,
  WIN,
  NONE
};
class Shape_Node
{
public:
  size_t index;
  GameObject *shape;
  Shape_Node *next;
  Shape_Node *prev;
  Shape_Node()
  {
    index = 0;
    shape = nullptr;
    next = nullptr;
  }

  static size_t get_max_index(List<Shape_Node *> &list)
  {
    size_t max = 0;
    for (Shape_Node *node : list)
    {
      if (node->index > max)
      {
        max = node->index;
      }
    }
    return max;
  }
};

// GameObject *next_piece;
List<Shape_Node *> pieces_list;

List<Shape_Node *> square_nodes;
List<Shape_Node *> circle_nodes;
List<Shape_Node *> triangle_nodes;
List<Shape_Node *> Rhambus_nodes;

List<Shape_Node *> blue_nodes;
List<Shape_Node *> yellow_nodes;
List<Shape_Node *> red_nodes;
List<Shape_Node *> green_nodes;

List<Shape_Node *> rand_pieces_list;

int game_init();
void handel_events();
void start_tetriste();
GameObject *return_random_shape(int x, int y);
void right_insert_piece2list();
void left_insert_piece2list();
void freeShapeNodeLists(List<Shape_Node *> &v);
void update_pieces_coordinates();
Response check_win();
void winOrlose(const char *msg);
void clear_tetriste();
void setup_static_UI();
void show_index(List<Shape_Node *> &v);
void distingush_shape_color(Shape_Node *node);
void left_rotate_by_shapes(Shape shape);
void erase_node_distangush_lists(Shape_Node *node);

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
} // * end main

void left_rotate_by_shapes(Shape shape)
{
  switch (shape)
  {
  case Shape::SQUARE:
    if (!square_nodes.size() > 1)
    {
      return;
    }

    square_nodes.left_rotate();
    // adjast the pieces_list with new order of shapes in shapes list
    for (size_t i = 0, j = 0; i < pieces_list.size(); i++)
    {
      if (pieces_list.at(i)->shape->shape == shape)
      {
        cout << "-square.size : " << square_nodes.size() << endl;
        for (Shape_Node *node : square_nodes)
        {
          cout << "square_nodes  " << node->index << " :" << static_cast<int>(node->shape->color) << endl;
        }

        cout << "i = " << i << " j = " << j << endl;
        cout << "i_shape = " << static_cast<int>(pieces_list.at(i)->shape->shape) << " i_color = " << static_cast<int>(pieces_list.at(i)->shape->color) << endl;
        cout << "j_shape = " << static_cast<int>(square_nodes.at(j)->shape->shape) << " j_color = " << static_cast<int>(square_nodes.at(j)->shape->color) << endl;
        pieces_list.at(i) = square_nodes.at(j);
        j++;
      }
    }
    break;

  default:
    break;
  }

  // adjast the x corrdinate of the pieces_list
  // update_pieces_coordinates();
}
void distingush_shape_color(Shape_Node *node)
{
  // by shapes
  switch (node->shape->shape)
  {
  case Shape::SQUARE:
    square_nodes.push_back(node);
    break;
  case Shape::CIRCLE:
    circle_nodes.push_back(node);
    break;
  case Shape::TRIANGLE:
    triangle_nodes.push_back(node);
    break;
  case Shape::RHAMBUS:
    Rhambus_nodes.push_back(node);
    break;
  }
  // By color
  switch (node->shape->color)
  {
  case Color::BLUE:
    blue_nodes.push_back(node);
    break;
  case Color::YELLOW:
    yellow_nodes.push_back(node);
    break;
  case Color::RED:
    red_nodes.push_back(node);
    break;
  case Color::GREEN:
    green_nodes.push_back(node);
    break;
  }
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
  new_node->index = Shape_Node::get_max_index(pieces_list) + 1;
  pieces_list.push_back(new_node);
  // correct the x of piece_list items
  update_pieces_coordinates();
  // insert to the distingush lists
  distingush_shape_color(new_node);
  show_index(pieces_list);

  // add the next rand piece
  next_rand_piece->shape = return_random_shape(padding + 120, 70); // push new random piece 2 rand
  // next_rand_piece->index = rand_pieces_list.size();
  rand_pieces_list.push_back(next_rand_piece); // push new random piece 2 randlist

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

  new_node->index = Shape_Node::get_max_index(pieces_list) + 1;
  // new_node->index = 0; // set index is the size of the list
  // shift all index by 1
  // for (Shape_Node *node : pieces_list)
  // {
  //   node->index++;
  // }

  pieces_list.push_front(new_node);
  update_pieces_coordinates(); // correct the x corrdinate using the their order
  // insert to the distingush lists
  distingush_shape_color(new_node);
  show_index(pieces_list);

  // add the next rand piece
  next_rand_piece->shape = return_random_shape(padding + 120, 70); // push new random piece 2 rand
  rand_pieces_list.push_back(next_rand_piece);                     // push new random piece 2 rand

  // remove the local pointers
  next_rand_piece = nullptr;
  new_node = nullptr;
}
void erase_node_distangush_lists(Shape_Node *node)
{
  // By shapes
  switch (node->shape->shape)
  {
  case Shape::SQUARE:
    square_nodes.remove_element(node);
    break;
  case Shape::CIRCLE:
    circle_nodes.remove_element(node);
    break;
  case Shape::TRIANGLE:
    triangle_nodes.remove_element(node);
    break;
  case Shape::RHAMBUS:
    Rhambus_nodes.remove_element(node);
    break;
  }
  // By color
  switch (node->shape->color)
  {
  case Color::BLUE:
    blue_nodes.remove_element(node);
    break;
  case Color::YELLOW:
    yellow_nodes.remove_element(node);
    break;
  case Color::RED:
    red_nodes.remove_element(node);
    break;
  case Color::GREEN:
    green_nodes.remove_element(node);
    break;
  }
}
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
      // erase node from distangush lists
      erase_node_distangush_lists(pieces_list[i]);
      erase_node_distangush_lists(pieces_list[i + 1]);
      erase_node_distangush_lists(pieces_list[i + 2]);

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
      // correct all indexs after erase
      // for (int i = 0; i < pieces_list.size(); i++)
      // {
      //   pieces_list.at(i)->index = i;
      // }

      update_pieces_coordinates();

      for (size_t i = 0; i < square_nodes.size(); i++)
      {
        cout << "square_nodes  " << square_nodes.at(i)->index << " :" << static_cast<int>(square_nodes.at(i)->shape->color) << endl;
      }

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

    case SDLK_DOWN:
      left_rotate_by_shapes(Shape::SQUARE);
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
  rand_piece->index = 0;
  pieces_list.push_back(rand_piece);
  distingush_shape_color(rand_piece);

  Shape_Node *rand = new Shape_Node;
  rand->shape = return_random_shape(padding + 120, 70);
  rand->index = 0;
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
void freeShapeNodeLists(List<Shape_Node *> &v)
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

void show_index(List<Shape_Node *> &v)
{
  for (Shape_Node *node : v)
  {
    cout << node->index << " ";
  }
  cout << endl;
}
