#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <iostream>

class Exception : public std::exception {
public:
  Exception() {}

  virtual ~Exception() {}

  virtual void what() = 0;
};

class IllegalInsert : public Exception {
public:
  IllegalInsert() {}

  ~IllegalInsert() {}

  void what() override {
    std::cout << "Insert Fail!" << std::endl;
  }
};

class IllegalDelete : public Exception {
public:
  IllegalDelete() {}

  ~IllegalDelete() {}

  void what() override {
    std::cout << "Delete Fail!" << std::endl;
  }
};

#endif // EXCEPTIONS_HPP