#include "train_manager.hpp"

TrainDatabase::TrainDatabase() : train_database_index_("train_data_index"), train_database_("train_data") {}

TrainData* TrainDatabase::FindTrain(TrainID train_id) {
  sjtu::vector<int> *ret;

  try {
    ret = train_database_index_.Find(train_id);
  } catch (Exception *error) {
    delete error;
    throw new NoSuchTrain();
  }

  TrainData* train_data = new TrainData();
  train_database_.read(*train_data, ret->at(0));
  delete ret;
  return train_data;
}

void TrainDatabase::DeleteTrain(TrainID train_id) {
  sjtu::vector<int> *ret;

  try {
    ret = train_database_index_.Find(train_id);
  } catch (Exception *error) {
    delete error;
    throw new NoSuchTrain();
  }

  train_database_index_.Delete(train_id, ret->at(0));
  delete ret;
  return;
}

void TrainDatabase::AddTrain(TrainData& train_data) {
  try {
    auto ret = train_database_index_.Find(train_data.train_id);
    delete ret;
    throw new TrainExist();
  } catch (NothingFind *error) {
    delete error;
    int index = train_database_.write(train_data);
    train_database_index_.Insert(train_data.train_id, index);
  }
  return;
}