#include <algorithm>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <string>
#include "Exceptions.hpp"
#include "bp_tree.hpp"

struct Index {
  char c[65] = {'\0'};

  Index() {}

  bool operator<(Index &rhs) {
    return (std::strcmp(c, rhs.c) < 0);
  }

  bool operator==(Index &rhs) {
    return !(std::strcmp(c, rhs.c));
  }
};

int main() {
  const char* file_path = "bpdata";

  remove(file_path);

  BpTree<Index, int> test;

  int n;

  std::cin >> n;

  for (int i = 1; i <= n; i++) {
    std::string command;

    std::cin >> command;

    if (command == "insert") {
      Index index;
      int data;

      std::cin >> index.c >> data;

      test.Insert(index, data);
    } else if (command == "delete") {
      Index index;
      int data;

      std::cin >> index.c >> data;
      try {
        test.Delete(index, data);
      } catch (Exception *error) {
        delete error;
      }
    } else {
      Index index;

      std::cin >> index.c;
      try {
        test.Find(index);
        std::cout << std::endl;
      } catch (Exception *error) {
        error->what();
        delete error;
      }
    }
  }

  // for (int j = 0; j <=100; j++) {
    // for (int i = 1; i <= 9; i++) {
    //   test1.Insert(i, i + 1);
    // }
  // }
  
  // try {
  //   test1.Find(10);
  // } catch (Exception *error) {
  //   error->what();
  //   delete error;
  // }
  return 0;
}