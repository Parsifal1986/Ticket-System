#ifndef SELLING_SYSTEM_HPP
#define SELLING_SYSTEM_HPP

#include "../bp_tree/bp_tree.hpp"
#include "../exceptions/exceptions.hpp"
#include "../map/map.hpp"
#include "../time_system/time_system.hpp"
#include "../vector/vector.hpp"
#include "../train_manager/train_manager.hpp"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

extern TrainDatabase train_database;

class SellingSystem {
public:
  struct TrainsOfDay {
    Time day;
    TrainID train_id;

    bool operator<(TrainsOfDay &other) {
      return train_id == other.train_id ? day < other.day : train_id < other.train_id;
    }

    bool operator==(TrainsOfDay &other) {
      return day == other.day && train_id == other.train_id;
    }
  };

  struct SoldSeat {
    int sold_seat[100] = {0};

    bool operator<(SoldSeat &other) {
      return false;
    }

    bool operator==(SoldSeat &other) {
      return false;
    }
  };

  struct Place {
    Place() {
      
    }

    Place(const char *place_) {
      std::strcpy(place, place_);
    }

    char place[31];

    bool operator<(Place &other) { return (std::strcmp(place, other.place) < 0); }

    bool operator==(Place &other) { return (std::strcmp(place, other.place) == 0); }
  };

  struct TrainInfo {
    TrainID train_id;
    int position;

    bool operator<(TrainInfo &other) { return train_id < other.train_id; }

    bool operator==(TrainInfo &other) { return train_id == other.train_id; }
  };

  struct Ticket {
    TrainData train;
    int start_pos, end_pos;
  };

  static bool cmp1(const Ticket &lhs, const Ticket &rhs){
      return lhs.train.price[lhs.end_pos - 1] - lhs.train.price[lhs.start_pos] <
             rhs.train.price[rhs.end_pos - 1] - rhs.train.price[rhs.start_pos];
  }

  static bool cmp2(const Ticket &lhs, const Ticket &rhs) {
      return lhs.train.travel_time[lhs.end_pos - 1] - lhs.train.travel_time[lhs.start_pos] +
             lhs.train.stopover_time[lhs.end_pos - 1] - lhs.train.stopover_time[lhs.start_pos] <
             rhs.train.travel_time[rhs.end_pos - 1] - rhs.train.travel_time[rhs.start_pos] +
             rhs.train.stopover_time[rhs.end_pos - 1] - rhs.train.stopover_time[rhs.start_pos];
  }

  void sort(sjtu::vector<Ticket>*& a, bool (*cmp)(const Ticket&, const Ticket&));

  SellingSystem();

  ~SellingSystem() = default;

  void ReleaseTrain(TrainID train_id);

  sjtu::vector<Ticket>* QueryTicket(Time start_day, std::string start_place, std::string end_place, bool type);

  void BuyTicket();

private:
  BpTree<TrainsOfDay, SoldSeat> release_train;
  BpTree<Place, TrainInfo> train_info_of_places;
};

#endif //SELLING_SYSTEM_HPP