#include "log_system.hpp"
#include <iostream>
#include <ostream>

LogSystem::LogSystem() : log_database("log_data") {

}

void LogSystem::AddLog(UserName username, TrainID& train_id, std::string& start_pos, std::string& end_pos, Time start_time,
       Time end_time, Time start_date, int price, int number, Status status, short start_p, short end_p) {
  log_database.Insert(username, Log(train_id, start_pos, end_pos, start_time,
                                    end_time, start_date, price, number, status, start_p, end_p, time_stamp));
}

sjtu::vector<Log>* LogSystem::QueryLog(UserName username) {
  sjtu::vector<Log> *ret;

  try {
    ret = log_database.Find(username);
  } catch (Exception *error) {
    delete error;
    ret = new sjtu::vector<Log>;
  }

  return ret;
}

void LogSystem::Output(std::ostream& output, sjtu::vector<Log>* list) {
  output << list->size() << std::endl;

  for (int i = 0; i < list->size(); i++) {
    output << "[" << mapping[list->at(i).status] << "] "
           << list->at(i).train_id.train_id << " " << list->at(i).start_pos
           << " " << list->at(i).start_time << " -> " << list->at(i).end_pos
           << " " << list->at(i).end_time << " " << list->at(i).price << " "
           << list->at(i).number << std::endl;
  }

  return;
}

Log LogSystem::QueryLog(UserName username, int number) {
  sjtu::vector<Log> *ret = QueryLog(username);

  if (ret->size() < number) {
    delete ret;
    throw new NoSuchLog;
  }

  Log log = ret->at(number - 1);
  delete ret;

  return log;
}

void LogSystem::Update(UserName username, int number, Status new_status) {
  Log log = QueryLog(username, number);

  log_database.Delete(username, log);
  
  log.status = new_status;

  log_database.Insert(username, log);
}

bool LogSystem::Update(UserName username, int time_stamp_, Status new_status, bool flag) {
  sjtu::vector<Log> *ret = log_database.Find(username);

  for (int i = 0; i < ret->size(); i++) {
    if (ret->at(i).time_stamp_ == time_stamp_) {
      log_database.Delete(username, ret->at(i));
      ret->at(i).status = new_status;
      log_database.Insert(username, ret->at(i));
      delete ret;
      return true;
    }
  }
  delete ret;
  return false;
}