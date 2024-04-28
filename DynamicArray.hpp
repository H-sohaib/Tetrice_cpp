#include <cstdlib>
#include <cassert>
#include <utility>
#include <algorithm> // For std::move
// #include <iterator>  // For std::distance

template <typename T>
class DynamicArray
{
private:
  T *array;
  size_t m_size;
  size_t m_capacity;

  void resize();

public:
  DynamicArray();
  ~DynamicArray();
  size_t size() const { return m_size; }
  size_t capacity() const { return m_capacity; }
  T &at(size_t index);
  void push_back(const T &value);
  T &back();
  T &front();
  void push_front(const T &value);
  T &operator[](size_t index);
  void insert(size_t pos, T data);
  void remove_element(T data);
  void erase(T *pos);
  T *erase(T *first, T *last);
  void clear();
  T *begin();
  T *end();

  void right_rotate();
  void left_rotate();
};

template <typename T>
DynamicArray<T>::DynamicArray() : array(nullptr), m_size(0), m_capacity(0) {}

template <typename T>
DynamicArray<T>::~DynamicArray()
{
  delete[] array;
}

template <typename T>
void DynamicArray<T>::resize()
{
  size_t new_capacity = m_capacity == 0 ? 1 : m_capacity * 2;
  T *new_data = new T[new_capacity];
  for (size_t i = 0; i < m_size; ++i)
  {
    new_data[i] = array[i];
  }
  delete[] array;
  array = new_data;
  m_capacity = new_capacity;
}

template <typename T>
T &DynamicArray<T>::at(size_t index)
{
  assert(index >= 0 && index < m_size);
  return array[index];
}

template <typename T>
void DynamicArray<T>::push_back(const T &value)
{
  if (m_size == m_capacity)
  {
    resize();
  }
  array[m_size++] = value;
}

template <typename T>
T &DynamicArray<T>::front()
{
  return array[0];
}

template <typename T>
T &DynamicArray<T>::back()
{
  return array[m_size - 1];
}

template <typename T>
T &DynamicArray<T>::operator[](size_t index)
{
  return at(index);
}

template <typename T>
void DynamicArray<T>::insert(size_t pos, T value)
{
  if (m_size == m_capacity)
  {
    resize();
  }
  for (size_t i = m_size; i > pos; --i)
  {
    array[i] = array[i - 1];
  }
  array[pos] = value;
  ++m_size;
}

template <typename T>
void DynamicArray<T>::push_front(const T &value)
{
  insert(0, value);
}

template <typename T>
T *DynamicArray<T>::begin()
{
  return array;
}

template <typename T>
T *DynamicArray<T>::end()
{
  return array + m_size;
}

template <typename T>
void DynamicArray<T>::erase(T *pos)
{
  if (pos >= array && pos < array + m_size)
  {
    std::move(pos + 1, array + m_size, pos);
    --m_size;
  }
}

template <typename T>
T *DynamicArray<T>::erase(T *first, T *last)
{
  if (first >= array && last <= array + m_size && first < last)
  {
    // Calculate the number of elements to erase
    size_t numElements = last - first;
    // Move the elements from 'last' onwards down to 'first'
    std::move(last, array + m_size, first);
    // Adjust the size
    m_size -= numElements;
    // Return the position now pointing to the element after the last erased one
    return first;
  }
  return last; // If the range is invalid, return 'last'
}

template <typename T>
void DynamicArray<T>::clear()
{
  m_size = 0;
}

template <typename T>
void DynamicArray<T>::right_rotate()
{
  if (m_size > 1)
  {
    T last = array[m_size - 1];
    for (size_t i = m_size - 1; i > 0; --i)
    {
      array[i] = array[i - 1];
    }
    array[0] = last;
  }
}

template <typename T>
void DynamicArray<T>::left_rotate()
{
  if (m_size > 1)
  {
    T first = array[0];
    for (size_t i = 0; i < m_size - 1; i++)
    {
      array[i] = array[i + 1];
    }
    array[m_size - 1] = first;
  }
}

template <typename T>
void DynamicArray<T>::remove_element(T data)
{
  if (m_size == 0)
    return;
  if (!data->shape)
    return;
  for (size_t i = 0; i < m_size; i++)
  {
    // if (array[i]->shape->shape == data->shape->shape && array[i]->shape->color == data->shape->color)
    // if (*data == *array[i])
    if (array[i]->index == data->index)
    {
      erase(array + i);
      break;
    }
  }
}