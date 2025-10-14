/*
Pg 112 #5-8
*/

#include <stdio.h>

int day_of_year(int year, int month, int day);
void month_day(int year, int yearday, int *pmonth, int *pday);

static char daytab[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

int main()
{
    int m, d;

    int doy = day_of_year(1991, 12, 5);
    printf("Day of year: %d\n", doy);

    month_day(1988, 60, &m, &d);
    printf("Month: %d, Day: %d\n", m, d);

    return 0;
}

/// @brief Given day month and year returns the numercal count of that day in the year
/// @param year
/// @param month
/// @param day
/// @return Number of that day (1-365)
int day_of_year(int year, int month, int day)
{
    if (year < 1 || year > 2050)
    {
        printf("Invalid year.\n");
        return -1;
    }

    if (month < 1 || month > 12)
    {
        printf("Invalid month.\n");
        return -1;
    }

    if (day < 1 || day > 31)
    {
        printf("Invalid day.\n");
        return -1;
    }
    int i, leap;

    leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
    for (i = 1; i < month; i++)
    {
        day += daytab[leap][i];
    }
    return day;
}

/// @brief Given year and year day (1-366) sets ptr to month and date
/// @param year
/// @param yearday
/// @param pmonth
/// @param pday
void month_day(int year, int yearday, int *pmonth, int *pday)
{
    if (year < 1 || year > 2050)
    {
        printf("Invalid year.\n");
        return;
    }
    if (yearday < 1 || yearday > 366)
    {
        printf("Invalid yearday (Should be between 1 and 366).\n");
        return;
    }
    if (NULL == pmonth || NULL == pday)
    {
        printf("Invalid pointer passed in.\n");
        return;
    }

    int i, leap;

    leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
    for (i = 1; yearday > daytab[leap][i]; i++)
    {
        yearday -= daytab[leap][i];
    }
    *pmonth = i;
    *pday = yearday;
}