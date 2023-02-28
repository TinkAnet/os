#include "G_time.h"
// todo : 使用 LINUX TIME 校验 输入
const int N = 100;
// 在 header 中尝试用 define 来定义.
//const int apo_start_hour = 18, apo_end_hour = 23, slot_per_hour =1;
int mth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const int days_of_holidays = 6;
Time holidays[] = {{20230305, 2023, 3, 5}, {20230312, 2023, 3, 12}, {20230319, 2023, 3, 19}, {20230326, 2023, 3, 26}, {20230329, 2023, 3, 29}, {20230329, 2023, 3, 30}};
Date start_day, end_day;


Date str2Date(long long a) {
    if(DEBUG_ALL) printf("str2Date: %lld\n", a);
    Date ret;
    ret.str = a;
    ret.year = a / 10000;
    ret.month = (a / 100) % 100;
    ret.day = a % 100;
    if(DEBUG_ALL){ printf("Date: "); date_print(ret);printf("\n");}
    return ret;
}

long long date2Str(Date a) { return a.year * 10000ll + a.month * 100 + a.day; }

Date add_day (Date a) {
    if(a.month == 2 && a.day == 28){
        if((a.year % 4 == 0 && a.year % 100 != 0) || a.year % 400 == 0) a.day = 29;
        else{ a.month = 3; a.day = 1; }
        return a;
    }

    a.day++;
    if(a.day > mth[a.month]){
        a.day = 1;
        a.month++;
        if(a.month > 12){
            a.month = 1;
            a.year++;
        }
    }
    return a;
}

int days(Date a){
    int cnt = 0;
    for(Date i = start_day; !eq(i, a); i = add_day(i)){
        cnt++;
        for(int j = 0; j < days_of_holidays; j++){
            if(eq(i, holidays[j])){
                cnt--;
                break;
            }
        }
    }
    return cnt;
}

void date_print(Date a) { printf("%04d-%02d-%02d", a.year, a.month, a.day); }

Time str2Time(long long a){
    Time ret;
    ret.str = a;
    ret.d = str2Date(a/10000);
    ret.hour = (a / 100) % 100;
    ret.minute = a % 100;
    return ret;
}

long long time2Str(Time a){ return date2Str(a.d) * 10000 + a.hour * 100 + a.minute; }

Time add_time(Time a, int hours, int minutes){
    a.minute += minutes;
    hours += a.minute / 60;
    a.minute %= 60;
    a.hour += hours;
    while(a.hour >= 24){
        a.hour -= 24;
        a.d = add_day(a.d);
    }
    return a;
}

void time_print(Time a){ date_print(a.d); printf(" %02d:%02d", a.hour, a.minute); }

Date setDate(int year, int month, int date){
    Date ret;
    ret.year = year; ret.month = month; ret.day = date;
    return ret;
}

void init_time_slot(long long Start, long long End){
    start_day = str2Date(Start);
    end_day = str2Date(End);
}

bool period_conflict(Period a, Period b){
    return !(a.ed < b.st || a.st > b.ed);
}

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

int time2Slot(Time a){
    for(int i = 0; i < days_of_holidays; i++){
        if(eq(a, holidays[i])){
            return -1;
        }
    }
    int base = days(a.d);
    return (base * (apo_end_hour - apo_start_hour) + (a.hour - apo_start_hour+1)) * slot_per_hour;
}

void period_print(Period a){
    time_print(a.start_time); printf("(%d) -> ",a.st); time_print(a.end_time); printf("(%d)", a.ed);
}
