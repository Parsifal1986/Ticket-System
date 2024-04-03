#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

class Exception {
public:
  Exception() {}

  virtual ~Exception() {}

  virtual void what() = 0;
};

class IllegalInsert : public Exception {
public:
  IllegalInsert() {}

  ~IllegalInsert() {}

  void what() override { std::cout << "Insert Fail!" << std::endl; }
};

class IllegalDelete : public Exception {
public:
  IllegalDelete() {}

  ~IllegalDelete() {}

  void what() override { std::cout << "Delete Fail!" << std::endl; }
};

class NothingFind : public Exception {
public:
  NothingFind() {}

  ~NothingFind() {}

  void what() override { std::cout << "null" << std::endl; }
};

template <class Key, class Data, int appoint_size = 0> class BpTree {

  static const int SIZE_OF_VALUE = sizeof(Key) + sizeof(Data);
  static const int SIZE_OF_BLOCK =
      appoint_size ? appoint_size : (4096 - 9) / (SIZE_OF_VALUE + 4);

private:
  struct ValueType { // Store the index and value in structure Node
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

    bool operator==(ValueType &other) {
      return (key_ == other.key_ && value_ == other.value_);
    }
  };

  template <class First, class Second> // A simple pair class to make it more
                                       // comvenient in insert & erase
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
    Node() { // Default constructer
      last_position_ = 0;
      is_leaf_ = false;
      for (int i = 0; i <= SIZE_OF_BLOCK; i++) {
        son_[i] = -1;
      }
    }

    ValueType value_[SIZE_OF_BLOCK];
    size_t son_[SIZE_OF_BLOCK + 1];
    bool is_leaf_;
    int last_position_;

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
  };

  std::string filename_; // Store the filename (No use in pre-homework, might
                         // help in Ticket System)
  size_t root_position_;

  Node *root_; // Store the root Node (Might be frequently read and modify, thus
               // put in memory)

  std::fstream file_;

  template <class Type>
  inline size_t
  read(Type *data, size_t position,
       std::ios_base::seekdir mode =
           std::ios::beg) { // Simple read funtion(to simplify my code)
    file_.seekg(position, mode);
    size_t tmp = size_t(file_.tellg());
    file_.read(reinterpret_cast<char *>(data), sizeof(*data));
    return tmp;
  }

  template <class Type>
  inline size_t
  write(Type *data, size_t position,
        std::ios_base::seekdir mode = std::ios::beg) { // Simple write funtion
    file_.seekp(position, mode);
    size_t tmp = size_t(file_.tellp());
    file_.write(reinterpret_cast<char *>(data), sizeof(*data));
    return tmp;
  }

  template <class Type>
  inline size_t
  read(Type &data, size_t position,
       std::ios_base::seekdir mode =
           std::ios::beg) { // Simple read funtion(to simplify my code)
    file_.seekg(position, mode);
    size_t tmp = size_t(file_.tellg());
    file_.read(reinterpret_cast<char *>(&data), sizeof(data));
    return tmp;
  }

  template <class Type>
  inline size_t
  write(Type data, size_t position,
        std::ios_base::seekdir mode = std::ios::beg) { // Simple write funtion
    file_.seekp(position, mode);
    size_t tmp = size_t(file_.tellp());
    file_.write(reinterpret_cast<char *>(&data), sizeof(data));
    return tmp;
  }

  inline ValueType
  Split(Node *p,
        size_t p_position) { // Split a full block into two parts (the methods
                             // depends on whether the Node is a leaf or not)
    Node *new_p =
        new Node; // Remember to deal with some special case (e.g p is root)
    ValueType ret_value;
    if (p->is_leaf_) { // There are actually two different place, one is that
                       // the son of last position is linked to the next Node,
      for (int i = p->last_position_ / 2, j = 0; i < p->last_position_;
           i++, j++) { // another is that the middle data of leaf Node won't
        new_p->value_[j] = p->value_[i]; // be transfered to its father Node
        new_p->son_[j] = -1;
        p->son_[i] = -1;
      }
      new_p->is_leaf_ = true;
      new_p->last_position_ = p->last_position_ - (p->last_position_ / 2);
      new_p->son_[new_p->last_position_] = p->son_[p->last_position_];
      p->son_[p->last_position_] = -1;
      p->last_position_ /= 2;
      size_t tmp_right = p->son_[p->last_position_] =
          write(new_p, 0, std::ios::end);
      size_t tmp_left = write(p, p_position);
      ret_value = new_p->value_[0];
      if (p == root_) { // if the Node to be split is the root Node, then we
                        // will have to add a new Node as new root
        root_ = new Node;
        root_->value_[0] = ret_value;
        root_->son_[0] = tmp_left;
        root_->son_[1] = tmp_right;
        root_->last_position_ = 1;
        root_position_ = write(root_, 0, std::ios::end);
        write(root_position_, 0);
        delete p;
      }
    } else {
      for (int i = p->last_position_ / 2 + 1, j = 0; i < p->last_position_;
           i++, j++) {
        new_p->value_[j] = p->value_[i];
        new_p->son_[j] = p->son_[i];
        p->son_[i] = -1;
      }
      new_p->last_position_ = p->last_position_ - (p->last_position_ / 2) - 1;
      new_p->son_[new_p->last_position_] = p->son_[p->last_position_];
      p->last_position_ /= 2;
      size_t tmp_right = p->son_[p->last_position_ + 1] = write(new_p, 0, std::ios::end);
      size_t tmp_left = write(p, p_position);
      ret_value = p->value_[p->last_position_];
      if (p == root_) { // if the Node to be split is the root Node, then we
                        // will have to add a new Node as new root
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

  Pair<bool, ValueType>
  InsertNode(ValueType value, Node *p,
             size_t p_position =
                 0) {   // Insert a value into the B+ tree(well I might have to
    if (p == nullptr) { // write a funtion for client to use)
      p = root_;
      p_position = root_position_;
    }
    int new_son = -1;
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
      Pair<bool, ValueType> ret =
          InsertNode(value, next_p, p->son_[next_p_position]);
      if (!ret.first) {
        delete next_p;
        return ret;
      } else {
        value = ret.second;
        new_son = next_p->is_leaf_ ? next_p->son_[next_p->last_position_]
                                   : next_p->son_[next_p->last_position_ + 1];
        delete next_p;
      }
    }
    for (int i = 0; i <= p->last_position_; i++) {
      if (i == p->last_position_) {
        p->value_[p->last_position_] = value;
        p->son_[p->last_position_ + 1] =
            p->is_leaf_ ? p->son_[p->last_position_] : new_son;
        p->son_[p->last_position_] =
            p->is_leaf_ ? -1 : p->son_[p->last_position_];
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
      return Pair<bool, ValueType>(true, Split(p, p_position));
    } else {
      write(p, p_position);
      return Pair<bool, ValueType>(false, ValueType());
    }
  }

  // There are lots of points that need concerning
  // p_relative_position stores the position of p in its
  Pair<bool, ValueType>
  DeleteNode(ValueType value, Node *p_father, Node *p,
             size_t p_relative_position) { // Delete a value from the B+ tree
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
      Pair<bool, ValueType> ret;
      try {
        ret = DeleteNode(value, p, next_p, next_p_position);
      } catch (Exception *error) {
        delete next_p;
        throw error;
      }
      delete next_p;
      if (ret.first) {
        if (next_p_position > 0 && p->value_[next_p_position - 1] == value) {
          p->value_[next_p_position - 1] =
              ret.second; // Replace the value that has been deleted
          ret.first = false;
        }
      }
      if (p->last_position_ < std::ceil(SIZE_OF_BLOCK / 2.0) - 1) {
        if (p_father == nullptr) {
          if (p->last_position_ == 0) {
            root_ = new Node;
            root_position_ = read(root_, p->son_[0]);
            write(root_position_, 0);
            delete p;
            return Pair<bool, ValueType>(false, ValueType());
          }
          write(p, root_position_);
          return Pair<bool, ValueType>(false, ValueType());
        }
        Node *bro = new Node;
        int bro_position, book = 3;
        if (p_relative_position != 0) { // If p has a previous brother
          bro_position = read(bro, p_father->son_[p_relative_position - 1]);
          book = 1;
          if (bro->last_position_ >
              std::ceil(SIZE_OF_BLOCK / 2.0) -
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
            p->value_[0] = p_father->value_[p_relative_position -
                                            1]; // Change the key in father
            p_father->value_[p_relative_position - 1] =
                bro->value_[bro->last_position_ - 1];
            --bro->last_position_;
            write(p, p_father->son_[p_relative_position]);
            write(bro, bro_position);
            book = 0;
          }
        }
        if (book &&
            p_relative_position <
                p_father
                    ->last_position_) { // If p has no previous brother, then
                                        // there must be a brother behind to it
          bro_position = read(bro, p_father->son_[p_relative_position + 1]);
          book = 2;
          if (bro->last_position_ >
              std::ceil(SIZE_OF_BLOCK / 2.0) -
                  1) { // Check whether we can borrow a value from there
            p->last_position_++;
            p->son_[p->last_position_] = bro->son_[0];
            p->value_[p->last_position_ - 1] =
                p_father->value_[p_relative_position];
            p_father->value_[p_relative_position] =
                bro->value_[0]; // Change the key in father
            for (int i = 0; i < bro->last_position_ - 1; i++) {
              bro->value_[i] = bro->value_[i + 1];
              bro->son_[i] = bro->son_[i + 1];
            }
            --bro->last_position_;
            bro->son_[bro->last_position_] =
                bro->son_[bro->last_position_ +
                          1]; // Adjust the value of brother node
            write(p, p_father->son_[p_relative_position]);
            write(bro, bro_position);
            book = 0;
          }
        }
        if (book && bro->last_position_ ==
                        std::ceil(SIZE_OF_BLOCK / 2.0) -
                            1) { // To merge two non-leaf nodes, you have pull a
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
        delete bro;
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
      for (; delete_value_position < p->last_position_;
           delete_value_position++) { // Find the value to be delete
        if (!(p->value_[delete_value_position] < value)) {
          break;
        }
      }
      if (delete_value_position == p->last_position_ ||
          !(p->value_[delete_value_position] == value)) {
        throw new IllegalDelete();
      }
      if (p->last_position_ ==
          SIZE_OF_BLOCK / 2) { // Devide circumstances into two condition
        if (p_father ==
            nullptr) { // If p is the root, then nothing have to be done
          for (int i = delete_value_position; i < p->last_position_ - 1; i++) {
            p->value_[i] = p->value_[i + 1];
            p->son_[i] = p->son_[i + 1];
          }
          p->son_[p->last_position_ - 1] = p->son_[p->last_position_];
          p->last_position_--;
          return Pair<bool, ValueType>(false, ValueType());
        }
        Node *bro = new Node;
        int bro_position, book = 3;
        if (p_relative_position != 0) { // If p has a previous brother
          bro_position = read(bro, p_father->son_[p_relative_position - 1]);
          book = 1;
          if (bro->last_position_ >
              SIZE_OF_BLOCK /
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
            delete bro;
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
              SIZE_OF_BLOCK /
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
            delete bro;
            book = 0;
          }
        }
        if (book &&
            bro->last_position_ ==
                SIZE_OF_BLOCK / 2) { // To merge two leaf nodes, you have to
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
          delete bro;
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
      write(root_position_, 0);
    }
  }

  ~BpTree() {
    delete root_;
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
      DeleteNode(ValueType(key, data), nullptr, root_, root_position_);
    } catch (Exception *error) {
      throw error;
    }
  }

  void Find(
      Key key) { // Find those value whose key equals to key that input by users
    Node p = *root_;
    while (!p.is_leaf_) {
      for (int i = 0; i <= p.last_position_; i++) {
        if (i == p.last_position_) {
          read(p, p.son_[p.last_position_]);
          break;
        } else {
          if (!(p.value_[i].key_ < key)) {
            read(p, p.son_[i]);
            break;
          }
        }
      }
    }
    int pos = 0;
    bool book = false;
    while (pos < p.last_position_ && !(p.value_[pos].key_ == key) &&
           p.value_[pos].key_ < key) {
      ++pos;
      if (pos == p.last_position_) {
        if (p.son_[p.last_position_] == -1) {
          throw new NothingFind();
        }
        read(p, p.son_[p.last_position_]);
        pos = 0;
      }
    }
    while (pos < p.last_position_ && p.value_[pos].key_ == key) {
      std::cout << p.value_[pos].value_ << " ";
      book = true;
      ++pos;
      if (pos >= p.last_position_) {
        if (p.son_[p.last_position_] == -1) {
          return;
        }
        read(p, p.son_[p.last_position_]);
        pos = 0;
      }
    }
    if (!book) {
      throw new NothingFind();
    }
  }
};

struct Index {
  char c[65] = {'\0'};

  Index() {}

  bool operator<(Index &rhs) { return (std::strcmp(c, rhs.c) < 0); }

  bool operator==(Index &rhs) { return !(std::strcmp(c, rhs.c)); }
};

int main() {

  BpTree<Index, int> test;

  int n;

  std::cin >> n;

  for (int i = 1; i <= n; i++) {
    std::string command;

    std::cin >> command;

    if (command == "insert") {
      Index index;
      int data;

      std::cin >> index.c >> data;

      test.Insert(index, data);
    } else if (command == "delete") {
      Index index;
      int data;

      std::cin >> index.c >> data;
      try {
        test.Delete(index, data);
      } catch (Exception *error) {
        delete error;
      }
    } else {
      Index index;

      std::cin >> index.c;
      try {
        test.Find(index);
        std::cout << std::endl;
      } catch (Exception *error) {
        error->what();
        delete error;
      }
    }
  }

  // for (int j = 0; j <=100; j++) {
  // for (int i = 1; i <= 9; i++) {
  //   test1.Insert(i, i + 1);
  // }
  // }

  // try {
  //   test1.Find(10);
  // } catch (Exception *error) {
  //   error->what();
  //   delete error;
  // }
  return 0;
}