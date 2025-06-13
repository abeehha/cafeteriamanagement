#define _CRT_SECURE_NO_WARNINGS
#include "Date.h"
#include <ctime>

const AYstr Date::monthNames[12] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};
Date::Date(int d, int m, int y) : day(d), month(m), year(y) {}
int Date::getDay() const {
    return day;
}
int Date::getMonth() const {
    return month;
}
int Date::getYear() const {
    return year;
}
AYstr Date::getFormattedDate() const {
    AYstr result;
    result += AYstr(day) + "-" + AYstr(month) + "-" + AYstr(year);
    return result;
}

AYstr Date::getMonthName() const {
    return monthNames[month - 1];
}
bool Date::isSameDay(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
}

bool Date::isSameMonth(const Date& other) const {
    return month == other.month && year == other.year;
}
 Date Date::getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    return Date(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
}