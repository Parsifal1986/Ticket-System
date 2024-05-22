#include "selling_system.hpp"
#include <algorithm>
#include <cstring>

SellingSystem::SellingSystem() : train_info_of_places_("train_info_of_places_data"), candidate_list_("candidate_info"), memory_river_(("release_train")) {
  int number;
  memory_river_.get_info(number, 1);
  sjtu::pair<TrainsOfDay, SoldSeat> tmp;
  for (int i = 1; i <= number; i++) {
    memory_river_.read(tmp, sizeof(int) + sizeof(tmp) * (i - 1));
    release_train_[tmp.first] = tmp.second;
  }
}

SellingSystem::~SellingSystem() {
  memory_river_.write_info(release_train_.size(), 1);

  int cnt = 0;
  while (!release_train_.empty()) {
    auto it = release_train_.begin();
    auto tmp = sjtu::pair<TrainsOfDay, SoldSeat>(it->first, it->second);
    memory_river_.write(tmp);
  }
}

void SellingSystem::ReleaseTrain(TrainID train_id) {
  TrainData *train_data = train_database.FindTrain(train_id);

  for (Time i = train_data->start_sale_date; i < train_data->end_sale_date;
       i.AddTime(0, 1, 0, 0)) {
    release_train_[TrainsOfDay{i, train_id}] = SoldSeat();
  }

  for (int i = 0; i < train_data->station_num; i++) {
    train_info_of_places_.Insert(Place(train_data->place[i]),
                                TrainInfo{train_id, i});
  }
  delete train_data;
  return;
}

sjtu::vector<SellingSystem::Ticket> *
SellingSystem::QueryTicket(Time start_day, std::string start_place, std::string end_place, bool type) {
  sjtu::vector<TrainInfo> *ret1 = train_info_of_places_.Find(Place(start_place.c_str()));

  sjtu::vector<Ticket> *ret = new sjtu::vector<Ticket>;

  for (int i = 0; i < ret1->size(); i++) {
    TrainData *train = train_database.FindTrain(ret1->at(i).train_id);
    auto it = release_train_.find(TrainsOfDay{start_day, ret1->at(i).train_id});
    if (it == release_train_.end()) {
      continue;
    }
    SoldSeat &sold_seat = it->second;
    int position = ret1->at(i).position;
    int total_spent_time_to_the_start_pos = (position >= 1 ? train->travel_time[position - 1] + train->stopover_time[position - 1] : 0);
    if (train->start_sale_date.AddTime_tmp(0, 0, train->start_time.hour,
      train->start_time.minute + total_spent_time_to_the_start_pos).LessInDate(start_day) &&
        start_day.LessInDate(train->end_sale_date.AddTime_tmp(0, 0, train->start_time.hour,
            train->start_time.minute + total_spent_time_to_the_start_pos))) {
      int max_ticket_ = -1;
      for (int j = position + 1; j < train->station_num; j++) {
        max_ticket_ = std::max(max_ticket_, train->seat_num - sold_seat.sold_seat[j - 1]);
        if (std::strcmp(train->place[j], end_place.c_str()) == 0){
          int total_spent_time_to_the_end_pos = (j >= 2 ? train->travel_time[j - 1] + train->stopover_time[j - 2] : train->travel_time[j - 1]);
          ret->push_back(Ticket(ret1->at(i).train_id, train->price[j] - train->price[position], train->start_sale_date.AddTime_tmp(0, 0, train->start_time.hour, train->start_time.minute + total_spent_time_to_the_start_pos), train->start_sale_date.AddTime_tmp(0, 0, train->start_time.hour, train->start_time.minute + total_spent_time_to_the_end_pos), max_ticket_, start_place, end_place));
        }
      }
    } else {
      delete train;
    }
  }

  // if (type == false) {
  //   sort(ret, cmp1);
  // } else {
  //   sort(ret, cmp2);
  // }
  return ret;
}

int SellingSystem::BuyTicket(std::string username, TrainID train_id, Time start_day, std::string start_place, std::string end_place, int number, bool candidate) {
  int ret, start_p = -1, end_p = -1;

  bool enough_ticket = true;

  TrainData *train = train_database.FindTrain(train_id);

  for (int i = 0; i < train->station_num; i++) {
    if (std::strcmp(train->place[i], start_place.c_str()) == 0) {
      start_p = i;
      break;
    }
  }

  if (start_p == -1) {
    return -1;
  }

  int total_spent_time_to_the_start_pos = (start_p >= 1 ? train->travel_time[start_p - 1] + train->stopover_time[start_p - 1] : 0);
  int total_spent_time_to_the_end_pos = (end_p >= 2 ? train->travel_time[end_p - 1] + train->stopover_time[end_p - 2] : train->travel_time[end_p - 1]);
  Time spent_on_way = train->start_time;
  spent_on_way.AddTime(0, 0, 0, total_spent_time_to_the_start_pos);
  start_day.MinusTime(0, 0, spent_on_way.hour, spent_on_way.minute);

  auto it = release_train_.find(TrainsOfDay{start_day, train_id});
  if (it == release_train_.end()) {
    return -1;
  }

  SoldSeat &train_seat = it->second;

  for (int i = start_p + 1; i < train->station_num; i++) {
    if (train_seat.sold_seat[i] + number > train->seat_num) {
      enough_ticket = false;
    }
    if (std::strcmp(train->place[i], end_place.c_str()) == 0) {
      end_p = i;
      break;
    }
  }


  if (!enough_ticket) {
    if (candidate) {
      candidate_list_.Insert(TrainsOfDay{start_day, train_id}, Candidate{username, time_stamp, start_p, end_p, number});
      log_system.AddLog(username, train_id, start_place, end_place, start_day.AddTime_tmp(0, 0, 0, total_spent_time_to_the_start_pos), start_day.AddTime_tmp(0, 0, 0, total_spent_time_to_the_end_pos), train->price[end_p] - train->price[start_p], number, 2, start_p, end_p);
      return -2;
    } else {
      return -1;
    }
  }

  if (end_p == -1) {
    return -1;
  }

  for (int i = start_p + 1; i < end_p; i++) {
    train_seat.sold_seat[i] += number;
  }
  log_system.AddLog(username, train_id, start_place, end_place, start_day.AddTime_tmp(0, 0, 0, total_spent_time_to_the_start_pos), start_day.AddTime_tmp(0, 0, 0, total_spent_time_to_the_end_pos), train->price[end_p] - train->price[start_p], number, 1, start_p, end_p);
  return train->price[end_p] - train->price[start_p];
}

bool SellingSystem::RefundTicket(std::string username, int n) {
  Log log = log_system.QueryLog(username, n);

  if (log.status != 1) {
    return false;
  }

  auto it = release_train_.find(TrainsOfDay{log.start_time, log.train_id});
  SoldSeat &train_seat = it->second;

  for (int i = log.start_p; i < log.end_p; i++) {
    train_seat.sold_seat[i] -= log.number;
  }

  sjtu::vector<Candidate> *ret = candidate_list_.Find(TrainsOfDay{log.start_time, log.train_id});

  for (int i = 0; i < ret->size(); i++) {
    bool enough_seat = true;
    if (ret->at(i).end_pos <= log.start_p || ret->at(i).start_pos >= log.end_p) {
      continue;
    }
    for (int j = ret->at(i).start_pos; j < ret->at(i).end_pos; j++) {
      if (train_seat.sold_seat[j] + ret->at(i).number > train_seat.all_seat) {
        enough_seat = false;
        break;
      }
    }
    if (enough_seat == true) {
      for (int j = ret->at(i).start_pos; j < ret->at(i).end_pos; j++) {
        train_seat.sold_seat[j] += ret->at(i).number;
      }
      log_system.Update(username, ret->at(i).time_stamp_of_request, 1, true);
    }
  }
  return true;
}

sjtu::vector<sjtu::pair<SellingSystem::Ticket, SellingSystem::Ticket>>* SellingSystem::QueryTransfer(std::string username, TrainID train_id, Time start_day, std::string start_place, std::string end_place, int number, bool candidate) {
  sjtu::vector<sjtu::pair<SellingSystem::Ticket, SellingSystem::Ticket>> *ret;
  
  return ret;
}