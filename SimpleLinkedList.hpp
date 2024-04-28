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

Node *SimpleLinkedList::at(int index)
{
  assert(index < size);
  Node *current = head;
  for (int i = 0; i < index; i++)
  {
    current = current->next;
  }
  return current;
}

void SimpleLinkedList::push_back(GameObject *piece)
{
  Node *node = new Node(piece, size);

  if (size == 0)
  {
    head = node;
    tail = node;
  }
  else
  {
    tail->next = node;
    tail = node;
  }
  ++size;
}
// template <typename T>
Node *SimpleLinkedList::back()
{
  return tail;
}

// template <typename T>
Node *SimpleLinkedList::front()
{
  return head;
}

// template <typename T>
void SimpleLinkedList::push_front(GameObject *piece)
{
  Node *node = new Node(piece, size);

  if (size == 0)
  {
    head = node;
    tail = node;
  }
  else
  {
    node->next = head;
    head = node;
  }
  ++size;
}

// template <typename T>
Node *SimpleLinkedList::operator[](int index)
{
  return at(index);
}

// template <typename T>
void SimpleLinkedList::insert(int pos, GameObject *piece)
{

  assert(pos <= size);

  if (pos == 0)
  {
    push_front(piece);
  }
  else if (pos == size)
  {
    push_back(piece);
  }
  else
  {
    Node *node = new Node(piece, size);

    Node *current = head;
    for (int i = 0; i < pos - 1; i++)
    {
      current = current->next;
    }
    node->next = current->next;
    current->next = node;
    ++size;
  }
}

// template <typename T>
void SimpleLinkedList::remove_element(Node *node)
{
  if (node == head)
  {
    head = head->next;
    delete node;
    --size;
    return;
  }
  Node *current = head;
  while (current->next != nullptr)
  {
    if (current->next == node)
    {
      current->next = node->next;
      delete node;
      --size;
      return;
    }
    current = current->next;
  }
}
// template <typename T>
void SimpleLinkedList::clear()
{
  Node *current = head;
  while (current != nullptr)
  {
    Node *next = current->next;
    delete current;
    current = next;
  }
  head = nullptr;
  tail = nullptr;
  size = 0;
}

// template <typename T>
Node *SimpleLinkedList::begin()
{
  return head;
}

// template <typename T>
Node *SimpleLinkedList::end()
{
  return tail;
}

// template <typename T>
void SimpleLinkedList::right_rotate()
{
  if (size <= 1)
  {
    return;
  }
  Node *current = head;
  while (current->next != tail)
  {
    current = current->next;
  }
  tail->next = head;
  head = tail;
  tail = current;
  tail->next = nullptr;
}

// template <typename T>
void SimpleLinkedList::left_rotate()
{
  if (size <= 1)
  {
    return;
  }
  tail->next = head;
  head = head->next;
  tail = tail->next;
  tail->next = nullptr;
}

// template <typename T>
void SimpleLinkedList::remove_range(int start, int end)
{
  assert(start <= end);
  assert(end < size);

  if (size == 0)
    return;

  Node *current = head;
  Node *prev = nullptr;

  for (int i = 0; i < start; i++)
  {
    prev = current;
    current = current->next;
  }

  Node *next = nullptr;
  for (int i = start; i <= end; i++)
  {
    next = current->next;
    delete current->piece;
    delete current;
    current = next;
    --size;
  }

  if (prev)
  {
    prev->next = current;
  }
  else
  {
    head = current;
  }

  if (current == nullptr)
  {
    tail = prev;
  }
}

// template <typename T>
// void SimpleLinkedList<T><T>::erase(T pos)
// {
//   if (pos == head)
//   {
//     head = head->next;
//     delete pos;
//     --size;
//     return;
//   }
//   T current = head;
//   while (current->next != nullptr)
//   {
//     if (current->next == pos)
//     {
//       current->next = pos->next;
//       delete pos;
//       --size;
//       return;
//     }
//     current = current->next;
//   }
// }

// template <typename T>
// T SimpleLinkedList<T>::erase(T first, T last)
// {
//   bool lastFound = false;
//   T current = head;
//   while (current->next != nullptr)
//   {
//     if (current == last)
//     {
//       lastFound = true;
//       break;
//     }
//     current = current->next;
//   }
//   while (current->next != nullptr && lastFound)
//   {
//     if (current->next == first)
//     {
//       current->next = last->next;
//       while (first != last)
//       {
//         T temp = first;
//         first = first->next;
//         delete temp;
//         --size;
//       }
//       delete last;
//       --size;
//       return current->next;
//     }
//     current = current->next;
//   }
//   return nullptr;
// }
