#include "time_system.hpp"
#include <iomanip>

Time& Time::AddTime(int add_month, int add_day, int add_hour, int add_minute) {
  minute += add_minute; add_hour += minute / 60;                 minute %= 60;
  hour += add_hour;     add_day += hour / 24;                    hour %= 24;
  day += add_day;
  if (day > kDaysOfMonths[month]) {
    add_month += day / kDaysOfMonths[month]; day %= kDaysOfMonths[month];
  }
  month += add_month;
  return *this;
}

Time Time::AddTime_tmp(int add_month, int add_day, int add_hour, int add_minute) {
  Time tmp(*this);
  return tmp.AddTime(add_month, add_day, add_hour, add_minute);
}

Time& Time::MinusTime(int minus_month, int minus_day, int minus_hour, int minus_minute) {
  minute -= minus_minute;
  if (minute < 0) { minute += 60; --hour; }
  hour -= minus_hour;
  if (hour < 0 && day > 0) { hour += 24; --day; }
  day -= minus_day;
  if (day < 1 && month > minus_month) { day += kDaysOfMonths[--month]; }
  month -= minus_month;
  return *this;
}

bool Time::InOneDay(const Time &rhs) { return month == rhs.month && day == rhs.day; }

bool Time::LessInDate(const Time &rhs) {return month <= rhs.month && day <= rhs.day; }

bool Time::empty() const { return month == 0 && day == 0 && hour == 0 && minute == 0; }

bool Time::operator<(const Time &other) const {
  return (month == other.month ? (day == other.day ? (hour == other.hour ? minute < other.minute : hour < other.hour) :
          day < other.day) : month < other.month);
}

bool Time::operator==(const Time &other) const {
  return month == other.month && day == other.day && hour == other.hour && minute == other.minute;
}

bool Time::operator<=(const Time &other) const {
  return *this < other || *this == other;
}

Time Time::operator+(const Time &other) const {
  Time tmp(*this);
  return tmp.AddTime(other.month, other.day, other.hour, other.minute);
}

Time Time::operator-(const Time &other) const {
  Time tmp(*this);
  return tmp.MinusTime(other.month, other.day, other.hour, other.minute);
}

std::ostream &operator<<(std::ostream &output, Time &time) {
  output << std::setw(2) << std::setfill('0') << time.month << "-"
         << std::setw(2) << std::setfill('0') << time.day << " "
         << std::setw(2) << std::setfill('0') << time.hour << ":"
         << std::setw(2) << std::setfill('0') << time.minute;
  return output;
}