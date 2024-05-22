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
#include <string>

extern TrainDatabase train_database;
extern UserDatabase user_database;
extern LogSystem log_system;

extern int time_stamp;

class SellingSystem {
public:
  struct TrainsOfDay {
    Time day;
    TrainID train_id;

    bool operator<(const TrainsOfDay &other) const {
      return train_id == other.train_id ? day < other.day : train_id < other.train_id;
    }

    bool operator==(const TrainsOfDay &other) const {
      return day == other.day && train_id == other.train_id;
    }

    struct Comp {
      bool operator()(const TrainsOfDay &lhs, const TrainsOfDay &rhs) {
        return lhs < rhs;
      }
    };
  };

  struct SoldSeat {
    SoldSeat() = default;

    SoldSeat(int all_seat_) : all_seat(all_seat_ ){
    }

    ~SoldSeat() = default;

    int sold_seat[100] = {0};
    int all_seat;

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
    TrainID train_id; 
    int price;
    Time start_time, end_time;
    int max_ticket;
    char start_place[31], end_place[31];

    Ticket() = default;

    ~Ticket() = default;

    Ticket(TrainID &train_id_, int price_, Time &start_time_, Time &end_time_, int &max_ticket_, std::string &start_place_, std::string &end_place_) :train_id(train_id_), price(price_), start_time(start_time_), end_time(end_time_), max_ticket(max_ticket_) {
      std::strcpy(start_place, start_place_.c_str());
      std::strcpy(end_place, end_place_.c_str());
    }
  };

  struct Candidate {
    UserName user_name;
    int time_stamp_of_request, start_pos, end_pos, number;

    bool operator<(const Candidate& other) const {
      return time_stamp_of_request < other.time_stamp_of_request;
    }

    bool operator==(const Candidate& other) const {
      return time_stamp_of_request == other.time_stamp_of_request;
    }
  };

  static bool cmp1(const Ticket &lhs, const Ticket &rhs) { return lhs.price < rhs.price; }

  static bool cmp2(const Ticket &lhs, const Ticket &rhs) {
      return lhs.end_time - lhs.start_time < rhs.end_time - rhs.start_time;
  }

  void sort(sjtu::vector<Ticket>*& a, bool (*cmp)(const Ticket&, const Ticket&));

  SellingSystem();

  ~SellingSystem();

  void ReleaseTrain(TrainID train_id);

  sjtu::vector<Ticket>* QueryTicket(Time start_day, std::string start_place, std::string end_place, bool type);

  int BuyTicket(std::string username, TrainID train_id, Time start_day, std::string start_place, std::string end_place, int number, bool candidate);

  bool RefundTicket(std::string username, int n);

  sjtu::vector<sjtu::pair<SellingSystem::Ticket, SellingSystem::Ticket>>* QueryTransfer(std::string username, TrainID train_id, Time start_day, std::string start_place, std::string end_place, int number, bool candidate);

private:
  sjtu::map<TrainsOfDay, SoldSeat, TrainsOfDay::Comp> release_train_;
  BpTree<Place, TrainInfo> train_info_of_places_;
  BpTree<TrainsOfDay, Candidate> candidate_list_;
  MemoryRiver<sjtu::pair<TrainsOfDay, SoldSeat>, 1> memory_river_;
};

#endif //SELLING_SYSTEM_HPP