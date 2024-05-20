#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <iostream>

class Exception {
public:
  Exception() {}

  virtual ~Exception() {}

  virtual void what() = 0;
};

class BptException : public Exception {};

class IllegalInsert : public BptException {
public:
  IllegalInsert() {}

  ~IllegalInsert() {}

  void what() override { std::cout << "In BP tree, Insert Fail!" << std::endl; }
};

class IllegalDelete : public BptException {
public:
  IllegalDelete() {}

  ~IllegalDelete() {}

  void what() override { std::cout << "In BP tree, Delete Fail!" << std::endl; }
};

class NothingFind : public BptException {
public:
  NothingFind() {}

  ~NothingFind() {}

  void what() override { std::cout << "null" << std::endl; }
};

class UserManagerException : public Exception {};

class NoSuchUser : public UserManagerException {
public:
  NoSuchUser() {}

  ~NoSuchUser() {}

  void what() override { std::cout << "The user you are looking for DOES NOT exist!"; }
};

class TrainManagerException : public Exception {};

class NoSuchTrain : public TrainManagerException {
public:
  NoSuchTrain() {};

  ~NoSuchTrain() {}

  void what() override { std::cout << "The train you are loooking for DOES NOT exist"; }
};

#endif // EXCEPTIONS_HPP