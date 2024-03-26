#ifndef BP_TREE_HPP
#define BP_TREE_HPP

#include <algorithm>
#include <iostream>
#include <cstdio>
#include <iterator>
#include <string>
#include <cmath>
#include <fstream>
#include <filesystem>
#include "Exceptions.hpp"

template <class Key, class Data>
class BpTree {

static const int SIZE_OF_VALUE = sizeof(Key) + sizeof(Data);
static const int SIZE_OF_BLOCK = (4096 - 9) / (SIZE_OF_VALUE + 4);

private:

  struct ValueType{ // Store the index and value in structure Node
    Key key_;
    Data value_;

    ValueType() = default;

    ValueType(Key key, Data value) {
      key_ = key;
      value_ = value;
    }

    bool operator<(ValueType &rhs) {
      return (key_ == rhs.key_) ? value_ < rhs.value_ : key_ < rhs.key_;
    }

    void operator=(ValueType &rhs) {
      key_ = rhs.key_;
      value_ = rhs.value_;
    }
  };

  template <class First, class Second> // A simple pair class to make it more comvenient in insert & erase
  class Pair {
  public:
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

    Pair & operator= (Pair &other) {
      if (&other == this) {
        return *this;
      }
      first = other.first;
      second = other.second;
    }
  };

  struct Node {
    Node () { // Default constructer
      last_position_ = 0;
      is_leaf_ = false;
      for (int i = 0; i <= SIZE_OF_BLOCK; i++) {
        son_[i] = -1;
      }
    }

    ValueType value_[SIZE_OF_BLOCK];
    int son_[SIZE_OF_BLOCK + 1];
    bool is_leaf_;
    int last_position_;
  };

  std::string filename_; // Store the filename (No use in pre-homework, might help in Ticket System)
  int root_position_;

  Node *root_; // Store the root Node (Might be frequently read and modify, thus put in memory)

  std::fstream file_;

  template <class Type>
  inline int read(Type *data, int position, std::ios_base::seekdir mode = std::ios::beg) { // Simple read funtion(to simplify my code)
    file_.seekg(position, mode);
    int tmp = int(file_.tellg());
    file_.read(reinterpret_cast<char *>(data), sizeof(*data));
    return tmp;
  }

  template <class Type>
  inline int write(Type *data, int position, std::ios_base::seekdir mode = std::ios::beg) { // Simple write funtion
    file_.seekp(position, mode);
    int tmp = int(file_.tellp());
    file_.write(reinterpret_cast<char *>(data), sizeof(*data));
    return tmp;
  }
  
  template <class Type>
  inline int read(Type &data, int position, std::ios_base::seekdir mode = std::ios::beg) { // Simple read funtion(to simplify my code)
    file_.seekg(position, mode);
    int tmp = int(file_.tellg());
    file_.read(reinterpret_cast<char *>(&data), sizeof(data));
    return tmp;
  }

  template <class Type>
  inline int write(Type data, int position, std::ios_base::seekdir mode = std::ios::beg) { // Simple write funtion
    file_.seekp(position, mode);
    int tmp = int(file_.tellp());
    file_.write(reinterpret_cast<char *>(&data), sizeof(data));
    return tmp;
  }

  inline ValueType Split(Node *p, int p_position) { // Split a full block into two parts (the methods depends on whether the Node is a leaf or not)
    Node *new_p = new Node;                         // Remember to deal with some special case (e.g p is root)
    ValueType ret_value;
    if (p->is_leaf_) {        // There are actually two different place, one is that the son of last position is linked to the next Node,
      for (int i = p->last_position_ / 2, j = 0; i < p->last_position_; i++, j++) { // another is that the middle data of leaf Node won't
        new_p->value_[j] = p->value_[i];                                            // be transfered to its father Node
        new_p->son_[j] = p->son_[i];
      }
      new_p->is_leaf_ = true;
      new_p->last_position_ = p->last_position_ - (p->last_position_ / 2);
      new_p->son_[new_p->last_position_] = p->son_[p->last_position_];
      p->last_position_ /= 2;
      int tmp_right = p->son_[p->last_position_] = write(new_p, 0, std::ios::end);
      int tmp_left = write(p, p_position);
      ret_value = new_p->value_[0];
      if (p == root_) { // if the Node to be split is the root Node, then we will have to add a new Node as new root
        root_ = new Node;
        root_->value_[0] = p->value_[p->last_position_ + 1];
        root_->son_[0] = tmp_left;
        root_->son_[1] = tmp_right;
        root_->last_position_ = 1;
        root_position_ = write(root_, 0, std::ios::end);
        write(root_position_, 0);
        delete p;
      }
    } else {
      for (int i = p->last_position_ / 2 + 1, j = 0; i < p->last_position_; i++, j++) {
        new_p->value_[j] = p->value_[i];
        new_p->son_[j] = p->son_[i];
      }
      new_p->last_position_ = p->last_position_ - (p->last_position_ / 2) - 1;
      new_p->son_[new_p->last_position_] = p->son_[p->last_position_];
      p->last_position_ /= 2;
      int tmp_right = p->son_[p->last_position_ + 1] = write(new_p, 0, std::ios::end);
      int tmp_left = write(p, p_position);
      ret_value = p->value_[p->last_position_];
      if (p == root_) { // if the Node to be split is the root Node, then we will have to add a new Node as new root
        root_ = new Node;
        root_->value_[0] = p->value_[p->last_position_];
        root_->son_[0] = tmp_left;
        root_->son_[1] = tmp_right;
        root_->last_position_ = 1;
        root_position_ = write(root_, 0, std::ios::end);
        write(root_position_, 0);
        delete p;
      }
    }
    delete new_p;
    return ret_value;
  }

  inline void merge(Node *prev, Node *next, int prev_position) {
    for (int i = 0; i < next->last_position_; i++) {
      prev->value_[i + prev->last_position_] = next->value_[i];
      prev->son_[i + prev->last_position_] = next->son_[i];
    }
    prev->son[next->last_position_ + prev->last_position_] = next->son_[next->last_position_];
    prev->last_position_ += next->last_position_;
    write(prev, prev_position);
    return;
  }

  Pair<bool, ValueType> InsertNode(ValueType value, Node *p, int p_position = 0) { // Insert a value into the B+ tree(well I might have to
    if (p == nullptr) {                                                            // write a funtion for client to use)
      p = root_;
    }
    int new_son = -1;
    if (!p->is_leaf_) {
      Node *next_p = new Node;
      int next_p_position;
      for (int i = 0; i <= p->last_position_; i++) {
        if (i == p->last_position_) {
          read(next_p, p->son_[p->last_position_]);
          next_p_position = p->son_[p->last_position_];
        } else {
          if (value < p->value_[i]) {
            read(next_p, p->son_[i]);
            next_p_position = p->son_[i];
            break;
          }
        }
      }
      Pair<bool, ValueType> ret = InsertNode(value, next_p, next_p_position);
      new_son = next_p->is_leaf_ ? next_p->son_[next_p->last_position_] : next_p->son_[next_p->last_position_ + 1];
      delete next_p;
      if (!ret.first) {
        return ret;
      } else {
        value = ret.second;
      }
    }
    for (int i = 0; i <= p->last_position_; i++) {
      if (i == p->last_position_) {
        p->value_[p->last_position_] = value;
        p->son_[p->last_position_ + 1] = p->is_leaf_ ? p->son_[p->last_position_] : new_son;
      } else {
        if (!(p->value_[i] < value)) {
          for (int j = p->last_position_; j > i; j--) {
            p->value_[j] = p->value_[j - 1];
            p->son_[j + 1] = p->son_[j];
          }
          p->value_[i] = value;
          p->son_[i + 1] = new_son;
          break;
        }  
      }
    }
    ++p->last_position_;
    if (p->last_position_ == SIZE_OF_BLOCK) {
      return Pair(true, Split(p, p_position));
    } else {
      write(p, p_position);
      return Pair(false, ValueType());
    }
  }

  // There are lots of points that need concerning
  //p_relative_position stores the position of p in its
  Pair<bool, ValueType> DeleteNode(ValueType value, Node *p_father, Node *p, int p_relative_position) { // Delete a value from the B+ tree
    if (p_father == nullptr) {
      p = root_;
    }
    if (!p->is_leaf_) {
      Node *next_p = new Node;
      int next_p_position;
      for (int i = 0; i <= p->last_position_; i++) {
        if (i == p->last_position_) {
          read(next_p, p->son_[p->last_position_]);
          next_p_position = p->last_position_;
        } else {
          if (value < p->value_[i]) {
            read(next_p, p->son_[i]);
            next_p_position = i;
            break;
          }
        }
      }
      Pair<bool, ValueType> ret = DeleteNode(value, p, next_p, next_p_position);
      delete next_p;
      if (ret.first) {
        p->value_[next_p_position - 1] = ret.second; // Replace the value that has been deleted
      }
      if (p->last_position_ < SIZE_OF_BLOCK / 2) {
        if (p_father == nullptr) {
          if (p->last_position_ == 0) {
            delete p;
          }
          return Pair(false, ValueType());
        }
        Node *bro = new Node;
        int bro_position, book = 0;
        if (p_relative_position != 0) { // If p has a previous brother
          bro_position = read(bro, p_father->son[p_relative_position - 1]);
          if (bro->last_position_ > SIZE_OF_BLOCK / 2) { // Check whether we can borrow a value from there
            for (int i = p->last_position_; i > 0; i--) { // We have to rewrite the data so that the value from brother can be insert
              p->value_[i] = p->value_[i - 1];
              p->son_[i + 1] = p->son_[i];
            }
            p->last_position_++;
            p->value_[0] = p_father->value_[p_relative_position - 1]; // Change the key in father
            p_father->value_[p_relative_position - 1] = bro->value_[bro->last_position_ - 1];
            --bro->last_position_;
            bro->son_[bro->last_position_] = bro->son_[bro->last_position_ + 1]; // Adjust the last node of brother node
            write(p, p_father->son_[p_relative_position]);
            write(bro, bro_position);
            delete bro;
            p_father->value_[p_relative_position - 1] = p->value_[0];
          }
        } else { // If p has no previous brother, then there must be a brother behind to it
          bro_position = read(bro, p_father->son_[p_relative_position + 1]);
          book = 1;
          if (bro->last_position_ > SIZE_OF_BLOCK / 2) { // Check whether we can borrow a value from there
            p->last_position_++;
            p->value_[p->last_position_ - 1] = bro->value_[0];
            for (int i = 0; i < bro->last_position_ - 1; i++) {
              bro->value_[i] = bro->value_[i + 1];
            }
            p_father->value[p_relative_position] = bro->value[0]; // Change the key in father
            --bro->last_position_;
            bro->son_[bro->last_position_] = bro->son_[bro->last_position_ + 1]; // Adjust the value of brother node
            write(p, p_father->son_[p_relative_position]);
            write(bro, bro_position);
            p_father->value_[p_relative_position] = bro->value_[0];
            delete bro;
          }
        }
        if (bro->last_position_ < SIZE_OF_BLOCK / 2) { // To merge two non-leaf nodes, you have pull a key from its father down
          int delete_place_in_father = 0;
          if (book == 0) {
            bro->value_[bro->last_position_] = p_father->value_[p_relative_position - 1];
            merge(bro, p, bro_position);
            delete_place_in_father = p_relative_position - 1;
          } else {
            p->value_[p->last_position_] = p_father->value_[p_relative_position];
            merge(p, bro, p_father->son_[p_relative_position - 1]);
            delete_place_in_father = p_relative_position;
          }
          for (int i = delete_place_in_father; i < p_father->last_position_; i++) {
            p_father->son_[i + 1] = p_father->son_[i + 2];
            p_father->value_[i] = p_father->value_[i + 1];
          }
          p_father->last_position_--;
          delete bro;
        }
      }
    } else {
      int delete_value_position = 0;
      for (; delete_value_position < p->last_position_; delete_value_position++) {  // Find the value to be delete
        if (!(p->value_[delete_value_position] < value)) {
          break;
        }
      }
      if (delete_value_position == p->last_position_) {
        throw IllegalDelete();
      }
      if (p->last_position_ == SIZE_OF_BLOCK / 2) { // Devide circumstances into two condition
        if (p_father == nullptr) { // If p is the root, then nothing have to be done
          return Pair(false, ValueType());
        }
        Node *bro = new Node;
        int bro_position, book = 0;
        if (p_relative_position != 0) { // If p has a previous brother
          bro_position = read(bro, p_father->son[p_relative_position - 1]);
          if (bro->last_position_ > SIZE_OF_BLOCK / 2) { // Check whether we can borrow a value from there
            for (int i = delete_value_position; i > 0; i--) { // We have to rewrite the data so that the value from brother can be insert
              p->value_[i] = p->value_[i - 1];
              p->son_[i + 1] = p->son_[i];
            }
            p->value_[0] = bro->value_[bro->last_position_];
            --bro->last_position_;
            bro->son_[bro->last_position_] = bro->son_[bro->last_position_ + 1]; // Adjust the last node of brother node
            write(p, p_father->son_[p_relative_position]);
            write(bro, bro_position);
            delete bro;
            p_father->value_[p_relative_position - 1] = p->value_[0];
          }
        } else { // If p has no previous brother, then there must be a brother behind to it
          bro_position = read(bro, p_father->son_[p_relative_position + 1]);
          book = 1;
          if (bro->last_position_ > SIZE_OF_BLOCK / 2) { // Check whether we can borrow a value from there
            for (int i = delete_value_position; i < p->last_position_ - 1; i++) {
              p->value_[i] = p->value_[i + 1];
              p->son_[i + 1] = p->son[i + 2];
            }
            p->value_[p->last_position_ - 1] = bro->value_[0];
            for (int i = 0; i < bro->last_position_ - 1; i++) {
              bro->value_[i] = bro->value_[i + 1];
            }
            --bro->last_position_;
            bro->son_[bro->last_position_] = bro->son_[bro->last_position_ + 1]; // Adjust the value of brother node
            write(p, p_father->son_[p_relative_position]);
            write(bro, bro_position);
            p_father->value_[p_relative_position] = bro->value_[0];
            delete bro;
          }
        }
        if (bro->last_position_ < SIZE_OF_BLOCK / 2) { // To merge two leaf nodes, you have to merge it with its previous node
          int delete_place_in_father = 0;
          if (book == 0) {
            merge(bro, p, bro_position);
            delete_place_in_father = p_relative_position - 1;
          } else {
            merge(p, bro, p_father->son_[p_relative_position - 1]);
            delete_place_in_father = p_relative_position;
          }
          for (int i = delete_place_in_father; i < p_father->last_position_; i++) {
            p_father->son_[i + 1] = p_father->son_[i + 2];
            p_father->value_[i] = p_father->value_[i + 1];
          }
          p_father->last_position_--;
          delete bro;
        }
        if (delete_value_position == 0) {
          return Pair(true, p->value_[0]);
        } else {
          return Pair(false, ValueType());
        }
      } else {
        for (int j = delete_value_position; j < p->last_position_ - 1; j++) {
          p->value_[j] = p->value_[j + 1];
          p->son_[j] = p->son_[j + 1];
        }
        p->son_[p->last_position_ - 1] = p->son_[p->last_position_];
        --p->last_position_;
        write(p, p_father->son_[p_relative_position]); // If there are enough value in the Node, then we can just erase it
        if (delete_value_position == 0) {
          return Pair(true, p->value_[0]);
        } else {
          return Pair(false, ValueType());
        }
      }
    }
  }



public:

  BpTree(std::string filename = "bpdata") {
    filename_ = filename;
    if (std::ifstream(filename_).good()) {
      file_.open(filename_, std::ios::in | std::ios::out | std::ios::binary);
      read(root_position_, 0);
      root_ = new Node;
      read(root_, root_position_);
    } else {
      std::ofstream(filename_).close();
      file_.open(filename_, std::ios::in | std::ios::out | std::ios::binary);
      root_ = new Node;
      root_->is_leaf_ = true;
      root_position_ = write(root_, sizeof(root_position_));
      Node *test = new Node;
      read(test, root_position_);
      write(root_position_, 0);
    }
  }

  ~BpTree() {
    delete root_;
    file_.close();
  }

  void Insert(Key key, Data data) { // A function for client to use
    try {
      InsertNode(ValueType(key, data), nullptr, root_position_).first;
    } catch (Exception *error) {
      throw error;
    }
  }

  bool Delete() {
    return true;
  }

  void Find(Key key) { // Find those value whose key equals to key that input by users
    Node p = *root_;
    while (!p.is_leaf_) {
      for (int i = 0; i <= p.last_position_; i++) {
        if (i == p.last_position_) {
          read(p, p.son_[p.last_position_]);
          break;
        } else {
          if (!(p.value_[i].key_ < key)) {
            read(p, p.son_[std::max(i - 1, 0)]);
            break;
          }
        }
      }
    }
    int pos = 0;
    while (!(p.value_[pos].key_ == key) && p.value_[pos].key_ < key) {
      ++pos;
      if (pos == p.last_position_) {
        if (p.son_[p.last_position_] == -1) {
          return;
        }
        read(p, p.son_[p.last_position_]);
        pos = 0;
      }
    }
    while (p.value_[pos].key_ == key) {
      std::cout << p.value_[pos].value_;
      ++pos;
      if (pos == p.last_position_) {
        if (p.son_[p.last_position_] == -1) {
          return;
        }
        read(p, p.son_[p.last_position_]);
        pos = 0;
      }
    }
  }

};

#endif //BP_TREE_HPP