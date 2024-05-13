#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP

#include "BP_tree/bp_tree.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

struct UserName {
  char username[21];

  bool operator<(const UserName &rhs) const { return (std::strcmp(username, rhs.username) < 0); }

  bool operator==(const UserName &rhs) const { return !(std::strcmp(username, rhs.username)); }
};

struct UserData {
  UserName username;
  char password[31], mailAddr;
  char32_t name[6];
  short privilege;
};

class UserDatabase {
public:


private:
  BpTree<UserName, UserData> user_database;
};

class UserManager {
public:

private:
  
}

#endif // USERMANAGER_HPP