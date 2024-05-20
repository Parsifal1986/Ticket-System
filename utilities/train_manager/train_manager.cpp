#include "train_manager.hpp"

TrainDatabase::TrainDatabase() : train_database("train_data") {}

TrainData* TrainDatabase::FindTrain(TrainID train_id) {
  sjtu::vector<TrainData> *ret;

  try {
    ret = train_database.Find(train_id);
  } catch (Exception *error) {
    delete error;
    throw new NoSuchTrain();
  }

  TrainData* train_data = new TrainData(ret->at(0));
  delete ret;
  return train_data;
}

TrainData* TrainDatabase::DeleteTrain(TrainID train_id) {
  TrainData* train_data = FindTrain(train_id);

  train_database.Delete(train_id, *train_data);

  return train_data;
}

void TrainDatabase::AddTrain(TrainData* train_data) {
  train_database.Insert(train_data->train_id, *train_data);
  return;
}