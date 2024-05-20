#include "selling_system.hpp"

SellingSystem::SellingSystem()
    : release_train("release_train_data"),
      train_info_of_places("train_info_of_places_data") {}

void SellingSystem::ReleaseTrain(TrainID train_id) {
  TrainData *train_data = train_database.FindTrain(train_id);

  for (Time i = train_data->start_sale_date; i < train_data->end_sale_date;
       i.AddTime(0, 1, 0, 0)) {
    release_train.Insert(TrainsOfDay{i, train_id}, SoldSeat());
  }

  for (int i = 0; i < train_data->station_num; i++) {
    train_info_of_places.Insert(Place(train_data->place[i]),
                                TrainInfo{train_id, i});
  }
  delete train_data;
  return;
}

sjtu::vector<SellingSystem::Ticket> *
SellingSystem::QueryTicket(Time start_day, std::string start_place,
                           std::string end_place, bool type) {
  sjtu::vector<TrainInfo> *ret1 =
      train_info_of_places.Find(Place(start_place.c_str()));
  sjtu::vector<TrainInfo> *ret2 =
      train_info_of_places.Find(Place(end_place.c_str()));

  sjtu::vector<Ticket> *ret = new sjtu::vector<Ticket>;

  sjtu::map<TrainData *, int> check_list;

  for (int i = 0; i < ret1->size(); i++) {
    TrainData *train = train_database.FindTrain(ret1->at(i).train_id);
    int position = ret1->at(i).position, total_spent_time;
    total_spent_time = (position >= 1 ? train->travel_time[position - 1] +
                                            train->stopover_time[position - 1]
                                      : 0);
    if (train->start_sale_date
            .AddTime(0, 0, train->start_time.hour,
                     train->start_time.minute + total_spent_time)
            .LessInDate(start_day) &&
        start_day.LessInDate(train->end_sale_date.AddTime(
            0, 0, train->start_time.hour,
            train->start_time.minute + total_spent_time))) {
      check_list[train] = position;
    } else {
      delete train;
    }
  }

  for (int i = 0; i < ret2->size(); i++) {
    TrainData *train = train_database.FindTrain(ret2->at(i).train_id);
    auto it = check_list.find(train);

    if (it != check_list.end()) {
      if (it->second <= ret2->at(i).position) {
        ret->push_back(Ticket{*train, it->second, ret2->at(i).position});
      }
    }
    delete train;
  }

  while (!check_list.empty()) {
    delete check_list.begin()->first;
    check_list.erase(check_list.begin());
  }

  // if (type == false) {
  //   sort(ret, cmp1);
  // } else {
  //   sort(ret, cmp2);
  // }
  return ret;
}