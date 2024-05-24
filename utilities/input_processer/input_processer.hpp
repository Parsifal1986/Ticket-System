#ifndef INPUTPROCESSER_HPP
#define INPUTPROCESSER_HPP

#include <cstring>
#include <iostream>
#include <cstdio>
#include <string>
#include "../parameter_type/parameter_type.hpp"
#include "../time_system/time_system.hpp"
#include "../pair/pair.hpp"

class TokenProcesser {
public:
  TokenProcesser() = default;

  ~TokenProcesser() = default;

  int StringToInt(std::string token);

  void DevidedBySlash(std::string token, char array[][31]);

  void DevidedBySlash(std::string token, int *array, int start_pos = 0);
  
  Time ProcessTime(std::string token);

  sjtu::pair<Time, Time> ProcessDate(std::string token);

  Time ProcessSingleDate(std::string token);

  int ReadTimeStamp(std::string token);

  ParameterType CheckParameter(std::string token, EnumType enum_type);
};

class TokenScanner {
public:
  TokenScanner() = default;

  TokenScanner(const std::string &line) : line_(line) {}

  ~TokenScanner() = default;

  void SetLine(const std::string &line);

  std::string NextToken();

  bool HasMoreToken();

private:
  std::string line_;
  TokenProcesser token_processer_;
  int pos;
};

#endif //INPUTPROCESSER_HPP