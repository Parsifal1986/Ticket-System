#include "input_processer.hpp"
#include <string>

void TokenScanner::SetLine(const std::string &line) {
  line_ = line;
  pos = 0;
}

bool TokenScanner::HasMoreToken() {
  while (line_[pos] == ' ' && pos < line_.size()) {
    pos++;
  }
  if (pos != line_.size()) {
    return true;
  } else {
    return false;
  }
}

std::string TokenScanner::NextToken() {
  std::string ret;
  while (line_[pos] == ' ' && pos < line_.size()) {
    pos++;
  }
  while (line_[pos] != ' ' && pos < line_.size()) {
    ret.push_back(line_[pos++]);
  }
  return ret;
}

int TokenProcesser::StringToInt(std::string token) {
  return stoi(token);
}

void TokenProcesser::DevidedBySlash(std::string token, char array[][31]) {
  int pos = 0, array_pos = 0, ret_pos = 0;
  while (pos < token.size()) {
    if (token[pos] == '|') {
      ++array_pos; ++pos;
      ret_pos = 0;
    } else {
      array[array_pos][ret_pos++] = token[pos++];
    }
  }
  return;
}

void TokenProcesser::DevidedBySlash(std::string token, int *array) {
  int pos = 0, array_pos = 0;
  std::string array_number;
  while (pos < token.size()) {
    if (token[pos] == '-') {
      return;
    }
    if (token[pos] == '|') {
      array[array_pos++] = StringToInt(array_number);
      array_number.clear();
      pos++;
    } else {
      array_number.push_back(token[pos++]);
    }
  }
  array[array_pos] = StringToInt(array_number);
  return;
}

Time TokenProcesser::ProcessTime(std::string token) {
  Time ret;
  int pos = 0;
  std::string number;
  while (pos < token.size()) {
    if (token[pos] == ':') {
      ret.hour = StringToInt(number);
      pos++;
      number.clear();
    } else {
      number.push_back(token[pos++]);
    }
  }
  ret.minute = StringToInt(number);
  return ret;
}

sjtu::pair<Time, Time> TokenProcesser::ProcessDate(std::string token) {
  sjtu::pair<Time, Time> ret;
  int pos = 0;
  std::string number;
  Time tmp_time;
  while (pos < token.size()) {
    if (token[pos] == '-') {
      tmp_time.month = StringToInt(number);
      pos++;
      number.clear();
    } else if (token[pos] == '|') {
      tmp_time.day = StringToInt(number);
      ret.first = tmp_time;
      pos++;
      number.clear();
    } else {
      number.push_back(token[pos++]);
    }
  }
  tmp_time.day = StringToInt(number);
  ret.second = tmp_time;
  return ret;
}

int TokenProcesser::ReadTimeStamp(std::string token) {
  return stoi(token.substr(1, token.size() - 2));
}

ParameterType TokenProcesser::CheckParameter(std::string token, EnumType enum_type) {
  if (token[0] == '-') {
    switch (token[1]) {
      case 'c' : {
        return CURRENT_USER;
      }
      case 'u': {
        return USERNAME;
      }
      case 'p' : {
        if (enum_type == USER) {
          return PASSWORD;
        } else if (enum_type == TRAIN) {
          return PRICES;
        } else {
          return UNDEFINED_PARAMETER;
        }
      }
      case 'n' : {
        if (enum_type == USER) {
          return NAME;
        } else if (enum_type == TRAIN) {
          return STATION_NUM;
        } else {
          return UNDEFINED_PARAMETER;
        }
      }
      case 'm' : {
        if (enum_type == USER) {
          return MAIL_ADDRESS; 
        } else if (enum_type == TRAIN) {
          return SEAT_NUM;
        } else {
          return UNDEFINED_PARAMETER;
        }
      }
      case 'g' : {
        return PRIVILEGE;
      }
      case 'i' : {
        return TRAIN_ID;
      }
      case 's': {
        return STATIONS;
      }
      case 'x' : {
        return START_TIME;
      }
      case 't' : {
        return TRAVEL_TIME;
      }
      case 'o' : {
        return STOPOVER_TIME;
      }
      case 'd' : {
        return SALEDATE;
      }
      case 'y' : {
        return TYPE;
      }
      case 'q' : {
        return OPTION;
      }
      default: {
        return UNDEFINED_PARAMETER;
      }
    }
  } else {
    return NOT_A_PARAMETER;
  }
}