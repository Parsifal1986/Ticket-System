#ifndef TIMESYSTEM_HPP
#define TIMESYSTEM_HPP
#include <iostream>

const int kDaysOfMonths[13] = {0x7fffffff, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

struct Time {
  friend std::ostream& operator << (std::ostream&, Time&);
  
  Time() {
    month = day = hour = minute = 0;
  }

  Time(const Time &other) {
    if (this == &other) {
      return;
    }
    this->month = other.month;
    this->day = other.day;
    this->hour = other.hour;
    this->minute = other.minute;
  }

  Time(int month_, int day_, int hour_, int minute_) {
    month = month_; day = day_; hour = hour_; minute = minute_;
  }

  int month, day, hour, minute;

  Time& AddTime(int add_month, int add_day, int add_hour, int add_minute);

  Time AddTime_tmp(int add_month, int add_day, int add_hour, int add_minute);

  Time& MinusTime(int minus_month, int minus_day, int minus_hour, int minus_minute);

  bool InOneDay(const Time &rhs);

  bool LessInDate(const Time &rhs);

  bool empty() const;

  bool operator<(const Time &other) const;

  bool operator==(const Time &other) const;

  bool operator<=(const Time &other) const;

  Time operator+(const Time &other) const;

  Time operator-(const Time &other) const;
};

#endif //TIMESYSTEM_HPP