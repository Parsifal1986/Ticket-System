#ifndef TRAIN_MANAGER_HPP
#define TRAIN_MANAGER_HPP

#include "../bp_tree/bp_tree.hpp"
#include "../exceptions/exceptions.hpp"
#include "../map/map.hpp"
#include "../vector/vector.hpp"
#include "../time_system/time_system.hpp"
#include "../memory_river/memory_river.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

struct TrainID {
  TrainID() {}

  TrainID(std::string train_id_) {
    std::strcpy(train_id, train_id_.c_str());
    return;
  }

  ~TrainID() {}

  bool Empty() const { return (train_id[0] == '\0'); }

  char train_id[21] = {'\0'};

  bool operator<(const TrainID &rhs) const {
    return (std::strcmp(train_id, rhs.train_id) < 0);
  }

  bool operator==(const TrainID &rhs) const {
    return !(std::strcmp(train_id, rhs.train_id));
  }

  TrainID& operator=(const TrainID &other) {
    if (this == &other) {
      return *this;
    }

    std::strcpy(train_id, other.train_id);
    
    return *this;
  }
};

struct TrainData {
  TrainID train_id;
  char type, place[100][31] = {0};
  short station_num, travel_time[99], stopover_time[99]; // 前缀和
  int seat_num, price[100] = {0};
  Time start_time, start_sale_date, end_sale_date;

  TrainData() {

  }

  TrainData(const TrainData &other) {
    if (this == &other) {
      return;
    }
    train_id = other.train_id;
    seat_num = other.seat_num;
    type = other.type;
    station_num = other.station_num;
    start_time = other.start_time;
    start_sale_date = other.start_sale_date;
    end_sale_date = other.end_sale_date;

    for (int i = 0; i < other.station_num; ++i) {
      std::strcpy(place[i], other.place[i]);
    }

    for (int i = 0; i < other.station_num - 1; ++i) {
      travel_time[i] = other.travel_time[i];
      price[i] = other.price[i];
    }

    for (int i = 0; i < other.station_num - 2; ++i) {
      stopover_time[i] = other.stopover_time[i];
    }
  };

  bool operator<(const TrainData &rhs) const { return train_id < rhs.train_id; }

  bool operator==(const TrainData &rhs) const {
    return train_id == rhs.train_id;
  } // The train_id is unique

  TrainData& operator=(const TrainData &other) {
    if (this == &other) {
      return *this;
    }
    train_id = other.train_id;
    seat_num = other.seat_num;
    type = other.type;
    station_num = other.station_num;
    start_time = other.start_time;
    start_sale_date = other.start_sale_date;
    end_sale_date = other.end_sale_date;

    for (int i = 0; i < other.station_num; ++i) {
      std::strcpy(place[i], other.place[i]);
    }

    for (int i = 0; i < other.station_num - 1; ++i) {
      travel_time[i] = other.travel_time[i];
      price[i] = other.price[i];
    }

    for (int i = 0; i < other.station_num - 2; ++i) {
      stopover_time[i] = other.stopover_time[i];
    }
    return *this;
  }
};

class TrainDatabase {
public:
  TrainDatabase();

  ~TrainDatabase() = default;

  TrainData* FindTrain(TrainID train_id);

  void DeleteTrain(TrainID train_id);

  void AddTrain(TrainData& train_data);

private:
  BpTree<TrainID, int> train_database_index_;
  MemoryRiver<TrainData> train_database_;
};

#endif // TRAIN_MANAGER_HPP