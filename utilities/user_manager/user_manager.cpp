#include "user_manager.hpp"

UserDatabase::UserDatabase() : user_database_("UserData") {

}

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

UserData UserDatabase::QueryUser(UserName username, short privilege) {
  UserData user = FindUser(username);

  if (user.privilege >= privilege) {
    throw new PriviledgeIsNotEnough();
  }

  return user;
}

UserData UserDatabase::DeleteUser(UserName username) {
  UserData user_data = FindUser(username);

  user_database_.Delete(username, user_data);

  return user_data;
};

void UserDatabase::AddUser(UserData user_data) {
  user_database_.Insert(UserName(user_data.username), user_data);
};

UserData UserDatabase::ModifyUser(UserName username, UserData modified_user_data, short priviledge) {
  UserData old_user_data = FindUser(username);

  if (old_user_data.privilege > priviledge) {
    throw new PriviledgeIsNotEnough();
  }

  DeleteUser(username);

  old_user_data.Modified(modified_user_data);

  AddUser(old_user_data);

  return old_user_data;
};

bool UserDatabase::IsFirstUser() {
  return user_database_.Empty();
}

extern UserDatabase user_database;

bool OnlineSystem::Login(UserName username, std::string password) {
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
    online_user_.insert(sjtu::pair<UserName, short>(username, user_data.privilege));
    return true;
  }
  return false;
}

bool OnlineSystem::Logout(UserName username) {
  auto it = online_user_.find(username);
  if (it == online_user_.end()) {
    return false;
  }
  online_user_.erase(it);
  return true;
}

int OnlineSystem::GetPrivilege(UserName username) {
  return online_user_.find(username)->second;
}

void OnlineSystem::UpdatePrivilege(UserName username, short privilege) {
  online_user_[username] = privilege;
}

bool OnlineSystem::CheckIfOnline(UserName username) {
  auto it = online_user_.find(username);
  if (it == online_user_.end()) {
    return false;
  }
  return true;
}