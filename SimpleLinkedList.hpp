
#pragma once
#include <cassert>
#include <iostream>
// using namespace std;
#include "GameObject.hpp"

class Node
{
public:
  int index;
  GameObject *piece;
  Node *next;
  Node(GameObject *piece, int index, Node *next = nullptr) : piece(piece), index(index), next(next) {}
  Node()
  {
    next = nullptr;
    piece = nullptr;
  }
};

class SimpleLinkedList
{
private:
  Node *head; // point to last node
  Node *tail;
  int size;

public:
  SimpleLinkedList() : head(nullptr), size(0){};
  ~SimpleLinkedList()
  {
    // clear();
  }
  int get_size() const { return size; }
  Node *at(int index);
  void push_back(GameObject *piece);
  Node *back();
  Node *front();
  void push_front(GameObject *piece);
  Node *operator[](int index);
  void insert(int pos, GameObject *piece);
  void remove_element(Node *node);
  void remove_range(int start, int end);
  void clear();
  Node *begin();
  Node *end();
  void right_rotate();
  void left_rotate();
  void test_print()
  {
    Node *current = head;
    while (current != nullptr)
    {
      std::cout << current->index << " , ";
      current = current->next;
    }
    std::cout << std::endl;
  }

  static int get_max_index(SimpleLinkedList &list)
  {
    int max = 0;
    for (int i = 0; i < list.get_size(); i++)
    {
      if (list.at(i)->index > max)
      {
        max = list.at(i)->index;
      }
    }
    return max;
  }
  // void erase(T pos);
  // T erase(T first, T last);
};
