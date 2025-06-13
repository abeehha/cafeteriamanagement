#ifndef DATE_H
#define DATE_H
#include "ayStr.h"

class Date {
    int day;
    int month;
    int year;
    static const AYstr monthNames[12];

public:
    Date(int d = 1, int m = 1, int y = 2000);
    int getDay() const;
    int getMonth() const;
    int getYear() const;
    AYstr getFormattedDate() const;
    AYstr getMonthName() const;
    bool isSameDay(const Date& other) const;
    bool isSameMonth(const Date& other) const;
    static Date getCurrentDate();
};
#endif
