#include <assert.h>
#include <memory>
#include <iterator>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::ostream;
using std::string;

/* * *
* Array based Deque container
* - custom iterator that satisfies enhanced for loop conditions
* - grows 2x on reaching front or back of allocated buffer
* - places the structure in the middle of the allocated buffer
* - circular buffer is probably much better for all cases
* - dissatisfied with indexes being one off and the special cases for
*   an empty container. i believe these can be refactored out
*/
template <class T>
class Deque
{
private:
  T* m_data{ nullptr };
  size_t m_size{ 0 };
  size_t m_capacity{ 0 };
  size_t m_front_index{ 0 };
  size_t m_back_index{ 0 };

  void check_size()
  {
    assert(m_front_index >= 0);
    assert(m_back_index <= m_capacity);

    if (m_front_index == 0 || m_back_index >= (m_capacity / sizeof(T)))
    {
      int tmp_capacity;

      if (m_capacity != 0)
      {
        tmp_capacity = 2 * m_capacity;
      }
      else {
        tmp_capacity = 4 * sizeof(T);
      }

      T* tmp_data = new T[tmp_capacity];

      size_t index_offset = (tmp_capacity / sizeof(T)) / 2;
      size_t new_front_index = index_offset - (m_size / 2) - 1 - (m_size % 2);
      size_t new_back_index = index_offset + (m_size / 2);

      if (m_size != 0)
      {
        T* tmp_start = tmp_data + new_front_index;
        T* curr_start = m_data + m_front_index;        
        
        std::copy(curr_start, curr_start + size(), stdext::checked_array_iterator<T*>(tmp_start, size()));

        delete[] m_data;
        m_data = nullptr;
      }

      m_data = tmp_data;
      m_capacity = tmp_capacity;
      m_front_index = new_front_index;
      m_back_index = new_back_index;
    }
  }

public:
  Deque() = default;

  Deque(size_t m_size) : m_size(m_size)
  {

  }

  bool empty()
  {
    return m_size == 0;
  }

  size_t size()
  {
    return m_size;
  }

  size_t capacity()
  {
    return m_capacity;
  }

  void push_back(const T& val)
  {
    check_size();

    m_data[m_back_index++] = val;

    if (m_size == 0)
    {
      m_front_index--;
    }

    m_size++;
  }

  void push_front(const T& val)
  {
    check_size();

    m_data[m_front_index--] = val;

    if (m_size == 0)
    {
      m_back_index = m_front_index + 2;
    }

    m_size++;
  }

  T& pop_back()
  {
    assert(!empty());
    m_back_index--;
    m_size--;

    if (m_size == 0)
    {
      T &temp = m_data[m_back_index];
      m_front_index = m_capacity / sizeof(T) / 2;
      m_back_index = m_capacity / sizeof(T) / 2;
      return temp;
    }

    return m_data[m_back_index];
  }

  T& pop_front()
  {
    assert(!empty());
    m_size--;
    m_front_index++;

    if (m_size == 0)
    {
      T &temp = m_data[m_front_index];
      m_front_index = m_capacity / sizeof(T) / 2;
      m_back_index = m_capacity / sizeof(T) / 2;
      return temp;
    }

    return m_data[m_front_index];
  }

  T& front()
  {
    if (size() == 0)
    {
      return m_data[m_front_index];
    }
    else
    {
      return m_data[m_front_index + 1];
    }
  }

  T& back()
  {
    if (size() == 0)
    {
      return m_data[m_back_index];
    }
    else
    {
      return m_data[m_back_index - 1];
    }
  }

  void insert(const T& val)
  {
    push_back(val);
  }

public:
  class iterator;

  iterator begin()
  {
    return iterator(0, *this);
  }

  iterator end()
  {
    return iterator(m_size, *this);
  }

public:
  T& operator[](size_t index)
  {
    size_t desired_index = index + m_front_index + 1;
    assert(desired_index <= m_back_index);
    return m_data[desired_index];
  }

public:
  void debug()
  {
    cout << "frnt: " << m_front_index << endl;
    cout << "back: " << m_front_index << endl;
    cout << "size: " << m_size << endl;
    cout << "capa: " << m_capacity << endl;
  }
};

template <class T>
class Deque<T>::iterator// : public std::iterator<std::random_access_iterator_tag, T, ptrdiff_t, T*, T&>
{
private:
  size_t m_index{ 0 };
  Deque<T> m_dq;

public:
  iterator() = delete;

  iterator(size_t m_index, const Deque<T>& m_dq) : m_index(m_index), m_dq(m_dq)
  {

  }

  // postfix
  iterator operator++(int)
  {
    m_index++;
    return *this;
  }

  // prefix
  iterator operator++()
  {
    iterator prev = *this;
    m_index++;
    return prev;
  }

  // postfix
  iterator operator--(int j)
  {
    m_index--;
    return *this;
  }

  // prefix
  iterator operator--()
  {
    iterator prev = *this;
    index--;
    return prev;
  }

  bool operator!=(const iterator& other)
  {
    return m_index != other.m_index && m_dq.m_data == other.m_dq.m_data;
  }

  bool operator<(const iterator& other)
  {
    return m_index < other.m_index;
  }

  T& operator*()
  {
    return m_dq[m_index];
  }
};