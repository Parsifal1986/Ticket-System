#ifndef LOG_SYSTEM_HPP
#define LOG_SYSTEM_HPP

#include "../bp_tree/bp_tree.hpp"
#include "../exceptions/exceptions.hpp"
#include "../map/map.hpp"
#include "../time_system/time_system.hpp"
#include "../user_manager/user_manager.hpp"
#include "../train_manager/train_manager.hpp"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

extern int time_stamp;

struct Log {
  Log() {}

  Log(const TrainID &train_id_, const std::string &start_pos_,
      const std::string &end_pos_, const Time &start_time_,
      const Time &end_time_, const int &price_, const int &number_,
      const short &status_, const short start_p_, const short end_p_)
      : train_id(train_id_), start_time(start_time_), end_time(end_time_),
        price(price_), number(number_), status(status_), start_p(start_p_), end_p(end_p_) {
    std::strcpy(start_pos, start_pos_.c_str());
  }

  ~Log() {}

  short status, start_p, end_p;
  TrainID train_id;
  char start_pos[31], end_pos[31];
  Time start_time, end_time;
  int price, number, time_stamp;

  bool operator<(const Log &other) { return time_stamp < other.time_stamp; }

  bool operator==(const Log &other) { return time_stamp == other.time_stamp; }
};

class LogSystem {
public:
  LogSystem();

  ~LogSystem() = default;

  void AddLog(UserName username, TrainID& train_id, std::string& start_pos, std::string& end_pos, Time& start_time,
       Time& end_time, int price, int number, short status, short start_p, short end_p);

  sjtu::vector<Log>* QueryLog(UserName username);

  Log QueryLog(UserName username, int number);
  
  void Update(UserName username, int number, int status);

  bool Update(UserName username, int time_stamp_, int status, bool flag);
private:
  BpTree<UserName, Log> log_database;
};

#endif // LOG_SYSTEM_HPP