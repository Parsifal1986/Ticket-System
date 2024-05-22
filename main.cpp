#include "utilities/exceptions/exceptions.hpp"
#include "utilities/headers.hpp"
#include "utilities/input_processer/input_processer.hpp"
#include "utilities/parameter_type/parameter_type.hpp"
#include "utilities/train_manager/train_manager.hpp"
#include "utilities/user_manager/user_manager.hpp"
#include <cstdio>
#include <iostream>

UserDatabase user_database;
OnlineSystem online_system;
TrainDatabase train_database;
LogSystem log_system;
TokenScanner token_scanner;
TokenProcesser token_processer;
SellingSystem selling_system;
int time_stamp;

int main() {
  std::string line, command;

  while (command != "exit") {
    getline(std::cin, line);
    token_scanner.SetLine(line);
    time_stamp = token_processer.ReadTimeStamp(token_scanner.NextToken());
    command = token_scanner.NextToken();

    std::cout << "[" << time_stamp << "]" << " ";

    if (command == "add_user") {
      bool fine = true;
      short current_user_privilege = -1;
      UserData user;
      bool is_first_user = user_database.IsFirstUser();
      while (token_scanner.HasMoreToken() && fine) {
        std::string parameter = token_scanner.NextToken();
        switch (token_processer.CheckParameter(parameter, USER)) {
          case CURRENT_USER: {
            UserName tmp = token_scanner.NextToken();
            if (is_first_user) {
              break;
            }
            if (!online_system.CheckIfOnline(tmp)) {
              std::cout << "-1" << std::endl;
              fine = false;
            } else {
              current_user_privilege = online_system.GetPrivilege(tmp);
            }
            break;
          }
          case USERNAME: {
            user.username = token_scanner.NextToken();
            break;
          }
          case PASSWORD: {
            std::strcpy(user.password, token_scanner.NextToken().c_str());
            break;
          }
          case NAME: {
            std::strcpy(user.name, token_scanner.NextToken().c_str());
            break;
          }
          case MAIL_ADDRESS: {
            std::strcpy(user.mailAddr, token_scanner.NextToken().c_str());
            break;
          }
          case PRIVILEGE: {
            if (is_first_user) {
              token_scanner.NextToken();
              break;
            }
            user.privilege = token_processer.StringToInt(token_scanner.NextToken());
            break;
          }
          default: {
            std::cout << "Undefined Parameter!" << std::endl;
            break;
          }
        }
      }
      if (!fine) {
        continue;
      }
      if (current_user_privilege == -1) {
        if (is_first_user) {
          user.privilege = 10;
          user_database.AddUser(user);
          std::cout << "0" << std::endl;
        } else {
          std::cout << "-1" << std::endl;
        }
      } else {
        if (current_user_privilege <= user.privilege) {
          std::cout << "-1" << std::endl;
        } else {
          user_database.AddUser(user);
          std::cout << "0" << std::endl;
        }
      }
    } else if (command == "login") {
      UserName username;
      std::string password;
      while (token_scanner.HasMoreToken()) {
        std::string parameter = token_scanner.NextToken();

        switch (token_processer.CheckParameter(parameter, USER)) {
          case USERNAME: {
            username = token_scanner.NextToken();
            break;
          }
          case PASSWORD: {
            password = token_scanner.NextToken();
            break;
          }
          default: {
            std::cout << "Undefined Parameter!" << std::endl;
            break;
          }
        }
      }
      std::cout << (online_system.Login(username, password) ? 0 : -1) << std::endl;
    } else if (command == "logout") {
      UserName username;
      while (token_scanner.HasMoreToken()) {
        std::string parameter = token_scanner.NextToken();
        switch (token_processer.CheckParameter(parameter, USER)) {
          case USERNAME: {
            username = token_scanner.NextToken();
            break;
          }
          default: {
            std::cout << "Undefined Parameter!" << std::endl;
            break;
          }
        }
      }
      std::cout << (online_system.Logout(username) ? 0 : -1) << std::endl;
    } else if (command == "query_profile") {
      bool fine = true;
      UserName cur_username, username;
      short current_user_privilege;
      while (token_scanner.HasMoreToken() && fine) {
        std::string parameter = token_scanner.NextToken();
        switch (token_processer.CheckParameter(parameter, USER)) {
          case CURRENT_USER: {
            cur_username = token_scanner.NextToken();
            if (!online_system.CheckIfOnline(cur_username)) {
              std::cout << "-1" << std::endl;
              fine = false;
            } else {
              current_user_privilege = online_system.GetPrivilege(cur_username);
            }
            break;
          }
          case USERNAME: {
            username = token_scanner.NextToken();
            break;
          }
          default: {
            std::cout << "Undefined Parameter!" << std::endl;
          }
        }
      }
      if (!fine) {
        continue;
      }
      try {
        UserData user;
        user = user_database.QueryUser(username, (cur_username == username) ? 11 : current_user_privilege);
        std::cout << user.username.username << " " << user.name << " " << user.mailAddr << " " << user.privilege <<std::endl;
      } catch (Exception *error) {
        delete error;
        std::cout << -1 << std::endl;
      }
    } else if (command == "modify_profile") {
      bool fine = true;
      short current_user_privilege;
      UserData user;
      UserName username, current_username;
      while (token_scanner.HasMoreToken() && fine) {
        std::string parameter = token_scanner.NextToken();
        switch (token_processer.CheckParameter(parameter, USER)) {
          case CURRENT_USER: {
            current_username = token_scanner.NextToken();
            if (!online_system.CheckIfOnline(current_username)) {
              std::cout << "-1" << std::endl;
              fine = false;
            } else {
              current_user_privilege = online_system.GetPrivilege(current_username);
            }
            break;
          }
          case USERNAME: {
              username = token_scanner.NextToken();
              break;
          }
          case PASSWORD: {
              std::strcpy(user.password, token_scanner.NextToken().c_str());
              break;
          }
          case NAME: {
              std::strcpy(user.name, token_scanner.NextToken().c_str());
              break;
          }
          case MAIL_ADDRESS: {
              std::strcpy(user.mailAddr, token_scanner.NextToken().c_str());
              break;
          }
          case PRIVILEGE: {
              user.privilege = token_processer.StringToInt(token_scanner.NextToken());
              break;
          }
          default: {
              std::cout << "Undefined Parameter!" << std::endl;
              break;
          }
        }
      }
      if (!fine) {
        continue;
      }
      if (user.privilege != -1 && user.privilege >= current_user_privilege) {
        std::cout << -1 << std::endl;
        continue;
      }
      try {
        UserData new_user = user_database.ModifyUser(username, user, (current_username == username) ? 11 : current_user_privilege);
        std::cout << new_user.username.username << " " << new_user.name << " " << new_user.mailAddr << " " << new_user.privilege <<std::endl;
        if (user.privilege != -1 && online_system.CheckIfOnline(username)) {
          online_system.UpdatePrivilege(username, user.privilege);
        }
      } catch (Exception *error) {
        delete error;
        std::cout << -1 << std::endl;
      }
    } else if (command == "add_train") {
      bool fine = true;
      TrainID train_id;
      TrainData train;
      std::string stations, travel_times, stopover_times, price;
      while (token_scanner.HasMoreToken() && fine) {
        std::string parameter = token_scanner.NextToken();
        switch (token_processer.CheckParameter(parameter, TRAIN)) {
          case TRAIN_ID: {
            train_id = token_scanner.NextToken();
            break;
          }
          case STATION_NUM: {
            train.station_num = token_processer.StringToInt(token_scanner.NextToken());
            break;
          }
          case SEAT_NUM: {
            train.seat_num = token_processer.StringToInt(token_scanner.NextToken());
            break;
          }
          case STATIONS: {
            stations = token_scanner.NextToken();
            token_processer.DevidedBySlash(stations, train.place);
            break;
          }
          case PRICES: {
            price = token_scanner.NextToken();
            token_processer.DevidedBySlash(price, train.price);
            break;
          }
          case START_TIME: {

            break;
          }
          case TRAVEL_TIME: {

            break;
          }
          case STOPOVER_TIME: {

            break;
          }
          case SALEDATE: {

            break;
          }
          case TYPE: {
            train.type = token_scanner.NextToken()[0];
            break;
          }
          default: {
            std::cout << "Undefined Parameter!" << std::endl;
            break;
          }
        }
      }
      if (!fine) {
        continue;
      }
    } else if (command == "delete_train") {

    } else if (command == "release_train") {

    } else if (command == "") {

    } else if (command == "exit") {
      std::cout << "bye" << std::endl;
    } else {
      std::cout << "Undefined Command!" << std::endl;
    }
  }
  return 0;
}