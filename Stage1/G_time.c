#include "G_time.h"
// todo : ʹ�� LINUX TIME У�� ����
const int N = 100;
// �� header �г����� define ������.
//const int apo_start_hour = 18, apo_end_hour = 23, slot_per_hour =1;

// init days in each month with an array
int mth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//init some poblic holidays
const int days_of_holidays = 6;
Time holidays[] = {{20230305, 2023, 3, 5}, {20230312, 2023, 3, 12}, {20230319, 2023, 3, 19}, {20230326, 2023, 3, 26}, {20230329, 2023, 3, 29}, {20230329, 2023, 3, 30}};
Date start_day, end_day;

/**
 * separate the date_value to year,month,day 
 */
Date str2Date(long long a) {
    if(DEBUG_ALL) printf("str2Date: %lld\n", a);
    Date ret;
    ret.str = a;
    ret.year = a / 10000;// obtain year
    ret.month = (a / 100) % 100;// obtain month
    ret.day = a % 100;// obtain day
    if(DEBUG_ALL){ printf("Date: "); date_print(ret);printf("\n");}
    return ret;
}

/**
 * combine year,month,day and obtain the date_value
 */
long long date2Str(Date a) { return a.year * 10000ll + a.month * 100 + a.day; }

/**
 * Add one day on Date a,obtain new date
 * 
 */
Date add_day (Date a) {
    if(a.month == 2 && a.day == 28){
        // leap year judgement
        if((a.year % 4 == 0 && a.year % 100 != 0) || a.year % 400 == 0) a.day = 29;
        else{ a.month = 3; a.day = 1; }
        return a;
    }

    a.day++;

    // days out of range,enter into next month/ next year
    if(a.day > mth[a.month]){
        a.day = 1;
        a.month++;
        if(a.month > 12){
            a.month = 1;
            a.year++;
        }
    }
    a.str = date2Str(a);
    return a;
}

/**
 * Get the number of working days from start_date to Date a
 * 
 */
int days(Date a){
    int cnt = 0;
    for(Date i = start_day; !eq(i, a); i = add_day(i)){
        //if(DEBUG_ALL){ date_print(i); date_print("a");printf("\n");}
        cnt++;
        for(int j = 0; j < days_of_holidays; j++){
            if(eq(i, holidays[j])){
                cnt--;// exclude holiday
                break;
            }
        }
    }
    return cnt;
}

/**
 * print date
 * 
 */
void date_print(Date a) { printf("%04d-%02d-%02d", a.year, a.month, a.day); }

/**
 * separate time_value to date,hour,minute
 * 
 */
Time str2Time(long long a){
    Time ret;
    ret.str = a;
    ret.d = str2Date(a/10000);//obtain date
    ret.hour = (a / 100) % 100;//obtain hour
    ret.minute = a % 100;//obtain minute
    return ret;
}

/**
 * combine date,hour,minute to obtain time_value
 * 
 */
long long time2Str(Time a){ return date2Str(a.d) * 10000 + a.hour * 100 + a.minute; }

/**
 *  Add a period of time on Time a
 *
 */
Time add_time(Time a, int hours, int minutes){
    a.minute += minutes;
    hours += a.minute / 60;
    a.minute %= 60;
    a.hour += hours;
    while(a.hour >= 24){
        //if hour greater than 24, add a day
        a.hour -= 24;
        a.d = add_day(a.d);
    }
    return a;
}

/**
 * print Time
 * 
 */
void time_print(Time a){ date_print(a.d); printf(" %02d:%02d", a.hour, a.minute); }

/**
 * @brief Set the Date object
 * 
 * @param year 
 * @param month 
 * @param date 
 * @return Date 
 */
Date setDate(int year, int month, int date){
    Date ret;
    ret.year = year; ret.month = month; ret.day = date;
    return ret;
}

/**
 * Init time slot
 */
void init_time_slot(long long Start, long long End){
    start_day = str2Date(Start);
    end_day = str2Date(End);
}

/**
 * check whether there's a conflict
 */
bool period_conflict(Period a, Period b){
    return !(a.ed <= b.st || a.st >= b.ed);
}

/**
 * Set a Period object
 * 
 */
Period str2Period(long long l, long long r){
    if(DEBUG_ALL) printf("str2Period: %lld %lld\n", l, r);
    Period ret;
    ret.start_time = str2Time(l);
    ret.end_time = str2Time(r);
    ret.st = time2Slot(ret.start_time);
    ret.ed = time2Slot(ret.end_time);
    if(DEBUG_ALL){ printf("Period: "); period_print(ret);printf("\n");}
    return ret;
}

/**
 * obtain slot id
 */
int time2Slot(Time a){
    //if(DEBUG_ALL) printf("holidays: %d\n", days_of_holidays);
    for(int i = 0; i < days_of_holidays; i++){
        if(eq(a, holidays[i])){
            return -1;
        }
    }
    int base = days(a.d);
    return (base * (apo_end_hour - apo_start_hour) + (a.hour - apo_start_hour+1)) * slot_per_hour;
}

/**
 * print period
 * 
 */
void period_print(Period a){
    time_print(a.start_time); printf("(%d) -> ",a.st); time_print(a.end_time); printf("(%d)", a.ed);
}
