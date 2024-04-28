#include <iostream>
#include <SDL2/SDL.h>
// #include <SDL2/SDL2_gfxPrimitives.h>
// #include <SDL2/SDL_ttf.h>
// #include <ctime>
// #include <cstdlib> // For rand() and srand()
// #include "SimpleLinkedList.hpp"
#include "GameObject.hpp"
#include "Game.hpp"
using namespace std;

Tetriste tertiste;

int main()
{

  if (tertiste.game_init() == -1)
  {
    return -1;
  }

  tertiste.setup_static_UI();
  tertiste.start_tetriste();

  while (tertiste.isRunning)
  {
    if (tertiste.check_win() == Response::WIN)
    {
      std::cout << "You Win" << endl;
      tertiste.winOrlose("You Win !!!");
      tertiste.isRunning = false;
    }
    else if (tertiste.check_win() == Response::LOSE)
    {
      std::cout << "You Lose" << endl;
      tertiste.winOrlose("You Lose !!!");
      tertiste.isRunning = false;
    }
    while (SDL_PollEvent(&tertiste.event))
    { // While there are events in the queue
      tertiste.handel_events();
    }
    // *render
    // Render static elements texture
    if (tertiste.staticElementsTexture)
    {
      SDL_RenderCopy(tertiste.renderer, tertiste.staticElementsTexture, NULL, NULL); // Draw the texture to the screen
    }
    //* rander all piece in list

    for (int i = 0; i < tertiste.pieces_list.get_size(); i++)
    {
      tertiste.pieces_list.at(i)->piece->render();
    }
    // {
    //   shapeNode->shape->render();
    // }

    tertiste.rand_pieces_list.back()->piece->render();

    SDL_RenderPresent(tertiste.renderer);

    // SDL_Delay(1000 / 60); // 60 fps
  }

  tertiste.clear_tetriste();
  return 0;
} // * end main

// GameObject *next_piece;

// int game_init();
// void handel_events();
// void start_tetriste();
// void clear_tetriste();
// void setup_static_UI();
// GameObject *return_random_shape(int x, int y);
// void right_insert_piece2list();
// void left_insert_piece2list();
// void update_pieces_coordinates();
// Response check_win();
// void winOrlose(const char *msg);
// void distingush_shape_color(Node *node);
// void left_rotate_by_shapes(Shape shape);
// void erase_node_distangush_lists(Node *node);

// void show_index(SimpleLinkedList<Node *> &v);
// void freeShapeNodeLists(SimpleLinkedList<Node *> &v);
