#include "input_processer.hpp"

void Tokenscanner::SetLine(const std::string &line) {
  line_ = line;
}

bool Tokenscanner::HasMoreToken() {
  while (line_[pos] == ' ' && pos < line_.size()) {
    pos++;
  }
  if (pos != line_.size()) {
    return true;
  } else {
    return false;
  }
}

std::string Tokenscanner::NextToken() {
  std::string ret;
  while (line_[pos] == ' ') {
    pos++;
  }
  while (line_[pos] != ' ') {
    ret.push_back(line_[++pos]);
  }
  return ret;
}

int TokenProcesser::StringToInt(std::string token) {
  return stoi(token);
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