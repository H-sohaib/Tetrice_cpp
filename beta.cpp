#include <iostream>
#include <memory>
#include "Game.hpp"
using namespace std;

int main()
{
  Tetriste *game = new Tetriste("game", WIN_WIDTH, WIN_HEIGHT);

  GameObject *square = new GameObject(game->renderer,
                                      Shape::SQUARE, Color::BLUE,
                                      SHAPES_SIZE, SHAPES_SIZE, 30 - (SHAPES_SIZE / 2), SHAPES_LINE_Y - (SHAPES_SIZE / 2));
  GameObject *tri = new GameObject(game->renderer,
                                   Shape::TRIANGLE, Color::GREEN, SHAPES_SIZE, SHAPES_SIZE, 100, SHAPES_LINE_Y);

  GameObject *cercle = new GameObject(game->renderer,
                                      Shape::CIRCLE, Color::YELLOW, SHAPES_SIZE / 2, SHAPES_SIZE / 2, 160, SHAPES_LINE_Y);

  GameObject *rhambus = new GameObject(game->renderer,
                                       Shape::RHAMBUS, Color::RED, SHAPES_SIZE, SHAPES_SIZE, 240, SHAPES_LINE_Y);
  // Show The first random shape in Game
  // game->show_random_shape(60, SHAPES_LINE_Y);
  game->start_tetriste(); //*insert rand pice to array and let game render rander the list
  // add the ran shape in a list and continuassly show them in Game loop

  while (game->isRunning) // check if the game is running
  {
    // hundel events
    game->handel_events();
    // update all object , x, y ect
    game->update();
    // render changes
    game->render();

    SDL_RenderPresent(game->renderer);
  }
  // clean win
  game->clear();

  return 0;
}