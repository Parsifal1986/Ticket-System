#ifndef USER_MANAGER_HPP
#define USER_MANAGER_HPP

#include "../exceptions/exceptions.hpp"
#include "../exceptions/exceptions.hpp"
#include "../bp_tree/bp_tree.hpp"
#include "../map/map.hpp"
#include "../vector/vector.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

struct UserName {
  UserName() {

  }

  UserName(std::string username_) {
    std::strcpy(username, username_.c_str());
    return;
  }

  ~UserName() {

  }

  bool Empty() const {
    return (username[0] == '\0');
  }

  char username[21] = {'\0'};

  bool operator<(const UserName &rhs) const { return (std::strcmp(username, rhs.username) < 0); }

  bool operator==(const UserName &rhs) const { return !(std::strcmp(username, rhs.username)); }

  void operator=(const UserName &rhs) { std::strcpy(username, rhs.username); }
};

struct UserData {
  UserName username;
  char password[31] = {'\0'}, mailAddr[31] = {'\0'}, name[16] = {'\0'};
  short privilege = -1;

  UserData() {

  };

  UserData(const UserData &other) {
    if (this == &other) {
      return;
    }
    username = other.username;
    std::strcpy(password, other.password); std::strcpy(mailAddr, other.mailAddr);
    std::strcpy(name, other.name);
    privilege = other.privilege;
  };

  void Modified(const UserData &other) {
    if (this == &other) {
      return;
    }
    if (!other.username.Empty()) {
      username = other.username;
    }
    if (other.mailAddr[0] != '\0') {
      std::strcpy(mailAddr, other.mailAddr);
    }
    if (other.password[0] != '\0') {
      std::strcpy(password, other.password);
    }
    if (other.name[0] != '\0') {
      std::strcpy(name, other.name);
    }
    if (other.privilege != -1) {
      privilege = other.privilege;
    }
  }

  bool operator<(const UserData &rhs) const { return username < rhs.username;}

  bool operator==(const UserData &rhs) const { return username == rhs.username; } // The username is unique
};

class UserDatabase {
public:
  UserData FindUser(UserName username);

  UserData DeleteUser(UserName username);

  void AddUser(UserData user_data);

  void ModifyUser(UserName username, UserData modified_user_data);

private:
  std::fstream file_;
  BpTree<UserName, UserData> user_database_;
};

class OnlineSystem {
public:
  bool login(UserName username, std::string password);

  bool logout(UserName username);

  int GetPrivilege(UserName username);
private:
  sjtu::map<UserName, UserData> online_user_;
};

#endif // USER_MANAGER_HPP