#ifndef SELLING_SYSTEM_HPP
#define SELLING_SYSTEM_HPP

#include "../bp_tree/bp_tree.hpp"
#include "../exceptions/exceptions.hpp"
#include "../map/map.hpp"
#include "../time_system/time_system.hpp"
#include "../vector/vector.hpp"
#include "../train_manager/train_manager.hpp"
#include "../user_manager/user_manager.hpp"
#include "../memory_river/memory_river.hpp"
#include "../log_system/log_system.hpp"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <ostream>
#include <string>

extern TrainDatabase train_database;
extern UserDatabase user_database;
extern LogSystem log_system;

extern int time_stamp;

struct TrainsOfDay {
  Time day;
  TrainID train_id;

  bool operator<(const TrainsOfDay &other) const {
    return train_id == other.train_id
               ? (day < other.day && !other.day.empty() && !day.empty())
               : train_id < other.train_id;
  }

  bool operator==(const TrainsOfDay &other) const {
    return (day == other.day || other.day.empty() || day.empty()) &&
           train_id == other.train_id;
  }

  struct Comp {
    bool operator()(const TrainsOfDay &lhs, const TrainsOfDay &rhs) {
      return lhs < rhs;
    }
  };
};

struct SoldSeat {
  SoldSeat() = default;

  SoldSeat(int all_seat_) : all_seat(all_seat_) {}

  ~SoldSeat() = default;

  int sold_seat[100] = {0};
  int all_seat;

  bool operator<(SoldSeat &other) { return false; }

  bool operator==(SoldSeat &other) { return false; }
};

struct Place {
  Place() {}

  Place(const char *place_) { std::strcpy(place, place_); }

  char place[31];

  bool operator<(Place &other) { return (std::strcmp(place, other.place) < 0); }

  bool operator==(Place &other) {
    return (std::strcmp(place, other.place) == 0);
  }
};

struct TrainInfo {
  TrainID train_id;
  int position;

  bool operator<(TrainInfo &other) { return train_id < other.train_id; }

  bool operator==(TrainInfo &other) { return train_id == other.train_id; }
};

struct Candidate {
  UserName user_name;
  int time_stamp_of_request, start_pos, end_pos, number;

  bool operator<(const Candidate &other) const {
    return time_stamp_of_request < other.time_stamp_of_request;
  }

  bool operator==(const Candidate &other) const {
    return time_stamp_of_request == other.time_stamp_of_request;
  }
};

struct Ticket {
  TrainID train_id;
  int price;
  Time start_time, end_time;
  int max_ticket;
  char start_place[31], end_place[31];

  Ticket() = default;

  ~Ticket() = default;

  Ticket(TrainID &train_id_, int price_, Time start_time_, Time end_time_,
         int &max_ticket_, std::string &start_place_, std::string &end_place_)
      : train_id(train_id_), price(price_), start_time(start_time_),
        end_time(end_time_), max_ticket(max_ticket_) {
    std::strcpy(start_place, start_place_.c_str());
    std::strcpy(end_place, end_place_.c_str());
  }

  Ticket(const Ticket& other) {
    if (this == &other) {
      return;
    }
    train_id = other.train_id; 
    price = other.price;
    start_time = other.start_time;
    end_time = other.end_time;
    max_ticket = other.max_ticket;
    std::strcpy(start_place, other.start_place);
    std::strcpy(end_place, other.end_place);
  }

  friend std::ostream& operator<<(std::ostream& output, Ticket& object);
};

class SellingSystem {
public:
  SellingSystem();

  ~SellingSystem();

  void ReleaseTrain(TrainID train_id);

  sjtu::vector<Ticket>* QueryTicket(Time start_day, std::string start_place, std::string end_place, int type);

  bool QueryRelease(TrainID train_id, Time day = Time());

  void QuerySeat(TrainID train_id, Time day, SoldSeat& sold_seat);

  int BuyTicket(UserName username, TrainID train_id, Time start_day, std::string start_place, std::string end_place, int number, bool candidate);

  bool RefundTicket(UserName username, int n);

  void Output(std::ostream& output, sjtu::vector<Ticket>* object);

  void Output(std::ostream &output, sjtu::pair<Ticket, Ticket> *object);

  sjtu::pair<Ticket, Ticket>* QueryTransfer(Time start_day, std::string start_place, std::string end_place, bool type);

private:
  sjtu::map<TrainsOfDay, int, TrainsOfDay::Comp> release_train_;
  BpTree<Place, TrainInfo> train_info_of_places_;
  BpTree<TrainsOfDay, Candidate> candidate_list_;
  MemoryRiver<sjtu::pair<TrainsOfDay, int>, 1> memory_river_;
  MemoryRiver<SoldSeat, 0> seat_info_;

  template<typename T, typename Compare>
  void QuickSort(sjtu::vector<T>& arr, int left, int right, Compare comp) {
      if (left < right) {
          int pivotIndex = partition(arr, left, right, comp);
          QuickSort(arr, left, pivotIndex - 1, comp);
          QuickSort(arr, pivotIndex + 1, right, comp);
      }
  }

  template<typename T, typename Compare>
  int partition(sjtu::vector<T>& arr, int left, int right, Compare comp) {
      T pivot = arr[right];
      int i = left - 1;
      for (int j = left; j < right; ++j) {
          if (comp(arr[j], pivot)) {
              ++i;
              std::swap(arr[i], arr[j]);
          }
      }
      std::swap(arr[i + 1], arr[right]);
      return i + 1;
  }

};

#endif //SELLING_SYSTEM_HPP