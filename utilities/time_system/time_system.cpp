#include "time_system.hpp"

Time& Time::AddTime(int add_month, int add_day, int add_hour, int add_minute) {
  minute += add_minute; add_hour += minute / 60;                 minute %= 60;
  hour += add_hour;     add_day += hour / 24;                    hour %= 24;
  day += add_day;       add_month += day / kDaysOfMonths[month]; day %= kDaysOfMonths[month];
  month += add_month;
  return *this;
}

bool Time::InOneDay(const Time &rhs) { return month == rhs.month && day == rhs.day; }

bool Time::LessInDate(const Time &rhs) {return month <= rhs.month && day <= rhs.day; }

bool Time::operator<(const Time &other) {
  return (month == other.month ? (day == other.day ? (hour == other.hour ? minute < other.minute : hour < other.hour) :
          day < other.day) : month < other.month);
}

bool Time::operator==(const Time &other) {
  return month == other.month && day == other.day && hour == other.hour && minute == other.minute;
}

bool Time::operator<=(const Time &other) {
  return *this < other || *this == other;
}