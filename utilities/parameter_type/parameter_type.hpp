#ifndef ParameterType_HPP
#define ParameterType_HPP

enum ParameterType {
  CURRENT_USER,
  USERNAME,
  PASSWORD,
  NAME,
  MAIL_ADDRESS,
  PRIVILEGE,
  TRAIN_ID,
  STATION_NUM,
  START_TIME,
  SEAT_NUM,
  STATIONS,
  PRICES,
  TRAVEL_TIME,
  STOPOVER_TIME,
  SALEDATE,
  TYPE,
  HELP,
  UNDEFINED_PARAMETER,
  NOT_A_PARAMETER,
  NUMBER,
  START_STATION,
  END_STATION,
  OPTION
};

enum EnumType {
  USER,
  TRAIN,
  SELLING,
  OTHER
};

enum Status {
  SUCCESS = 0,
  PENDING = 1,
  REFUNDED = 2
};

#endif //ParameterType_HPP
