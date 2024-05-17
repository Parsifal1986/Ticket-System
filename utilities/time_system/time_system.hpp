#ifndef TIMESYSTEM_HPP
#define TIMESYSTEM_HPP

const int kDaysOfMonths[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

struct Time {
  int month;
  int day;
  int hour;
  int minute;

  void AddTime(int add_month, int add_day, int add_hour, int add_minute);
};

#endif //TIMESYSTEM_HPP