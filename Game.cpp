#include "Game.hpp"

// #include <SDL2/SDL2_gfxPrimitives.h>

// SDL_Rect *n_p_txt_src, *n_p_txt_dest;

// void Game::handel_events()
// {
//   SDL_Event event;
//   SDL_PollEvent(&event);
//   switch (event.type)
//   {
//   case SDL_QUIT:
//     isRunning = false;
//     break;
//   default:
//     break;
//   }
// }

// void Game::render()
// {
//   int padding = 10;
//   // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//   SDL_RenderClear(renderer);
//   SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//   thickLineRGBA(renderer, 0 + padding, win_height - 70, win_width - padding, win_height - 70, 4, 0, 0, 0, 255); // Example coordinates, thickness, and color
//   //* rander all piece in list
//   for (Shape_Node *shapeNode : shapes_list)
//   {
//     shapeNode->shape->render();
//   }
//   //
//   SDL_Color textColor = {255, 255, 255, 255};
//   // Render text surface
//   SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Hello, SDL_ttf!", textColor);
//   // Create texture from surface pixels
//   SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

//   SDL_RenderCopy(renderer, textTexture, NULL, n_p_txt_dest);
// }

// void Game::update()
// {
//   n_p_txt_dest->w = 100;
//   n_p_txt_dest->h = 100;
// }

/**********************************************************************/
int Tetriste::game_init()
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

void Tetriste::start_tetriste()
{
  pieces_list.push_back(return_random_shape(60, SHAPES_LINE_Y));

  rand_pieces_list.push_back(return_random_shape(padding + 120, 70));
}

void Tetriste::handel_events()
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
      insert2mainList(true);
      removeConsecutiveMatchingPieces();
      break;
    case SDLK_LEFT:
      insert2mainList(false);
      removeConsecutiveMatchingPieces();
      break;

    case SDLK_DOWN:
      // left_rotate_by_shapes(Shape::SQUARE);
      break;
    }
    break;
  default:
    break;
  }
}

Response Tetriste::removeConsecutiveMatchingPieces()
{
  for (int i = 0; i < pieces_list.get_size() - 2;)
  {
    if (pieces_list.get_size() < 3)
    {
      return Response::NONE;
    }
    // Notice the condition and increment change
    bool isSameShape = (pieces_list.at(i)->piece->shape == pieces_list.at(i + 1)->piece->shape) &&
                       (pieces_list.at(i)->piece->shape == pieces_list.at(i + 2)->piece->shape);
    bool isSameColor = (pieces_list.at(i)->piece->color == pieces_list.at(i + 1)->piece->color) &&
                       (pieces_list.at(i)->piece->color == pieces_list.at(i + 2)->piece->color);

    if (isSameShape || isSameColor)
    {
      // // erase node from distangush lists
      // erase_node_distangush_lists(pieces_list[i]);
      // erase_node_distangush_lists(pieces_list[i + 1]);
      // erase_node_distangush_lists(pieces_list[i + 2]);

      // Delete the GameObjects if dynamically allocated
      // delete pieces_list[i]->shape;
      // delete pieces_list[i + 1]->shape;
      // delete pieces_list[i + 2]->shape;
      // // Delete the Nodes
      // delete pieces_list[i];
      // delete pieces_list[i + 1];
      // delete pieces_list[i + 2];
      // Erase elements: erase three consecutive elements starting from index i
      pieces_list.remove_range(i, i + 2);
      // pieces_list.remove_element(pieces_list.at(i));
      // pieces_list.remove_element(pieces_list.at(i + 1));
      // pieces_list.remove_element(pieces_list.at(i + 2));

      // pieces_list.erase(pieces_list.begin() + i, pieces_list.begin() + i + 3);
      // correct all indexs after erase
      // for (int i = 0; i < pieces_list.get_size(); i++)
      // {
      //   pieces_list.at(i)->index = i;
      // }

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

  return Response::NONE;
}

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

GameObject *Tetriste::copyShapeNode(Node *OriginNode)
{
  Node *new_node = new Node;

  // * copy the shape node from ran list to new node pointer & insert it to pieces_list
  new_node->piece = new GameObject(*(OriginNode->piece));
  // return new_node;
  return new GameObject(*(OriginNode->piece));
}

void Tetriste::insert2mainList(bool isRight)
{
  // int PL_size = pieces_list.get_size();
  // Node *new_node = copyShapeNode(rand_pieces_list.back());

  // std::cout << "piece_List befor I : ";
  // for (int i = 0; i < pieces_list.get_size(); i++)
  // {
  //   std::cout << static_cast<int>(pieces_list.at(i)->shape->color) << " , ";
  // }

  if (isRight)
  {
    pieces_list.push_back(copyShapeNode(rand_pieces_list.back()));
  }
  else
  {
    pieces_list.push_front(copyShapeNode(rand_pieces_list.back()));
  }

  // std::cout << "piece_List after I : ";
  // for (int i = 0; i < pieces_list.get_size(); i++)
  // {
  //   std::cout << static_cast<int>(pieces_list.at(i)->shape->color) << " , ";
  // }

  // correct the x of piece_list items
  update_pieces_coordinates();
  // insert to the distingush lists
  // distingush_shape_color(new_node);
  // show_index(pieces_list);

  // add the next rand piece
  // Node *next_rand_piece = new Node;
  // next_rand_piece->piece = return_random_shape(padding + 120, 70); // push new random piece 2 rand
  // next_rand_piece->index = rand_pieces_list.get_size();
  rand_pieces_list.push_back(return_random_shape(padding + 120, 70)); // push new random piece 2 randlist

  // remove the local pointers
  // next_rand_piece = nullptr;
  // new_node = nullptr;
}

void Tetriste::update_pieces_coordinates()
{
  for (int i = 0; i < pieces_list.get_size(); i++)
  {
    pieces_list.at(i)->piece->update_coordinates(60 + i * 60, SHAPES_LINE_Y);
  }
}

Response Tetriste::check_win()
{
  if (pieces_list.get_size() == 0)
  {
    return Response::WIN;
  }
  else if (pieces_list.get_size() > 14)
  {
    return Response::LOSE;
  }
  return Response::NONE;
}

void Tetriste::winOrlose(const char *msg)
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

void Tetriste::setup_static_UI()
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

void Tetriste::left_rotate_by_shapes(Shape shape)
{
  switch (shape)
  {
  case Shape::SQUARE:
    if (!square_nodes.get_size() > 1)
    {
      return;
    }

    square_nodes.left_rotate();
    // adjast the pieces_list with new order of shapes in shapes list
    for (size_t i = 0, j = 0; i < pieces_list.get_size(); i++)
    {
      if (pieces_list.at(i)->piece->shape == shape)
      {
        std::cout << "-square.size : " << square_nodes.get_size() << std::endl;
        for (size_t i = 0; i < square_nodes.get_size(); i++)
        {
          std::cout << "square_nodes  " << square_nodes.at(i)->index << " :" << static_cast<int>(square_nodes.at(i)->piece->color) << std::endl;
        }

        std::cout << "i = " << i << " j = " << j << std::endl;
        std::cout << "i_shape = " << static_cast<int>(pieces_list.at(i)->piece->shape) << " i_color = " << static_cast<int>(pieces_list.at(i)->piece->color) << std::endl;
        std::cout << "j_shape = " << static_cast<int>(square_nodes.at(j)->piece->shape) << " j_color = " << static_cast<int>(square_nodes.at(j)->piece->color) << std::endl;
        *(pieces_list.at(i)) = *(square_nodes.at(j));
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

void Tetriste::clear_tetriste()
{
  // freeShapeNodeLists(pieces_list);
  // freeShapeNodeLists(rand_pieces_list);
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

// void Tetriste::distingush_shape_color(Node *node)
// {
//   // by shapes
//   switch (node->piece->shape)
//   {
//   case Shape::SQUARE:
//     square_nodes.push_back(node);
//     break;
//   case Shape::CIRCLE:
//     circle_nodes.push_back(node);
//     break;
//   case Shape::TRIANGLE:
//     triangle_nodes.push_back(node);
//     break;
//   case Shape::RHAMBUS:
//     Rhambus_nodes.push_back(node);
//     break;
//   }
//   // By color
//   switch (node->piece->color)
//   {
//   case Color::BLUE:
//     blue_nodes.push_back(node);
//     break;
//   case Color::YELLOW:
//     yellow_nodes.push_back(node);
//     break;
//   case Color::RED:
//     red_nodes.push_back(node);
//     break;
//   case Color::GREEN:
//     green_nodes.push_back(node);
//     break;
//   }
// }

// void Tetriste::erase_node_distangush_lists(Node *node)
// {
//   // By shapes
//   switch (node->piece->shape)
//   {
//   case Shape::SQUARE:
//     square_nodes.remove_element(node);
//     break;
//   case Shape::CIRCLE:
//     circle_nodes.remove_element(node);
//     break;
//   case Shape::TRIANGLE:
//     triangle_nodes.remove_element(node);
//     break;
//   case Shape::RHAMBUS:
//     Rhambus_nodes.remove_element(node);
//     break;
//   }
//   // By color
//   switch (node->piece->color)
//   {
//   case Color::BLUE:
//     blue_nodes.remove_element(node);
//     break;
//   case Color::YELLOW:
//     yellow_nodes.remove_element(node);
//     break;
//   case Color::RED:
//     red_nodes.remove_element(node);
//     break;
//   case Color::GREEN:
//     green_nodes.remove_element(node);
//     break;
//   }
// }

// void freeShapeNodeLists(SimpleLinkedList<Node *> &v)
// {
//   for (size_t i = 0; i < v.get_size(); i++)
//   {
//     delete v.at(i)->shape;
//     delete v.at(i);
//   }
//   v.clear();
// }