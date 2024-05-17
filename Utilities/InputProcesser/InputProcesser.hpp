#ifndef INPUTPROCESSER_HPP
#define INPUTPROCESSER_HPP

#include <cstring>
#include <iostream>
#include <cstdio>
#include <string>
#include "../ParameterType/ParameterType.hpp"

class TokenProcesser {
public:
  TokenProcesser() = default;

  ~TokenProcesser() = default;

  int StringToInt(std::string token);

  ParameterType CheckParameter(std::string token, EnumType enum_type);
};

class Tokenscanner {
public:
  Tokenscanner() = default;

  Tokenscanner(const std::string &line) : line_(line) {}

  ~Tokenscanner() = default;

  void SetLine(const std::string &line);

  std::string NextToken();

  bool HasMoreToken();

private:
  std::string line_;
  TokenProcesser token_processer_;
  int pos;
};

#endif //INPUTPROCESSER_HPP