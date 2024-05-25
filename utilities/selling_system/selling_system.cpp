#include "selling_system.hpp"
#include <algorithm>
#include <cstring>

std::ostream& operator<<(std::ostream& output, Ticket& object) {
  output << object.train_id.train_id << " " << object.start_place
         << " " << object.start_time << " -> " << object.end_place
         << " " << object.end_time << " " << object.price << " "
         << object.max_ticket;
  return output;
}

SellingSystem::SellingSystem() : release_train_("release_train_index"), train_info_of_places_("train_info_of_places_data"), candidate_list_("candidate_info"), seat_info_("seat_information") {
}

SellingSystem::~SellingSystem() {

}

void SellingSystem::ReleaseTrain(TrainID train_id) {
  TrainData *train_data = train_database.FindTrain(train_id);

  if (QueryRelease(train_id, Time(0, 0, 0, 0))) {
    delete train_data;
    throw new TrainHasReleased();
  }

  SoldSeat tmp(train_data->seat_num);
  for (Time i = train_data->start_sale_date; i <= train_data->end_sale_date; i.AddTime(0, 1, 0, 0)) {
    int index = seat_info_.write(tmp);
    release_train_.Insert(TrainsOfDay{i, train_id}, index);
  }

  for (int i = 0; i < train_data->station_num; i++) {
    train_info_of_places_.Insert(Place(train_data->place[i]), TrainInfo{train_id, i});
  }
  delete train_data;
  return;
}

sjtu::vector<Ticket> *
SellingSystem::QueryTicket(Time start_day, std::string start_place, std::string end_place, int type) {
  sjtu::vector<TrainInfo> *ret1;
  try {
    ret1 = train_info_of_places_.Find(Place(start_place.c_str()));
  } catch (Exception *error) {
    delete error;
    ret1 = new sjtu::vector<TrainInfo>;
  }
  sjtu::vector<TrainInfo> *ret2;
  try {
    ret2 = train_info_of_places_.Find(Place(end_place.c_str()));
  } catch (Exception *error) {
    delete error;
    ret2 = new sjtu::vector<TrainInfo>;
  }

  sjtu::vector<Ticket> *ret = new sjtu::vector<Ticket>;

  int i = 0, j = 0;
  while (i < ret1->size() &&  j < ret2->size()) {
    if (ret1->at(i).train_id < ret2->at(j).train_id) {
      ++i;
      continue;
    } else if (ret2->at(j).train_id < ret1->at(i).train_id) {
      ++j;
      continue;
    }
    if (ret1->at(i).position >= ret2->at(j).position) {
      ++i; ++j;
      continue;
    }
    TrainData *train = train_database.FindTrain(ret1->at(i).train_id);
    int position = ret1->at(i).position;
    int total_spent_time_to_the_start_pos = train->travel_time[position] + train->stopover_time[position];
    Time spent_on_way = train->start_time, original_start_day(start_day.month, start_day.day, 0, 0), original_start_time;
    spent_on_way.AddTime(0, 0, 0, total_spent_time_to_the_start_pos);
    original_start_day.MinusTime(0, spent_on_way.day, 0, 0);
    if (type == 2 || type == 3) {
      if (original_start_day < train->start_sale_date) {
        original_start_day = train->start_sale_date;
      }
    }
    original_start_time = original_start_day + train->start_time;
    if (original_start_time.AddTime_tmp(0, 0, 0, total_spent_time_to_the_start_pos) < start_day) {
      original_start_day.AddTime(0, 1, 0, 0);
      original_start_time.AddTime(0, 1, 0, 0);
    }
    sjtu::vector<int>* it;
    try {
      it = release_train_.Find(TrainsOfDay{original_start_day, ret1->at(i).train_id});
    } catch (Exception *error) {
      delete error;
      delete train;
      ++i; ++j;
      continue;
    }
    SoldSeat sold_seat;
    seat_info_.read(sold_seat, it->at(0));
    int max_ticket_ = 0x7fffffff;
    for (int k = position + 1; k <= ret2->at(j).position; k++) {
      max_ticket_ = std::min(max_ticket_, train->seat_num - sold_seat.sold_seat[k - 1]);
    }
    int total_spent_time_to_the_end_pos = train->travel_time[ret2->at(j).position] + train->stopover_time[ret2->at(j).position - 1];
    ret->push_back(Ticket(ret1->at(i).train_id, train->price[ret2->at(j).position] - train->price[position], original_start_time.AddTime_tmp(0, 0, 0, total_spent_time_to_the_start_pos), original_start_time.AddTime_tmp(0, 0, 0, total_spent_time_to_the_end_pos), max_ticket_, start_place, end_place));
    delete train;
    delete it;
    ++i; ++j;
  }

  if (type == 0 || type == 3) {
    QuickSort(*ret, 0, ret->size() - 1, [](const Ticket& lhs, const Ticket &rhs) { return lhs.price == rhs.price ? lhs.train_id < rhs.train_id : lhs.price < rhs.price; });
  } else if (type == 1) {
    QuickSort(*ret, 0, ret->size() - 1, [](const Ticket& lhs, const Ticket &rhs) { return lhs.end_time - lhs.start_time == rhs.end_time - rhs.start_time ? lhs.train_id < rhs.train_id : lhs.end_time - lhs.start_time < rhs.end_time - rhs.start_time; });
  } else {
    QuickSort(*ret, 0, ret->size() - 1, [](const Ticket& lhs, const Ticket &rhs) { return lhs.end_time == rhs.end_time ? lhs.train_id < rhs.train_id : lhs.end_time < rhs.end_time; });
  }

  delete ret1;
  delete ret2;
  return ret;
}

bool SellingSystem::QueryRelease(TrainID train_id, Time day) {
  try {
    auto ret = release_train_.Find(TrainsOfDay{day, train_id});
    delete ret;
    return true;
  } catch (Exception *error) {
    delete error;
    return false;
  }
}

void SellingSystem::QuerySeat(TrainID train_id, Time day, SoldSeat& sold_seat) {
  auto ret = release_train_.Find(TrainsOfDay{day, train_id});
  int tmp = ret->at(0);
  delete ret;
  seat_info_.read(sold_seat, tmp);
  return;
}

int SellingSystem::BuyTicket(UserName username, TrainID train_id, Time start_day, std::string start_place, std::string end_place, int number, bool candidate) {
  int ret, start_p = -1, end_p = -1;

  bool enough_ticket = true;

  TrainData *train = train_database.FindTrain(train_id);

  if (number > train->seat_num) {
    delete train;
    return -1;
  }

  for (int i = 0; i < train->station_num; i++) {
    if (std::strcmp(train->place[i], start_place.c_str()) == 0) {
      start_p = i;
      break;
    }
  }

  if (start_p == -1) {
    delete train;
    return -1;
  }

  int total_spent_time_to_the_start_pos = train->travel_time[start_p] + train->stopover_time[start_p];
  Time spent_on_way = train->start_time, original_start_day = start_day, original_start_time;
  spent_on_way.AddTime(0, 0, 0, total_spent_time_to_the_start_pos);
  original_start_day.MinusTime(0, spent_on_way.day, 0, 0);
  original_start_time = original_start_day + train->start_time;

  sjtu::vector<int> *it;
  try {
    it = release_train_.Find(TrainsOfDay{original_start_day, train_id});
  } catch (Exception *error) {
    delete error;
    delete train;
    return -1;
  }

  SoldSeat train_seat;
  seat_info_.read(train_seat, it->at(0));

  for (int i = start_p + 1; i < train->station_num; i++) {
    if (train_seat.sold_seat[i - 1] + number > train->seat_num) {
      enough_ticket = false;
    }
    if (std::strcmp(train->place[i], end_place.c_str()) == 0) {
      end_p = i;
      break;
    }
  }

  if (end_p == -1) {
    delete train;
    delete it;
    return -1;
  }

  int total_spent_time_to_the_end_pos = train->travel_time[end_p] + train->stopover_time[end_p - 1];

  if (!enough_ticket) {
    if (candidate) {
      candidate_list_.Insert(TrainsOfDay{original_start_day, train_id}, Candidate{username, time_stamp, start_p, end_p, number});
      log_system.AddLog(username, train_id, start_place, end_place, original_start_time.AddTime_tmp(0, 0, 0, total_spent_time_to_the_start_pos), original_start_time.AddTime_tmp(0, 0, 0, total_spent_time_to_the_end_pos), original_start_day, train->price[end_p] - train->price[start_p], number, PENDING, start_p, end_p);
      delete train;
      delete it;
      return -2;
    } else {
      delete train;
      delete it;
      return -1;
    }
  }

  for (int i = start_p; i < end_p; i++) {
    train_seat.sold_seat[i] += number;
  }
  seat_info_.update(train_seat, it->at(0));
  log_system.AddLog(username, train_id, start_place, end_place, original_start_time.AddTime_tmp(0, 0, 0, total_spent_time_to_the_start_pos), original_start_time.AddTime_tmp(0, 0, 0, total_spent_time_to_the_end_pos), original_start_day, train->price[end_p] - train->price[start_p], number, SUCCESS, start_p, end_p);
  int tmp = (train->price[end_p] - train->price[start_p]) * number;
  delete train;
  delete it;
  return tmp;
}

bool SellingSystem::RefundTicket(UserName username, int n) {
  Log log;
  try {
    log = log_system.QueryLog(username, n);
  } catch (Exception *error) {
    delete error;
    return false;
  }

  if (log.status == REFUNDED) {
    return false;
  }

  if (log.status == PENDING) {
    log_system.Update(username, n, REFUNDED);
    sjtu::vector<Candidate> *ret = candidate_list_.Find(TrainsOfDay{log.start_date, log.train_id});

    for (int i = 0; i < ret->size(); i++) {
      if (ret->at(i).user_name == username) {
        candidate_list_.Delete(TrainsOfDay{log.start_date, log.train_id}, ret->at(i));
        break;
      }
    }
    delete ret;
    return true;
  }

  sjtu::vector<int> *it;
  it = release_train_.Find(TrainsOfDay{log.start_date, log.train_id});
  SoldSeat train_seat;
  seat_info_.read(train_seat, it->at(0));

  for (int i = log.start_p; i < log.end_p; i++) {
    train_seat.sold_seat[i] -= log.number;
  }

  log_system.Update(username, n, REFUNDED);

  sjtu::vector<Candidate> *ret;
  try {
     ret = candidate_list_.Find(TrainsOfDay{log.start_date, log.train_id});
  } catch (Exception *error) {
    delete error;
    seat_info_.update(train_seat, it->at(0));
    delete it;
    return true;
  }

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
      candidate_list_.Delete(TrainsOfDay{log.start_date, log.train_id}, ret->at(i));
      log_system.Update(ret->at(i).user_name, ret->at(i).time_stamp_of_request, SUCCESS, true);
    }
  }
  seat_info_.update(train_seat, it->at(0));
  delete it;
  delete ret;
  return true;
}

void SellingSystem::Output(std::ostream &output, sjtu::vector<Ticket> *object) {
  output << object->size() << std::endl;

  for (int i = 0; i < object->size(); i++) {
    output << object->at(i) << std::endl;
  }
  return;
}

void SellingSystem::Output(std::ostream &output, sjtu::pair<Ticket, Ticket> *object) {
  if (object == nullptr) {
    std::cout << 0 << std::endl;
    return;
  } else {
    output << object->first << std::endl << object->second << std::endl;
  }
  return;
}

sjtu::pair<Ticket, Ticket>* SellingSystem::QueryTransfer(Time start_day, std::string start_place, std::string end_place, bool type) {
  sjtu::pair<Ticket, Ticket> *ret = nullptr;
  sjtu::vector<TrainInfo> *start_train;
  try {
    start_train = train_info_of_places_.Find(Place(start_place.c_str()));
  } catch (Exception *error) {
    delete error;
    start_train = new sjtu::vector<TrainInfo>;
  }

  int min_price = 0x7fffffff;
  Time min_time(0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff);

  for (int i = 0; i < start_train->size(); i++) {
    TrainData *train = train_database.FindTrain(start_train->at(i).train_id);
    int position = start_train->at(i).position;
    int total_spent_time_to_the_start_pos = train->travel_time[position] + train->stopover_time[position];
    Time spent_on_way = train->start_time, original_start_day = start_day, original_start_time;
    spent_on_way.AddTime(0, 0, 0, total_spent_time_to_the_start_pos);
    original_start_day.MinusTime(0, spent_on_way.day, 0, 0);
    original_start_time = original_start_day + train->start_time;
    sjtu::vector<int> *it;
    try {
      it = release_train_.Find(TrainsOfDay{original_start_day, start_train->at(i).train_id});
    } catch (Exception *error) {
      delete error;
      delete train;
      continue;
    }
    SoldSeat sold_seat;
    seat_info_.read(sold_seat, it->at(0));
    int max_ticket_ = 0x7fffffff;
    for (int j = position + 1; j < train->station_num; j++) {
      max_ticket_ = std::min(max_ticket_, train->seat_num - sold_seat.sold_seat[j - 1]);
      int total_spent_time_to_the_transfer_pos = train->travel_time[j] + train->stopover_time[j - 1];
      sjtu::vector<Ticket> *transfer_train = QueryTicket(original_start_time.AddTime_tmp(0, 0, 0, total_spent_time_to_the_transfer_pos), train->place[j], end_place, (type == true ? 2 : 3));
      int x = 0;
      if (transfer_train->empty()) {
        delete transfer_train;
        continue;
      }
      if (transfer_train->at(x).train_id == train->train_id) {
        ++x;
      }
      if (x >= transfer_train->size()) {
        delete transfer_train;
        continue;
      }
      std::string transfer_pos = train->place[j];
      if (type) {
        if (transfer_train->at(x).end_time - original_start_time.AddTime_tmp(0, 0, 0, total_spent_time_to_the_start_pos) == min_time ? (transfer_train->at(x).price + train->price[j] - train->price[position] == ret->first.price + ret->second.price ? (train->train_id == ret->first.train_id ? transfer_train->at(x).train_id < ret->second.train_id : train->train_id < ret->first.train_id) : transfer_train->at(x).price + train->price[j] - train->price[position] < ret->first.price + ret->second.price) : transfer_train->at(x).end_time - original_start_time.AddTime_tmp(0, 0, 0, total_spent_time_to_the_start_pos) < min_time) {
          if (ret != nullptr) {
            delete ret;
          }
          min_time = transfer_train->at(x).end_time - original_start_time.AddTime_tmp(0, 0, 0, total_spent_time_to_the_start_pos);
          ret = new sjtu::pair<Ticket, Ticket> (Ticket(train->train_id, train->price[j] - train->price[position], original_start_time.AddTime_tmp(0, 0, 0, total_spent_time_to_the_start_pos), original_start_time.AddTime_tmp(0, 0, 0, total_spent_time_to_the_transfer_pos), max_ticket_, start_place, transfer_pos), transfer_train->at(x));
          }
      } else {
        if (transfer_train->at(x).price + train->price[j] - train->price[position] == min_price ? (transfer_train->at(x).end_time - original_start_time.AddTime_tmp(0, 0, 0, total_spent_time_to_the_start_pos) == ret->second.end_time - ret->first.start_time ? (train->train_id == ret->first.train_id ? transfer_train->at(x).train_id < ret->second.train_id : train->train_id < ret->first.train_id):transfer_train->at(x).end_time - original_start_time.AddTime_tmp(0, 0, 0, total_spent_time_to_the_start_pos) < ret->second.end_time - ret->first.start_time) : transfer_train->at(x).price + train->price[j] - train->price[position] < min_price) {
          if (ret != nullptr) {
            delete ret;
          }
          min_price = transfer_train->at(x).price + train->price[j] - train->price[position];
          ret = new sjtu::pair<Ticket, Ticket> (Ticket(train->train_id, train->price[j] - train->price[position], original_start_time.AddTime_tmp(0, 0, 0, total_spent_time_to_the_start_pos), original_start_time.AddTime_tmp(0, 0, 0, total_spent_time_to_the_transfer_pos), max_ticket_, start_place, transfer_pos), transfer_train->at(x));
        }
      }
      delete transfer_train;
    }
    delete train;
    delete it;
  }
  delete start_train;
  return ret;
}