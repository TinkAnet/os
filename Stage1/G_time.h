#ifndef _TIME_H
#define _TIME_H

#define le(a, b) (a.str < b.str)
#define eq(a, b) (a.str == b.str)
#define ge(a, b) (a.str > b.str)

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "G_Debug.h"
/**
 * Init start&end time & how many slots per hour
 * 
 */

#define apo_start_hour 18
#define apo_end_hour 23
#define slot_per_hour 1

#define dosth(x) do{ \
    x.a ++; \
} while(0);

/**
 * Date information
 * 
 */
typedef struct DATE
{
    long long str;
    int year, month, day;
}Date;

/**
 * @brief convert param str to a date format
 * eg: long long str = 20130101 to (year=2013,month=01,day=01)
 * 
 * 
 * @param str the date
 * @return Date 
 */
Date str2Date (long long str);

/**
 * @brief convert date to a value of type "long long"
 * eg: (2013,01,01) to value = 20130101
 * 
 * @param a of type "Date"
 * @return long long 
 */
long long date2Str (Date a);

/**
 * @brief add one day based on current date
 *        obtain new date 
 * @param a o f type "Date"
 * @return Date 
 */
Date add_day (Date a);

/**
 * calculate how many days has past from a strat_date to Date a
 */
int days(Date a);
void date_print (Date a);

/**
 * Time information
 */
typedef struct TIME
{
    long long str;
    Date d;
    int hour, minute;
}Time;

/**
 * @brief convert param a to TIME format
 * eg: a = 1800 to (18:00)
 * 
 * @param a of type "long long"
 * @return Time 
 */
Time str2Time (long long a);

/**
 * @brief convert time to a value of type "long long"
 * @return long long 
 */
long long time2Str (Time a);

/**
 * convert Time a to slot
 */
int time2Slot(Time a);

/**
 * 
 * @param a based Time a
 * @param hours the hours that to be added
 * @param minutes the minutes that to be added
 * @return Time (new Time) 
 */
Time add_time(Time a, int hours, int minutes);
void time_print (Time a);

/**
 * Event time period
 */
typedef struct PERIOD
{
    int st, ed; // Slot id
    Time start_time, end_time;
}Period;

/**
 * Init start time & end time
 * make time-to-slot convertion easily
 */
void init_time_slot(long long Start, long long End);

/**
 * judge whether 2 periods are conflict
 * 
 * - true: there exists a confliction 
 * - false: there is no confliction
 */
bool period_conflict ( Period a, Period b );

/**
 * obtain the slot id of this period
 * @param a the event period
 * @return int 
 */
int period2Slot ( Period a );

/**
 * 
 * @param l start time
 * @param r end time
 * @return Period 
 */
Period str2Period (long long l, long long r);
void period_print ( Period a );

#else

#endif
