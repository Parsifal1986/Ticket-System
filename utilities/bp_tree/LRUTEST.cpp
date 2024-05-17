#include "LRU.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>

LruCache<int, 7, 256> LRU;

int main() {
  for (int i = 1; i <= 10; i++) {
    LRU.Throw(i * sizeof(int), new int(i), std::ios::beg);
  }

  for (int i = 1; i <= 10; i++) {
    int *p = LRU.Find(i);
    if (p != nullptr) {
      std::cout << *p << std::endl;
    } else {
      std::cout << "NOT IN CACHE!" << std::endl;
    }
  }
  return 0;
}