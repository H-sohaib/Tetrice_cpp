#include "SimpleLinkedList.hpp"
#include <iostream>
using namespace std;
// typedef struct Node
// {
//   int index;
//   Node *next;
//   Node(int index, Node *next = nullptr)
//   {
//     index = index;
//     next = next;
//   }
// } Node;
int main()
{
  SimpleLinkedList<Node *> list;

  Node *node1 = new Node(1);
  Node *node2 = new Node(2);
  Node *node3 = new Node(3);
  Node *node4 = new Node(8);
  Node *node5 = new Node(7);

  list.push_back(node1);
  list.push_back(node2);
  list.push_back(node3);
  list.push_back(new Node(7));
  list.push_back(node4);
  list.push_back(node5);

  cout << "Size: " << list.get_size() << endl;

  // list.left_rotate();
  // list.right_rotate();

  list.test_print();

  list.remove_range(0, 3);

  cout << "Size: " << list.get_size() << endl;
  list.test_print();
}
