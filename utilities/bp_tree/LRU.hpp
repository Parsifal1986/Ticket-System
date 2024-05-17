#ifndef LRU_HPP
#define LRU_HPP

#include <cstdio>
#include <fstream>
#include <iostream>
#include <list>
#include <string>

// extern std::fstream file_;

template <class ValueType, size_t appoint_hash_size, size_t cache_size>
class LruCache {
  static const size_t kMod = appoint_hash_size;

private:
  struct ListNode {
    int index_;
    ValueType *data_;

    ListNode() {
      index_ = 0;
      data_ = nullptr;
    }

    ListNode(int index, ValueType *data) {
      index_ = index;
      data_ = data;
    }

    ~ListNode() { delete data_; }
  };

  std::list<ListNode *> datalist_;

  std::list<typename std::list<ListNode *>::iterator> hashmap_[kMod];

  std::hash<int> hashfunction_;

  int size_counter_ = 0;

  int appoint_hash_size_, cache_size_;

  std::string filename_;

  std::fstream file_;

  static const size_t kMax =
      (cache_size - sizeof(size_counter_) - sizeof(hashfunction_) -
       sizeof(hashmap_)) /
          (sizeof(ListNode *) +
           sizeof(typename std::list<ListNode *>::iterator) + sizeof(ListNode) +
           sizeof(ValueType)) -
      2;

  void Move_To_The_Top(typename std::list<ListNode *>::iterator it) {
    ListNode *tmp = *it;

    datalist_.erase(it);

    datalist_.insert(datalist_.begin(), tmp);
  }

  void RemoveBack() {
    auto node = datalist_.back();

    int key = hashfunction_(node->index_) % kMod;

    auto p = hashmap_[key].begin();

    for (; p != hashmap_[key].end(); p++) {
      if ((*(*p))->index_ == node->index_) {
        break;
      }
    }

    file_.seekp(node->index_, std::ios::beg);
    file_.write(reinterpret_cast<char *>(node->data_), sizeof(ValueType));

    hashmap_[key].erase(p);

    auto it = datalist_.end();
    --it;

    datalist_.erase(it);

    delete node;
  }

  void Insert(int index, ValueType *data) {
    if (size_counter_ == kMax) {
      RemoveBack();
      size_counter_--;
    }

    size_counter_++;

    int key = hashfunction_(index) % kMod;

    auto it = datalist_.insert(datalist_.begin(), new ListNode(index, data));

    hashmap_[key].insert(hashmap_[key].end(), it);
  }

public:
  LruCache() {
    appoint_hash_size_ = appoint_hash_size;
    cache_size_ = cache_size;
  };

  ~LruCache() {
    while (datalist_.size()) {
      auto it = datalist_.begin();

      file_.seekp((*it)->index_, std::ios::beg);
      file_.write(reinterpret_cast<char *>((*it)->data_), sizeof(ValueType));
      delete (*it);

      datalist_.erase(it);
    }
    file_.close();
  };

  ValueType *Find(int index) {
    int key = hashfunction_(index) % kMod;

    for (auto it = hashmap_[key].begin(); it != hashmap_[key].end(); it++) {
      ListNode *tmp = *(*it);
      if (tmp->index_ == index) {
        Move_To_The_Top(*it);
        *it = datalist_.begin();
        return tmp->data_;
      }
    }
    return nullptr;
  }

  void OpenFile(std::string filename = "bpdata") {
    filename_ = filename;
    if (std::ifstream(filename_).good()) {
      file_.open(filename_, std::ios::in | std::ios::out | std::ios::binary);
    } else {
      std::ofstream(filename_).close();
      file_.open(filename_, std::ios::in | std::ios::out | std::ios::binary);
    }
  }

  ValueType *Get(int index) {
    ValueType* ret = Find(index);
    if (ret != nullptr) {
      return ret;
    } else {
      ValueType *data = new ValueType;
      file_.seekg(index);
      size_t tmp = size_t(file_.tellg());
      file_.read(reinterpret_cast<char *>(data), sizeof(*data));
      Insert(index, data);
      return data;
    }
  }

  int Throw(int index, ValueType* data, std::ios_base::seekdir mode) {
    
    if (mode == std::ios::end) {
      file_.seekp(0, mode);
      size_t tmp = size_t(file_.tellp());
      file_.write(reinterpret_cast<char *>(data), sizeof(*data));
      Insert(tmp, data);
      return tmp;
    } else {
      if (Find(index) != nullptr) {
        return index;
      }
      file_.seekp(index, mode);
      file_.write(reinterpret_cast<char *>(data), sizeof(*data));
      Insert(index, data);
      return index;
    }
  }
};

#endif // LRU_HPP