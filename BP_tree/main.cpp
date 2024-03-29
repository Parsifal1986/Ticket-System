#include <algorithm>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "Exceptions.hpp"
#include "bp_tree.hpp"
#include <utility>
#include <map>
#include <vector>
#include <assert.h>

struct Index {
  char c[65] = {'\0'};

  Index() {}

  bool operator<(const Index &rhs) const {
    return (std::strcmp(c, rhs.c) < 0);
  }

  bool operator==(const Index &rhs) const {
    return !(std::strcmp(c, rhs.c));
  }
};

void init_seed() {
  srand(123);
}

int get_random() {
  return rand();
}

void random_string_generator(Index &tmp) {
  for (int i = 0 ; i < 64; i++) {
    tmp.c[i] = 'a' + get_random() % 26;
  }
  return;
}

int main() {
  const char* file_path = "bpdata";

  remove(file_path);

  // BpTree<Index, int> test;

  // BpTree<int, int> test1;

  BpTree<Index, int> test1;

  // int n;

  // std::cin >> n;

  // for (int i = 1; i <= n; i++) {
  //   std::string command;

  //   std::cin >> command;

  //   if (command == "insert") {
  //     Index index;
  //     int data;

  //     std::cin >> index.c >> data;

  //     test.Insert(index, data);
  //   } else if (command == "delete") {
  //     Index index;
  //     int data;

  //     std::cin >> index.c >> data;
  //     try {
  //       test.Delete(index, data);
  //     } catch (Exception *error) {
  //       delete error;
  //     }
  //   } else {
  //     Index index;

  //     std::cin >> index.c;
  //     try {
  //       test.Find(index);
  //       std::cout << std::endl;
  //     } catch (Exception *error) {
  //       error->what();
  //       delete error;
  //     }
  //   }
  // }

  std::vector<std::pair<Index, int>> number;
  std::map<Index, int> memory;

  std::string tmp_str = "iwjrfduujgrkuddsfkjjwqjjoiaptulbquvxxprgvksqnwktiwefmpqczsusnfuf";
  Index tmp_index; std::strcpy(tmp_index.c, tmp_str.c_str());
  for (int i = 1; i <= 300000; i++) {
    std::cout << i << " " << std::endl;
    if (i == 403) {
      std::cout << " HERE";
    }
    try {
      test1.Find(tmp_index);
    } catch (...) {
      if (memory.find(tmp_index) != memory.end()) {
        std::cout << " " << memory[tmp_index];
        getchar();
      }
    }
    switch (get_random() % 5) {
      case 1 : {
        if (number.empty()) {
          continue;
        }
        Index tmp = number[get_random() % number.size()].first;
        try {
          test1.Find(tmp);
          std::cout << " " << memory[tmp] << std::endl;
        } catch (Exception *error) {
          if (memory.find(tmp) != memory.end()) {
            std::cout << tmp.c << " " << memory[tmp];
            std::cout << "Fail!";
            getchar();
          }
          error->what();
          delete error;
        }
        break;
      }
      case 2 : {
        Index index;
        int value;
        do {
          random_string_generator(index);
        } while (memory.find(index) != memory.end());
        try {
          test1.Insert(index, value = get_random());
          memory.insert(std::make_pair(index, value));
          number.push_back(std::make_pair(index, value));
        } catch (Exception *error) {
          error->what();
          delete error;
        }
        break;
      }
      case 3: {
        Index index;
        int value;
        do {
          random_string_generator(index);
        } while (memory.find(index) != memory.end());
        try {
          test1.Insert(index, value = get_random());
          memory.insert(std::make_pair(index, value));
          number.push_back(std::make_pair(index, value));
        } catch (Exception *error) {
          error->what();
          delete error;
        }
        break;
      }
      case 4: {
        Index index;
        int value;
        do {
          random_string_generator(index);
        } while (memory.find(index) != memory.end());
        try {
          test1.Insert(index, value = get_random());
          memory.insert(std::make_pair(index, value));
          number.push_back(std::make_pair(index, value));
        } catch (Exception *error) {
          error->what();
          delete error;
        }
        break;
      }
      default: {
        if (number.empty()) {
          continue;
        }
        try {
          test1.Delete(number.back().first, number.back().second);
          memory.erase(memory.find(number.back().first));
        } catch (...) {
          if (memory.erase(memory.find(number.back().first)) != memory.end()) {
            std::cout << "Fail";
            getchar();
          }
        }
        number.pop_back();
      }
    }
  }

  // for (int j = 0; j <= 1e4; j++) {
  //   for (int i = 1; i <= 9; i++) {
  //     test1.Insert(i, i + j);
  //   }
  // }

  // try {
  //   test1.Find(9);
  // } catch (Exception *error) {
  //   error->what();
  //   delete error;
  // }
  return 0;
}