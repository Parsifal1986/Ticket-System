#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <iostream>

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

#endif // EXCEPTIONS_HPP