#ifndef BP_TREE_HPP
#define BP_TREE_HPP

#include "../exceptions/exceptions.hpp"
#include "../vector/vector.hpp"
#include "LRU.hpp"
#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

// extern std::fstream file_;

template <class Key, class Data, int appoint_size = 0> class BpTree {

  static const int kSizeOfValue = sizeof(Key) + sizeof(Data);
  static const int kSizeOfBlock =
      appoint_size ? appoint_size : (4096 - 9) / (kSizeOfValue + 8) - 2;

private:
  struct ValueType { // Store the index and value in structure Node
    Key key_;
    Data value_;

    ValueType() = default;

    ValueType(Key key, Data value) {
      key_ = key;
      value_ = value;
    }

    ValueType(ValueType &other) {
      if (this == &other) {
        return;
      }
      key_ = other.key_;
      value_ = other.value_;
    }

    bool operator<(ValueType &rhs) {
      return (key_ == rhs.key_) ? value_ < rhs.value_ : key_ < rhs.key_;
    }

    ValueType operator=(ValueType &other) {
      if (this == &other) {
        return *this;
      }
      
      key_ = other.key_;
      value_ = other.value_;

      return *this;
    }

    bool operator==(ValueType &other) {
      return (key_ == other.key_ && value_ == other.value_);
    }
  };

  template <class First, class Second> // A simple pair class to make it more comvenient in insert & erase
  struct Pair {
    First first;
    Second second;

    Pair() = default;

    Pair(Pair &other) {
      first = other.first;
      second = other.second;
    }

    Pair(First _first, Second _second) {
      first = _first;
      second = _second;
    }

    ~Pair() = default;

    Pair &operator=(Pair &other) {
      if (&other == this) {
        return *this;
      }
      first = other.first;
      second = other.second;
      return *this;
    }

    Pair &operator=(Pair other) {
      first = other.first;
      second = other.second;
      return *this;
    }
  };

  struct Node {
    ValueType value_[kSizeOfBlock];
    size_t son_[kSizeOfBlock + 1];
    bool is_leaf_;
    int last_position_;

    Node() { // Default constructer
      last_position_ = 0;
      is_leaf_ = false;
      for (int i = 0; i <= kSizeOfBlock; i++) {
        son_[i] = -1;
      }
    }

    Node(Node &other) { // Default constructer
      if (this == &other) {
        return;
      }
      last_position_ = other.last_position_;
      is_leaf_ = other.is_leaf_;
      for (int i = 0; i < kSizeOfBlock; i++) {
        son_[i] = other.son_[i];
        value_[i] = other.value_[i];
      }
      son_[kSizeOfBlock] = other.son_[kSizeOfBlock];
    }

    Node &operator=(Node &other) {
      if (this == &other) {
        return *this;
      }
      is_leaf_ = other.is_leaf_;
      last_position_ = other.last_position_;
      for (int i = 0; i < last_position_; i++) {
        value_[i] = other.value_[i];
        son_[i] = other.son_[i];
      }
      son_[last_position_] = other.son_[other.last_position_];
      return *this;
    }

    inline int lower_bound(ValueType target) {
      int l = 0, count = last_position_, step;

      while (count != 0) {
        step = count / 2;

        if (value_[l + step] < target) {
          l = l + step + 1;
          count -= step + 1;
        } else {
          count = step;
        }
      }

      return l;
    }

    inline int upper_bound(ValueType target) {
      int l = 0, count = last_position_, step;

      while (count != 0) {
        step = count / 2;

        if (value_[l + step] < target || value_[l + step] == target) {
          l = l + step + 1;
          count -= step + 1;
        } else {
          count = step;
        }
      }

      return l;
    }

    inline int lower_bound(Key target) {
      int l = 0, count = last_position_, step;

      while (count != 0) {
        step = count / 2;

        if (value_[l + step].key_ < target) {
          l = l + step + 1;
          count -= step + 1;
        } else {
          count = step;
        }
      }

      return l;
    }

    inline int upper_bound(Key target) {
      int l = 0, count = last_position_, step;

      while (count != 0) {
        step = count / 2;

        if (value_[l + step].key_ < target || value_[l + step].key_ == target) {
          l = l + step + 1;
          count -= step + 1;
        } else {
          count = step;
        }
      }

      return l;
    }
  };

  std::string filename_; // Store the filename (No use in pre-homework, might
                         // help in Ticket System)
  size_t root_position_;

  LruCache<Node, 20011, size_t(3e6 + 1)> cache;

  std::fstream file_;

  // Node *root_; // Store the root Node (Might be frequently read and modify,
  // thus put in memory)

  template <class Type>
  inline size_t
  read(Type *data, size_t position, std::ios_base::seekdir mode = std::ios::beg) { // Simple read funtion(to simplify my code)
    file_.seekg(position, mode);
    size_t tmp = size_t(file_.tellg());
    file_.read(reinterpret_cast<char *>(data), sizeof(*data));
    return tmp;
  }

  inline size_t read(Node *&data, size_t position, std::ios_base::seekdir mode = std::ios::beg) {
    data = cache.Get(position);
    return position;
  }

  template <class Type>
  inline size_t
  write(Type *data, size_t position, std::ios_base::seekdir mode = std::ios::beg) { // Simple write funtion
    file_.seekp(position, mode);
    size_t tmp = size_t(file_.tellp());
    file_.write(reinterpret_cast<char *>(data), sizeof(*data));
    return tmp;
  }

  inline size_t
  write(Node *data, size_t position, std::ios_base::seekdir mode = std::ios::beg) { // Simple write funtion
    return cache.Throw(position, data, mode);
  }

  template <class Type>
  inline size_t
  read(Type &data, size_t position, std::ios_base::seekdir mode = std::ios::beg) { // Simple read funtion(to simplify my code)
    file_.seekg(position, mode);
    size_t tmp = size_t(file_.tellg());
    file_.read(reinterpret_cast<char *>(&data), sizeof(data));
    return tmp;
  }

  // inline size_t
  // read(Node &data, size_t position, std::ios_base::seekdir mode = std::ios::beg) { // Simple read funtion(to simplify my code)
  //   Node *cache_data = cache.Find(position);
  //   if (cache_data != nullptr) {
  //     data = *cache_data;
  //     return position;
  //   }
  //   file_.seekg(position, mode);
  //   size_t tmp = size_t(file_.tellg());
  //   file_.read(reinterpret_cast<char *>(&data), sizeof(data));
  //   cache.Insert(position, new Node(data));
  //   return tmp;
  // }

  template <class Type>
  inline size_t
  write(Type data, size_t position, std::ios_base::seekdir mode = std::ios::beg) { // Simple write funtion
    file_.seekp(position, mode);
    size_t tmp = size_t(file_.tellp());
    file_.write(reinterpret_cast<char *>(&data), sizeof(data));
    return tmp;
  }

  inline ValueType
  Split(Node *p,size_t p_position) { // Split a full block into two parts (the methods
                                    // depends on whether the Node is a leaf or not)
    Node *new_p = new Node; // Remember to deal with some special case (e.g p is root)
    ValueType ret_value;
    if (p->is_leaf_) { // There are actually two different place, one is that
                       // the son of last position is linked to the next Node,
      for (int i = p->last_position_ / 2, j = 0; i < p->last_position_; i++, j++) { // another is that the middle data of leaf 
        new_p->value_[j] = p->value_[i]; // Node won't be transfered to its father Node
        p->son_[i] = -1;
      }
      new_p->is_leaf_ = true;
      new_p->last_position_ = p->last_position_ - (p->last_position_ / 2);
      new_p->son_[new_p->last_position_] = p->son_[p->last_position_];
      p->son_[p->last_position_] = -1;
      p->last_position_ /= 2;
      size_t tmp_right = p->son_[p->last_position_] = write(new_p, 0, std::ios::end);
      size_t tmp_left = write(p, p_position);
      ret_value = new_p->value_[0];
      if (p_position ==
          root_position_) { // if the Node to be split is the root Node, then we
                            // will have to add a new Node as new root
        Node *new_root = new Node;
        new_root->value_[0] = ret_value;
        new_root->son_[0] = tmp_left;
        new_root->son_[1] = tmp_right;
        new_root->last_position_ = 1;
        root_position_ = write(new_root, 0, std::ios::end);
        write(root_position_, 0);
        // delete p;
      }
    } else {
      for (int i = p->last_position_ / 2 + 1, j = 0; i < p->last_position_;
           i++, j++) {
        new_p->value_[j] = p->value_[i];
        new_p->son_[j] = p->son_[i];
      }
      new_p->last_position_ = p->last_position_ - (p->last_position_ / 2) - 1;
      new_p->son_[new_p->last_position_] = p->son_[p->last_position_];
      p->last_position_ /= 2;
      size_t tmp_right = p->son_[p->last_position_ + 1] = write(new_p, 0, std::ios::end);
      size_t tmp_left = write(p, p_position);
      ret_value = p->value_[p->last_position_];
      if (p_position == root_position_) { // if the Node to be split is the root Node, then we
                                         // will have to add a new Node as new root
        Node *new_root = new Node;
        new_root->value_[0] = p->value_[p->last_position_];
        new_root->son_[0] = tmp_left;
        new_root->son_[1] = tmp_right;
        new_root->last_position_ = 1;
        root_position_ = write(new_root, 0, std::ios::end);
        write(root_position_, 0);
        // delete p;
      }
    }
    // delete new_p;
    return ret_value;
  }

  inline void merge(Node *prev, Node *next, size_t prev_position) {
    for (int i = 0; i < next->last_position_; i++) {
      prev->value_[i + prev->last_position_] = next->value_[i];
      prev->son_[i + prev->last_position_] = next->son_[i];
    }
    prev->son_[next->last_position_ + prev->last_position_] =
        next->son_[next->last_position_];
    prev->last_position_ += next->last_position_;
    write(prev, prev_position);
    return;
  }

  Pair<bool, ValueType> InsertNode(ValueType value, Node *p, size_t p_position = 0) {   // Insert a value into the B+ tree(well I 
    if (p == nullptr) { // might have to write a funtion for client to use)
      read(p, root_position_);
      p_position = root_position_;
    }
    int new_son = -1;
    int next_p_position = p->lower_bound(value);
    if (!p->is_leaf_) {
      // Node *next_p = new Node;
      Node *next_p;
      read(next_p, p->son_[next_p_position]);
      Pair<bool, ValueType> ret;
      try {
        ret = InsertNode(value, next_p, p->son_[next_p_position]);
      } catch (Exception *error) {
        throw error;
      }
      if (!ret.first) {
        // delete next_p;
        return ret;
      } else {
        value = ret.second;
        new_son = next_p->is_leaf_ ? next_p->son_[next_p->last_position_] : next_p->son_[next_p->last_position_ + 1];
        // delete next_p;
      }
    }
    if (p->value_[next_p_position] == value && next_p_position != p->last_position_) {
      throw new IllegalInsert();
    }
    for (int j = p->last_position_; j > next_p_position; j--) {
      p->value_[j] = p->value_[j - 1];
      p->son_[j + 1] = p->son_[j];
    }
    p->value_[next_p_position] = value;
    if (next_p_position == p->last_position_) {
      p->son_[next_p_position + 1] =
          p->is_leaf_ ? p->son_[p->last_position_] : new_son;
    } else {
      p->son_[next_p_position + 1] = new_son;
    }
    ++p->last_position_;
    if (p->last_position_ == kSizeOfBlock) {
      return Pair<bool, ValueType>(true, Split(p, p_position));
    } else {
      write(p, p_position);
      return Pair<bool, ValueType>(false, ValueType());
    }
  }

  // There are lots of points that need concerning
  // p_relative_position stores the position of p in its
  Pair<bool, ValueType>
  DeleteNode(ValueType value, Node *p_father, Node *p, size_t p_relative_position) { // Delete a value from the B+ tree
    if (!p->is_leaf_) {
      // Node *next_p = new Node;
      Node *next_p;
      int next_p_position = p->upper_bound(value);
      read(next_p, p->son_[next_p_position]);
      Pair<bool, ValueType> ret;
      try {
        ret = DeleteNode(value, p, next_p, next_p_position);
      } catch (Exception *error) {
        // delete next_p;
        throw error;
      }
      // delete next_p;
      if (ret.first) {
        if (next_p_position > 0 && p->value_[next_p_position - 1] == value) {
          p->value_[next_p_position - 1] =
              ret.second; // Replace the value that has been deleted
          ret.first = false;
        }
      }
      if (p->last_position_ < std::ceil(kSizeOfBlock / 2.0) - 1) {
        if (p_father == nullptr) {
          if (p->last_position_ == 0) {
            // root_ = new Node;
            // root_position_ = read(root_, p->son_[0]);
            root_position_ = p->son_[0];
            write(root_position_, 0);
            // delete p;
            return Pair<bool, ValueType>(false, ValueType());
          }
          write(p, root_position_);
          return Pair<bool, ValueType>(false, ValueType());
        }
        // Node *bro = new Node;
        Node *bro;
        int bro_position, book = 3;
        if (p_relative_position != 0) { // If p has a previous brother
          bro_position = read(bro, p_father->son_[p_relative_position - 1]);
          book = 1;
          if (bro->last_position_ >
              std::ceil(kSizeOfBlock / 2.0) -
                  1) { // Check whether we can borrow a value from there
            for (int i = p->last_position_; i > 0;
                 i--) { // We have to rewrite the data so that the value from
                        // brother can be insert
              p->value_[i] = p->value_[i - 1];
              p->son_[i + 1] = p->son_[i];
            }
            p->son_[1] = p->son_[0];
            p->son_[0] = bro->son_[bro->last_position_];
            p->last_position_++; // Adjust the last node of p node
            p->value_[0] = p_father->value_[p_relative_position - 1]; // Change the key in father
            p_father->value_[p_relative_position - 1] =
                bro->value_[bro->last_position_ - 1];
            --bro->last_position_;
            write(p, p_father->son_[p_relative_position]);
            write(bro, bro_position);
            book = 0;
          }
        }
        if (book && p_relative_position < p_father->last_position_) { // If p has no previous brother, then
                                                                      // there must be a brother behind to it
          bro_position = read(bro, p_father->son_[p_relative_position + 1]);
          book = 2;
          if (bro->last_position_ > std::ceil(kSizeOfBlock / 2.0) - 1) { // Check whether we can borrow a value from there
            p->last_position_++;
            p->son_[p->last_position_] = bro->son_[0];
            p->value_[p->last_position_ - 1] = p_father->value_[p_relative_position];
            p_father->value_[p_relative_position] = bro->value_[0]; // Change the key in father
            for (int i = 0; i < bro->last_position_ - 1; i++) {
              bro->value_[i] = bro->value_[i + 1];
              bro->son_[i] = bro->son_[i + 1];
            }
            --bro->last_position_;
            bro->son_[bro->last_position_] = bro->son_[bro->last_position_ + 1]; // Adjust the value of brother node
            write(p, p_father->son_[p_relative_position]);
            write(bro, bro_position);
            book = 0;
          }
        }
        if (book && bro->last_position_ == std::ceil(kSizeOfBlock / 2.0) - 1) { // To merge two non-leaf nodes, you have pull a
                                                                                    // key from its father down
          int delete_place_in_father = 0;
          if (book == 1) {
            bro->value_[bro->last_position_] =
                p_father->value_[p_relative_position - 1];
            bro->last_position_++;
            merge(bro, p, bro_position);
            delete_place_in_father = p_relative_position - 1;
          } else {
            p->value_[p->last_position_] =
                p_father->value_[p_relative_position];
            p->last_position_++;
            merge(p, bro, p_father->son_[p_relative_position]);
            delete_place_in_father = p_relative_position;
          }
          for (int i = delete_place_in_father; i < p_father->last_position_;
               i++) {
            p_father->son_[i + 1] = p_father->son_[i + 2];
            p_father->value_[i] = p_father->value_[i + 1];
          }
          p_father->last_position_--;
        }
        // delete bro;
      } else {
        if (p_father == nullptr) {
          write(p, root_position_);
        } else {
          write(p, p_father->son_[p_relative_position]);
        }
      }
      return ret;
    } else {
      int delete_value_position = 0;
      for (; delete_value_position < p->last_position_; delete_value_position++) { // Find the value to be delete
        if (!(p->value_[delete_value_position] < value)) {
          break;
        }
      }
      if (delete_value_position == p->last_position_ || !(p->value_[delete_value_position] == value)) {
        throw new IllegalDelete();
      }
      if (p->last_position_ ==
          kSizeOfBlock / 2) { // Devide circumstances into two condition
        if (p_father ==
            nullptr) { // If p is the root, then nothing have to be done
          for (int i = delete_value_position; i < p->last_position_ - 1; i++) {
            p->value_[i] = p->value_[i + 1];
            p->son_[i] = p->son_[i + 1];
          }
          p->son_[p->last_position_ - 1] = p->son_[p->last_position_];
          p->last_position_--;
          write(p, root_position_);
          return Pair<bool, ValueType>(false, ValueType());
        }
        // Node *bro = new Node;
        Node *bro;
        int bro_position, book = 3;
        if (p_relative_position != 0) { // If p has a previous brother
          bro_position = read(bro, p_father->son_[p_relative_position - 1]);
          book = 1;
          if (bro->last_position_ >
              kSizeOfBlock /
                  2) { // Check whether we can borrow a value from there
            for (int i = delete_value_position; i > 0;
                 i--) { // We have to rewrite the data so that the value from
                        // brother can be insert
              p->value_[i] = p->value_[i - 1];
              p->son_[i] = p->son_[i - 1];
            }
            p->value_[0] = bro->value_[bro->last_position_ - 1];
            --bro->last_position_;
            bro->son_[bro->last_position_] =
                bro->son_[bro->last_position_ +
                          1]; // Adjust the last node of brother node
            write(p, p_father->son_[p_relative_position]);
            write(bro, bro_position);
            // delete bro;
            p_father->value_[p_relative_position - 1] = p->value_[0];
            book = 0;
          }
        }
        if (book &&
            p_relative_position <
                p_father->last_position_) { // If book is not zero, then we can
                                            // check the node behind p
          bro_position = read(bro, p_father->son_[p_relative_position + 1]);
          book = 2;
          if (bro->last_position_ >
              kSizeOfBlock /
                  2) { // Check whether we can borrow a value from there
            for (int i = delete_value_position; i < p->last_position_ - 1;
                 i++) {
              p->value_[i] = p->value_[i + 1];
              p->son_[i] = p->son_[i + 1];
            }
            p->value_[p->last_position_ - 1] = bro->value_[0];
            for (int i = 0; i < bro->last_position_ - 1; i++) {
              bro->value_[i] = bro->value_[i + 1];
            }
            --bro->last_position_;
            bro->son_[bro->last_position_] =
                bro->son_[bro->last_position_ +
                          1]; // Adjust the value of brother node
            write(p, p_father->son_[p_relative_position]);
            write(bro, bro_position);
            p_father->value_[p_relative_position] = bro->value_[0];
            // delete bro;
            book = 0;
          }
        }
        if (book && bro->last_position_ == kSizeOfBlock / 2) { // To merge two leaf nodes, you have to
                                                                // merge it with its previous node
          int delete_place_in_father = 0;
          for (int i = delete_value_position; i < p->last_position_ - 1; i++) {
            p->value_[i] = p->value_[i + 1];
          }
          p->son_[p->last_position_ - 1] = p->son_[p->last_position_];
          p->son_[p->last_position_] = -1;
          p->last_position_--;
          if (book == 1) {
            merge(bro, p, bro_position);
            delete_place_in_father = p_relative_position - 1;
          } else {
            merge(p, bro, p_father->son_[p_relative_position]);
            delete_place_in_father = p_relative_position;
          }
          for (int i = delete_place_in_father; i < p_father->last_position_ - 1;
               i++) {
            p_father->son_[i + 1] = p_father->son_[i + 2];
            p_father->value_[i] = p_father->value_[i + 1];
          }
          p_father->last_position_--;
          // delete bro;
        }
        if (delete_value_position == 0) {
          return Pair<bool, ValueType>(true, p->value_[0]);
        } else {
          return Pair<bool, ValueType>(false, ValueType());
        }
      } else {
        for (int j = delete_value_position; j < p->last_position_ - 1; j++) {
          p->value_[j] = p->value_[j + 1];
        }
        p->son_[p->last_position_ - 1] = p->son_[p->last_position_];
        p->son_[p->last_position_] = -1;
        --p->last_position_;
        if (p_father == nullptr) { // If there are enough value in the Node,
                                   // then we can just erase it
          write(p, root_position_);
        } else {
          write(p, p_father->son_[p_relative_position]);
        }
        if (delete_value_position == 0) {
          return Pair<bool, ValueType>(true, p->value_[0]);
        } else {
          return Pair<bool, ValueType>(false, ValueType());
        }
      }
    }
  }

public:
  BpTree(std::string filename = "bpdata") {
    filename_ = filename + "_rootinfo";
    cache.OpenFile(filename);
    if (std::ifstream(filename_).good()) {
      file_.open(filename_, std::ios::in | std::ios::out | std::ios::binary);
      read(root_position_, 0);
      // root_ = new Node;
      Node *root_;
      read(root_, root_position_);
    } else {
      std::ofstream(filename_).close();
      file_.open(filename_, std::ios::in | std::ios::out | std::ios::binary);
      // root_ = new Node;
      Node *root_ = new Node;
      root_->is_leaf_ = true;
      root_position_ = 0;
      write(root_position_, 0);
      write(root_, 0, std::ios::end);
    }
  }

  ~BpTree() {
    // delete root_;
    write(root_position_, 0);
    file_.close();
  }

  void Insert(Key key, Data data) { // A function for client to use
    try {
      InsertNode(ValueType(key, data), nullptr, root_position_);
    } catch (Exception *error) {
      throw error;
    }
  }

  void Delete(Key key, Data data) {
    try {
      Node *root;
      read(root, root_position_);
      DeleteNode(ValueType(key, data), nullptr, root, root_position_);
    } catch (Exception *error) {
      throw error;
    }
  }

  bool Empty() {
    Node *root;
    read(root, root_position_);
    return root->last_position_ == 0;
  }

  sjtu::vector<Data>* Find(Key key) { // Find those value whose key equals to key that input by users
    // Node p = *root_;
    Node *p;
    sjtu::vector<Data>* ret = new sjtu::vector<Data>;
    read(p, root_position_);
    while (!p->is_leaf_) {
      Node *tmp;
      read(tmp, p->son_[p->lower_bound(key)]);
      p = tmp;
    }
    int pos = 0;
    bool book = false;
    while (pos < p->last_position_ && !(p->value_[pos].key_ == key) && p->value_[pos].key_ < key) {
      ++pos;
      if (pos == p->last_position_) {
        if (p->son_[p->last_position_] == -1) {
          delete ret;
          throw new NothingFind();
        }
        Node *tmp;
        read(tmp, p->son_[p->last_position_]);
        p = tmp;
        pos = 0;
      }
    }
    while (pos < p->last_position_ && p->value_[pos].key_ == key) {
      ret->push_back(p->value_[pos].value_);
      book = true;
      ++pos;
      if (pos >= p->last_position_) {
        if (p->son_[p->last_position_] == -1) {
          return ret;
        }
        Node *tmp;
        read(tmp, p->son_[p->last_position_]);
        p = tmp;
        pos = 0;
      }
    }
    if (!book) {
      delete ret;
      throw new NothingFind();
    }
    return ret;
  }
};

#endif // BP_TREE_HPP