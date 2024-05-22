#include "log_system.hpp"

LogSystem::LogSystem() : log_database("log_data") {

}

void LogSystem::AddLog(UserName username, TrainID& train_id, std::string& start_pos, std::string& end_pos, Time& start_time,
       Time& end_time, int price, int number, short status, short start_p, short end_p) {
  log_database.Insert(username, Log(train_id, start_pos, end_pos, start_time,
                                    end_time, price, number, status, start_p, end_p));
}

sjtu::vector<Log>* LogSystem::QueryLog(UserName username) {
  sjtu::vector<Log> *ret;

  try {
    ret = log_database.Find(username);
  } catch (Exception *error) {
    delete error;
    throw new NoSuchLog();
  }

  return ret;
}

Log LogSystem::QueryLog(UserName username, int number) {
  sjtu::vector<Log> *ret = QueryLog(username);

  Log log = ret->at(number - 1);
  delete ret;

  return log;
}

void LogSystem::Update(UserName username, int number, int new_status) {
  Log log = QueryLog(username, number);

  log_database.Delete(username, log);
  
  log.status = new_status;

  log_database.Insert(username, log);
}

bool LogSystem::Update(UserName username, int time_stamp_, int new_status, bool flag) {
  sjtu::vector<Log> *ret = QueryLog(username);

  for (int i = 0; i < ret->size(); i++) {
    if (ret->at(i).time_stamp == time_stamp_) {
      log_database.Delete(username, ret->at(i));
      ret->at(i).status = new_status;
      log_database.Insert(username, ret->at(i));
      return true;;
    }
  }
  return false;
}