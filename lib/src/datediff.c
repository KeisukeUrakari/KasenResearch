/**
 * @file datediff.c
 * @author BeNest
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../datediff.h"

bool checkleap(int year) {
    if((year % 4) != 0) {
        return false;
    } else if(((year % 100) == 0) && ((year % 400) != 0)) {
        return false;
    }

    return true;
}

static bool checkday(int year, int month, int day) {
    static const int days_normal[] = {-1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    static const int days_leap[] = {-1, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if((day < 1) || (day > (checkleap(year) ? days_leap[month] : days_normal[month]))) {
        return false;
    }

    return true;
}

bool checkdate(int year, int month, int day) {
    if(year < 1900 || year > 2100) {
        return false;
    } else if(month < 1 || month > 12) {
        return false;
    } else if(!checkday(year, month, day)) {
        return false;
    }

    return true;
}

int compdate(int year1, int month1, int day1, int year2, int month2, int day2) {
    if(year1 < year2) {
        return -1;
    } else if(year1 > year2) {
        return 1;
    } else if(month1 < month2) {
        return -1;
    } else if(month1 > month2) {
        return 1;
    } else if(day1 < day2) {
        return -1;
    } else if(day1 > day2) {
        return 1;
    }

    return 0;
}

void incdate(int *year, int *month, int *day) {
    *day += 1;
    if(!checkday(*year, *month, *day)) {
        *day = 1;
        *month += 1;
        if(*month > 12) {
            *month = 1;
            *year += 1;
        }
    }
}

int datediff(int year1, int month1, int day1, int year2, int month2, int day2) {
    if(compdate(year1, month1, day1, year2, month2, day2) > 0) {
        return -1;
    }

    int days = 0;

    for(int y = year1, m = month1, d = day1; compdate(y, m, d, year2, month2, day2); ++days) {
        incdate(&y, &m, &d);
    }

    return days;
}
