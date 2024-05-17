#include "UserManager.hpp"
#include <cstring>
#include <map>

UserData UserDatabase::FindUser(UserName username) {
  sjtu::vector<UserData> *ret;

  try {
    ret = user_database_.Find(username);
  } catch (Exception *error) {
    delete error;
    throw new NoSuchUser();
  }
  
  UserData user = ret->at(0);
  delete ret;
  return user;
};

UserData UserDatabase::DeleteUser(UserName username) {
  UserData user_data = FindUser(username);

  user_database_.Delete(username, user_data);

  return user_data;
};

void UserDatabase::AddUser(UserData user_data) {
  user_database_.Insert(UserName(user_data.username), user_data);
};

void UserDatabase::ModifyUser(UserName username, UserData modified_user_data) {
  UserData old_user_data = DeleteUser(username);

  old_user_data.Modified(modified_user_data);

  AddUser(old_user_data);
};

extern UserDatabase user_database;

bool OnlineSystem::login(UserName username, std::string password) {
  UserData user_data;
  try {
    user_data = user_database.FindUser(username);
  } catch (Exception *error) {
    delete error;
    return false;
  }
  if (online_user_.find(username) != online_user_.end()) {
    return false;
  }
  if (!std::strcmp(user_data.password, password.c_str())) {
    online_user_.insert(sjtu::pair<UserName, UserData>(username, user_data));
    return true;
  }
  return false;
}

bool OnlineSystem::logout(UserName username) {
  auto it = online_user_.find(username);
  if (it == online_user_.end()) {
    return false;
  }
  online_user_.erase(it);
  return true;
}

int OnlineSystem::GetPrivilege(UserName username) {
  return online_user_.find(username)->second.privilege;
}