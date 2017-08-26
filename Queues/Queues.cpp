// Queues.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "ArrayDeque.h"

using std::cout;
using std::cin;
using std::string;

int main()
{
  Deque<string> dq;

  for (int i = 1; i < 10; i++)
  {
    dq.debug();
    dq.push_front("dogs f1");
    dq.push_front("dogs f2");
    cout << "f " << dq.front() << endl;
    cout << "b " << dq.back() << endl;
    cout << "pop b " << dq.pop_back() << endl;
    cout << "pop b " << dq.pop_front() << endl;
    dq.debug();
    cout << endl;
  }
  
  cin.get();

  return 0;
}

