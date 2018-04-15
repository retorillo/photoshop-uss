#pragma once

#include <vector>
#include <functional>
using namespace std;

#define DISPOSER_INITIAL_CAPACITY 8
#define ACTION function<void()>

class disposer {
  vector<ACTION> stack;
public:
  disposer();
  ~disposer();
  void push(ACTION f);
};
