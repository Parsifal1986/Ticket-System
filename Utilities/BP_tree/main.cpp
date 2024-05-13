#include "Exceptions.hpp"
#include "LRU.hpp"
#include "bp_tree.hpp"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

std::fstream file_;

struct Index {
  char c[65] = {'\0'};

  Index() {}

  bool operator<(const Index &rhs) const { return (std::strcmp(c, rhs.c) < 0); }

  bool operator==(const Index &rhs) const { return !(std::strcmp(c, rhs.c)); }
};

void init_seed() { srand(123); }

int get_random() { return rand(); }

void random_string_generator(Index &tmp) {
  for (int i = 0; i < get_random() % 64 + 1; i++) {
    tmp.c[i] = 'a' + get_random() % 26;
  }
  return;
}

int main() {
  // const char *file_path1 = "test1";
  // const char *file_path2 = "test2";
  // const char *file_path3 = "test3";

  // remove(file_path1);
  // remove(file_path2);
  // remove(file_path3);

  // BpTree<Index, int> test1("test1");

  // BpTree<int, int, 5> test2("test2");

  // BpTree<Index, int> test3("test3");

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

  // fclose(stdout);

  // test2

  // std::vector<std::pair<Index, int>> number;
  // std::map<Index, int> memory;

  // std::string tmp_str = "mrazdjdamtqmxelcfg";
  // Index tmp_index; std::strcpy(tmp_index.c, tmp_str.c_str());
  // for (int i = 1; i <= 300000; i++) {
  //   if (i == 5523) {
  //     std::cout << "here";
  //   }
  //     std::cout << i << std::endl;
  //   try {
  //     test3.Find(tmp_index);
  //   } catch (Exception *error) {
  //     if (memory.find(tmp_index) != memory.end()) {
  //       getchar();
  //     }
  //     delete error;
  //   }
  //   switch (get_random() % 5) {
  //     case 1 : {
  //       if (number.empty()) {
  //         continue;
  //       }
  //       Index tmp = number[get_random() % number.size()].first;
  //       try {
  //         test3.Find(tmp);
  //         std::cout << " " << memory[tmp] << std::endl;
  //       } catch (Exception *error) {
  //         if (memory.find(tmp) != memory.end()) {
  //           std::cout << tmp.c << " " << memory[tmp];
  //           std::cout << "Fail!";
  //           getchar();
  //         }
  //         error->what();
  //         delete error;
  //       }
  //       break;
  //     }
  //     case 2 : {
  //       Index index;
  //       int value;
  //       do {
  //         random_string_generator(index);
  //       } while (memory.find(index) != memory.end());
  //       try {
  //         test3.Insert(index, value = get_random());
  //         memory.insert(std::make_pair(index, value));
  //         number.push_back(std::make_pair(index, value));
  //       } catch (Exception *error) {
  //         error->what();
  //         delete error;
  //       }
  //       break;
  //     }
  //     case 3: {
  //       Index index;
  //       int value;
  //       do {
  //         random_string_generator(index);
  //       } while (memory.find(index) != memory.end());
  //       try {
  //         test3.Insert(index, value = get_random());
  //         memory.insert(std::make_pair(index, value));
  //         number.push_back(std::make_pair(index, value));
  //       } catch (Exception *error) {
  //         error->what();
  //         delete error;
  //       }
  //       break;
  //     }
  //     case 4: {
  //       Index index;
  //       int value;
  //       do {
  //         random_string_generator(index);
  //       } while (memory.find(index) != memory.end());
  //       try {
  //         test3.Insert(index, value = get_random());
  //         memory.insert(std::make_pair(index, value));
  //         number.push_back(std::make_pair(index, value));
  //       } catch (Exception *error) {
  //         error->what();
  //         delete error;
  //       }
  //       break;
  //     }
  //     default: {
  //       if (number.empty()) {
  //         continue;
  //       }
  //       try {
  //         test3.Delete(number.back().first, number.back().second);
  //         memory.erase(memory.find(number.back().first));
  //       } catch (Exception *error) {
  //         if (memory.erase(memory.find(number.back().first)) != memory.end())
  //         {
  //           std::cout << "Fail";
  //           getchar();
  //         }
  //         delete error;
  //       }
  //       number.pop_back();
  //     }
  //   }
  // }

  // while (!number.empty()) {
  //   try {
  //     test3.Delete(number.back().first, number.back().second);
  //     memory.erase(memory.find(number.back().first));
  //   } catch(Exception *error) {
  //     if (memory.find(number.back().first) != memory.end()) {
  //       std::cout << number.back().first.c;
  //       std::cout << "Fail" << std::endl;
  //       getchar();
  //     }
  //     delete error;
  //   }
  //   number.pop_back();
  // }

  // test3

  // for (int i = 50; i <= 60; i++) {
  //   for (int j = 1; j <= i; j++) {
  //     test2.Insert(i, j + i);
  //   }

  //   test2.Find(i);
  //   std::cout << std::endl;

  //   for (int j = 1; j <= i; j++) {
  //     test2.Delete(i, j + i);
  //   }
  // }
  // for (int j = 1; j <= 13; j++) {
  //   test2.Insert(j,  j + 1);
  // }
  // for (int j = 11; j <= 13; j++) {
  //   test2.Delete(j, j + 1);
  //   for (int i = 1; i <= 13; i++) {
  //     try {
  //       test2.Find(i);
  //       std::cout << std::endl;
  //     } catch (Exception *error) {
  //       error->what();
  //       delete error;
  //     }
  //   }
  //   test2.Insert(j, j + 1);
  // }
  // for (int i = 400 * 400; i >= 1; i--) {
  //   test2.Insert(i, i);
  //   test2.Insert(i + 1, i + 1);
  //   test2.Delete(i + 1, i + 1);
  // }

  // for (int i = 1; i <= 400 * 400; i++) {
  //   test2.Find(i);
  // }
  // test2.Find(2);
  // std::cout << std::endl;
  // test2.Delete(2, 3);
  // test2.Insert(2, 3);

  // for (int i = 1; i <= 14; i++) {
  //   try {
  //     test2.Delete(2, 2 + i);
  //     test2.Find(2);
  //     test2.Insert(2, 2 + i);
  //     std::cout << std::endl;
  //   } catch (Exception *error) {
  //     error->what();
  //   }
  // }

  // try {
  //   test2.Find(9);
  // } catch (Exception *error) {
  //   error->what();
  //   delete error;
  // }

  // test4

  const char *file_path4 = "test4";

  remove(file_path4);

  BpTree<Index, int> *test4 = new BpTree<Index, int>("test4");

  Index test_index;
  std::strcpy(test_index.c, "okay");

  for (int i = 1; i <= 3025; i++) {
    test4->Insert(test_index, i);
  }
  test4->Find(test_index);

  test4->Delete(test_index, 10);

  test4->Find(test_index);

  test4->Insert(test_index, 10);

  delete test4;

  test4 = new BpTree<Index, int>("test4");

  test4->Find(test_index);

  // test5

  // const char *file_path5 = "test5";

  // remove(file_path5);

  // BpTree<Index, int> *test5 = new BpTree<Index, int>("test5");

  // Index test_index;
  // std::strcpy(test_index.c, "okay");

  // for (int i = 1; i <= 10; i++) {
  //   test5->Insert(test_index, i);
  // }
  // test5->Find(test_index);

  // for (int i = 1; i <= 10; i++) {
  //   test5->Delete(test_index, i);
  //   try {
  //     test5->Find(test_index);
  //   } catch (Exception *error) {
  //     error->what();
  //   }
  // }

  // test5->Insert(test_index, 10);

  // delete test5;

  // test5 = new BpTree<Index, int>("test5");

  // test5->Find(test_index);

  // test6

  // std::vector<std::pair<Index, int>> number;
  // std::map<Index, int> memory;

  // for (int i = 1; i <= 300000; i++) {
  //   Index index;
  //   int value;
  //   do {
  //     random_string_generator(index);
  //   } while (memory.find(index) != memory.end());
  //   try {
  //     test3.Insert(index, value = get_random());
  //     memory.insert(std::make_pair(index, value));
  //     number.push_back(std::make_pair(index, value));
  //   } catch (Exception *error) {
  //     error->what();
  //     delete error;
  //   }
  // }

  // while (number.size()) {
  //   try {
  //     test3.Delete(number.back().first, number.back().second);
  //     memory.erase(memory.find(number.back().first));
  //   } catch (Exception *error) {
  //     if (memory.erase(memory.find(number.back().first)) !=
  //     memory.end()) {
  //       std::cout << "Fail";
  //       getchar();
  //     }
  //     delete error;
  //   }
  //   number.pop_back();
  // }
  return 0;
}