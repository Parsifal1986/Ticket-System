#ifndef TIMESYSTEM_HPP
#define TIMESYSTEM_HPP

const int kDaysOfMonths[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

struct Time {
  Time() {
    month = day = hour = minute = 0;
  }

  Time(int month_, int day_, int hour_, int minute_) {
    month = month_; day = day_; hour = hour_; minute = minute_;
  }

  int month, day, hour, minute;

  Time& AddTime(int add_month, int add_day, int add_hour, int add_minute);

  bool InOneDay(const Time &rhs);

  bool LessInDate(const Time &rhs);

  bool operator<(const Time &other);

  bool operator==(const Time &other);

  bool operator<=(const Time &other);
};

#endif //TIMESYSTEM_HPP