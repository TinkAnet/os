#ifndef _TIME_H
#define _TIME_H

#define le(a, b) a.str < b.str
#define eq(a, b) a.str == b.str
#define ge(a, b) a.str > b.str

#define apo_start_hour 18
#define apo_end_hour 23
#define slot_per_hour 1

#define dosth(x) do{ \
    x.a ++; \
} while(0);


struct Date
{
    long long str;
    int year, month, day;
};
Date str2Date (long long str);
long long date2Str (Date a);
Date add_day (Date a);
int days(Date a);
void date_print (Date a);

struct Time
{
    long long str;
    Date d;
    int hour, minute;
};

Time str2Time (long long a);
long long time2Str (Time a);
Time add_time(Time a, int hours, int minutes);
void time_print (Time a);

struct Period
{
    int st, ed; // Slot id
    Time start_time, end_time;
};

void init_time_slot(long long Start, long long End);
bool period_conflict ( Period a, Period b );
int period2Slot ( Period a );
Period str2Period (long long l, long long r);
void period_print ( Period a );

#else

#endif
