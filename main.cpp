#include "utilities/exceptions/exceptions.hpp"
#include "utilities/headers.hpp"
#include "utilities/input_processer/input_processer.hpp"
#include "utilities/log_system/log_system.hpp"
#include "utilities/pair/pair.hpp"
#include "utilities/parameter_type/parameter_type.hpp"
#include "utilities/selling_system/selling_system.hpp"
#include "utilities/time_system/time_system.hpp"
#include "utilities/train_manager/train_manager.hpp"
#include "utilities/user_manager/user_manager.hpp"
#include "utilities/vector/vector.hpp"
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
      TrainData train;
      std::string stations, travel_times, stopover_times, price;
      while (token_scanner.HasMoreToken()) {
        std::string parameter = token_scanner.NextToken();
        switch (token_processer.CheckParameter(parameter, TRAIN)) {
          case TRAIN_ID: {
            train.train_id = token_scanner.NextToken();
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
            token_processer.DevidedBySlash(token_scanner.NextToken(), train.place);
            break;
          }
          case PRICES: {
            token_processer.DevidedBySlash(token_scanner.NextToken(), train.price, 1);
            break;
          }
          case START_TIME: {
            train.start_time = token_processer.ProcessTime(token_scanner.NextToken());
            break;
          }
          case TRAVEL_TIME: {
            token_processer.DevidedBySlash(token_scanner.NextToken(), train.travel_time, 1);
            break;
          }
          case STOPOVER_TIME: {
            token_processer.DevidedBySlash(token_scanner.NextToken(), train.stopover_time, 1);
            break;
          }
          case SALEDATE: {
            sjtu::pair<Time, Time> ret = token_processer.ProcessDate(token_scanner.NextToken());
            train.start_sale_date = ret.first;
            train.end_sale_date = ret.second;
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
      for (int i = 1; i < train.station_num; i++) {
        train.price[i] = train.price[i - 1] + train.price[i];
        train.travel_time[i] = train.travel_time[i - 1] + train.travel_time[i];
        train.stopover_time[i] = train.stopover_time[i - 1] + train.stopover_time[i];
      }
      try {
        train_database.AddTrain(train);
        std::cout << 0 << std::endl;
      } catch (Exception *error) {
        delete error;
        std::cout << -1 << std::endl;
      }
    } else if (command == "delete_train") {
      TrainID train_id;
      while (token_scanner.HasMoreToken()) {
        switch (token_processer.CheckParameter(token_scanner.NextToken(), TRAIN)) {
          case TRAIN_ID: {
            train_id = token_scanner.NextToken();
            break;
          }
          default: {
            std::cout << "Undefined Parameter!" << std::endl;
            break;
          }
        }
      }
      if (selling_system.QueryRelease(train_id)) {
        std::cout << -1 << std::endl;
        continue;
      }
      try {
        train_database.DeleteTrain(train_id);
        std::cout << 0 << std::endl;
      } catch (Exception *error) {
        delete error;
        std::cout << -1 << std::endl;
      }
    } else if (command == "release_train") {
      TrainID train_id;
      while (token_scanner.HasMoreToken()) {
        switch (token_processer.CheckParameter(token_scanner.NextToken(), TRAIN)) {
          case TRAIN_ID: {
            train_id = token_scanner.NextToken();
            break;
          }
          default: {
            std::cout << "Undefined Parameter!" << std::endl;
            break;
          }
        }
      }
      try {
        selling_system.ReleaseTrain(train_id);
        std::cout << 0 << std::endl;
      } catch (Exception *error) {
        delete error;
        std::cout << -1 << std::endl;
      }
    } else if (command == "query_train") {
      TrainID train_id;
      Time day;
      while (token_scanner.HasMoreToken()) {
        switch (token_processer.CheckParameter(token_scanner.NextToken(), TRAIN)) {
          case TRAIN_ID: {
            train_id = token_scanner.NextToken();
            break;
          }
          case SALEDATE: {
            day = token_processer.ProcessSingleDate(token_scanner.NextToken());
            break;
          }
          default: {
            std::cout << "Undefined Parameter!" << std::endl;
            break;
          }
        }
      }
      TrainData *train;
      try {
        train = train_database.FindTrain(train_id);
      } catch (Exception *error) {
        delete error;
        std::cout << -1 << std::endl;
        continue;
      }
      if (day < train->start_sale_date || train->end_sale_date < day) {
        std::cout << -1 << std::endl;
        continue;
      }
      int* soldseat = nullptr;
      if (selling_system.QueryRelease(train_id, day)) {
        soldseat = selling_system.QuerySeat(train_id, day); // can be optimized
      }
      std::cout << train->train_id.train_id << " " << train->type << std::endl;
      Time cur_time = day + train->start_time;
      std::cout << train->place[0] << " " << "xx-xx xx:xx" << " -> " << cur_time << " " << train->price[0] << " " << (soldseat == nullptr ? train->seat_num : train->seat_num - soldseat[0]) << std::endl;
      for (int i = 1; i < train->station_num - 1; i++) {
        cur_time.AddTime(0, 0, 0, train->travel_time[i] - train->travel_time[i - 1]);
        std::cout << train->place[i] << " " << cur_time << " -> " << cur_time.AddTime(0, 0, 0, train->stopover_time[i] - train->stopover_time[i - 1]) << " " << train->price[i] << " " << (soldseat == nullptr ? train->seat_num : train->seat_num - soldseat[i]) << std::endl;
      }
      cur_time.AddTime(0, 0, 0, train->travel_time[train->station_num - 1] - train->travel_time[train->station_num - 2]);
      std::cout << train->place[train->station_num - 1] << " " << cur_time << " -> " << "xx-xx xx:xx"<< " " << train->price[train->station_num - 1] << " x"  << std::endl;
      delete train;
    } else if (command == "query_ticket") {
      TrainID train_id;
      std::string start_station, end_station;
      Time start_day;
      int type = 1;
      while (token_scanner.HasMoreToken()) {
        switch (token_processer.CheckParameter(token_scanner.NextToken(), SELLING)) {
          case TRAIN_ID: {
            train_id = token_scanner.NextToken();
            break;
          }
          case SALEDATE: {
            start_day = token_processer.ProcessSingleDate(token_scanner.NextToken());
            break;
          }
          case START_STATION: {
            start_station = token_scanner.NextToken();
            break;
          }
          case END_STATION: {
            end_station = token_scanner.NextToken();
            break;
          }
          case OPTION: {
            type = (token_scanner.NextToken() == "time");
            break;
          }
          default: {
            std::cout << "Undefined Parameter!" << std::endl;
            break;
          }
        }
      }
      sjtu::vector<Ticket> *ret = selling_system.QueryTicket(start_day, start_station, end_station, type);
      selling_system.Output(std::cout, ret);
      delete ret;
    } else if (command == "query_transfer") {
      TrainID train_id;
      std::string start_station, end_station;
      Time start_day;
      bool type = true;
      while (token_scanner.HasMoreToken()) {
        switch (token_processer.CheckParameter(token_scanner.NextToken(), SELLING)) {
          case TRAIN_ID: {
            train_id = token_scanner.NextToken();
            break;
          }
          case SALEDATE: {
            start_day = token_processer.ProcessSingleDate(token_scanner.NextToken());
            break;
          }
          case START_STATION: {
            start_station = token_scanner.NextToken();
            break;
          }
          case END_STATION: {
            end_station = token_scanner.NextToken();
            break;
          }
          case OPTION: {
            type = (token_scanner.NextToken() == "time");
            break;
          }
          default: {
            std::cout << "Undefined Parameter!" << std::endl;
            break;
          }
        }
      }
      sjtu::pair<Ticket, Ticket> *ret = selling_system.QueryTransfer(start_day, start_station, end_station, type);
      selling_system.Output(std::cout, ret);
      delete ret;
    } else if (command == "buy_ticket") {
      UserName username;
      TrainID train_id;
      std::string start_station, end_station;
      Time start_day;
      int number;
      bool candidate = false;
      while (token_scanner.HasMoreToken()) {
        switch (token_processer.CheckParameter(token_scanner.NextToken(), SELLING)) {
          case USERNAME: {
            username = token_scanner.NextToken();
            break;
          }
          case TRAIN_ID: {
            train_id = token_scanner.NextToken();
            break;
          }
          case SALEDATE: {
            start_day = token_processer.ProcessSingleDate(token_scanner.NextToken());
            break;
          }
          case NUMBER: {
            number = token_processer.StringToInt(token_scanner.NextToken());
            break;
          }
          case START_STATION: {
            start_station = token_scanner.NextToken();
            break;
          }
          case END_STATION: {
            end_station = token_scanner.NextToken();
            break;
          }
          case OPTION: {
            candidate = (token_scanner.NextToken() == "true");
            break;
          }
          default: {
            std::cout << "Undefined Parameter!" << std::endl;
            break;
          }
        }
      }
      if (!online_system.CheckIfOnline(username)) {
        std::cout << "-1" << std::endl;
        continue;
      }
      int price = selling_system.BuyTicket(username, train_id, start_day, start_station, end_station, number, candidate);
      if (price == -1) {
        std::cout << -1 << std::endl;
      } else if (price == -2) {
        std::cout << "queue" << std::endl;
      } else {
        std::cout << price << std::endl;
      }
    } else if (command == "query_order") {
      UserName username;
      while (token_scanner.HasMoreToken()) {
        std::string parameter = token_scanner.NextToken();
        switch (token_processer.CheckParameter(parameter, SELLING)) {
          case USERNAME: {
            username = token_scanner.NextToken();
            break;
          }
          default: {
            std::cout << "Undefined Parameter!" << std::endl;
          }
        }
      }
      if (!online_system.CheckIfOnline(username)) {
        std::cout << -1 << std::endl;
        continue;
      }
      sjtu::vector<Log> *ret = log_system.QueryLog(username);
      log_system.Output(std::cout, ret);
      delete ret;
    } else if (command == "refund_ticket") {
      UserName username;
      int number = 1;
      while (token_scanner.HasMoreToken()) {
        std::string parameter = token_scanner.NextToken();
        switch (token_processer.CheckParameter(parameter, SELLING)) {
          case USERNAME: {
            username = token_scanner.NextToken();
            break;
          }
          case NUMBER: {
            number = token_processer.StringToInt(token_scanner.NextToken());
            break;
          }
          default: {
            std::cout << "Undefined Parameter!" << std::endl;
            break;
          }
        }
      }
      if (!online_system.CheckIfOnline(username)) {
        std::cout << -1 << std::endl;
        continue;
      }
      std::cout << (selling_system.RefundTicket(username, number) ? 0 : -1) << std::endl;
    } else if (command == "clean") {
      std::cout << std::endl;
    } else if (command == "exit") {
      std::cout << "bye" << std::endl;
    } else {
      std::cout << "Undefined Command!" << std::endl;
    }
  }
  return 0;
}