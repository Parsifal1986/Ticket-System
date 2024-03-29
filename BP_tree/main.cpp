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
  srand(321);
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
  const char *file_path1 = "test1";
  const char *file_path2 = "test2";
  const char *file_path3 = "test3";

  remove(file_path1);
  remove(file_path2);
  remove(file_path3);

  BpTree<Index, int> test1("test1");

  BpTree<int, int> test2("test2");

  BpTree<Index, int> test3("test3");

  // test1

  // int n;

  // std::cin >> n;

  // for (int i = 1; i <= n; i++) {
  //   std::string command;

  //   std::cin >> command;

  //   if (command == "insert") {
  //     Index index;
  //     int data;

  //     std::cin >> index.c >> data;

  //     test1.Insert(index, data);
  //   } else if (command == "delete") {
  //     Index index;
  //     int data;

  //     std::cin >> index.c >> data;
  //     try {
  //       test1.Delete(index, data);
  //     } catch (Exception *error) {
  //       delete error;
  //     }
  //   } else {
  //     Index index;

  //     std::cin >> index.c;
  //     try {
  //       test1.Find(index);
  //       std::cout << std::endl;
  //     } catch (Exception *error) {
  //       error->what();
  //       delete error;
  //     }
  //   }
  // }

  //test2

  std::vector<std::pair<Index, int>> number;
  std::map<Index, int> memory;

  std::string tmp_str = "xojdkagshqnwviufwlxusafosbvsqbrosateazwjrjhmtcrqpqmhqrykstclwvbo";
  Index tmp_index; std::strcpy(tmp_index.c, tmp_str.c_str());
  for (int i = 1; i <= 300000; i++) {
    std::cout << i << " " << std::endl;
    if (i == 287656) {
      std::cout << "Here";
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
          test3.Find(tmp);
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
          test3.Insert(index, value = get_random());
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
          test3.Insert(index, value = get_random());
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
          test3.Insert(index, value = get_random());
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
          test3.Delete(number.back().first, number.back().second);
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

  while (!number.empty()) {
    try {
      test3.Delete(number.back().first, number.back().second);
      memory.erase(memory.find(number.back().first));
    } catch(...) {
      if (memory.find(number.back().first) != memory.end()) {
        std::cout << number.back().first.c;
        std::cout << "Fail" << std::endl;
        getchar();
      }
    }
    number.pop_back();
  }

  // test3

  // for (int j = 0; j <= 1e4; j++) {
  //   for (int i = 1; i <= 9; i++) {
  //     test2.Insert(i, i + j);
  //   }
  // }

  // try {
  //   test2.Find(9);
  // } catch (Exception *error) {
  //   error->what();
  //   delete error;
  // }

  // test4

  // const char* file_path4 = "test4";

  // remove(file_path4);

  // BpTree<Index, int> *test4 = new BpTree<Index, int>("test4");

  // Index test_index;
  // std::strcpy(test_index.c, "okay");

  // for (int i = 1; i <= 3025; i++) {
  //   test4->Insert(test_index, i);
  // }
  // test4->Find(test_index);

  // test4->Delete(test_index, 1);

  // test4->Find(test_index);

  // delete test4;

  // test4 = new BpTree<Index, int>("test4");

  // test4->Find(test_index);
  return 0;
}