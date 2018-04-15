#include "disposer.h"

disposer::disposer() {
  stack.reserve(DISPOSER_INITIAL_CAPACITY);
}
disposer::~disposer() {
  for (auto& a : stack) a();
}
void disposer::push(ACTION f) {
  stack.push_back(f);
}
