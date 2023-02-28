#include "G_AP.h"

char level_str[level_max][level_name_length] = {"None", "gathering", "groupStudy", "projectMeeting", "privateTime"};

void init_all(long long start_date, long long end_date, int number_of_users, char users_name[][name_length]){
    init_time_slot(start_date, end_date);
    init_user(number_of_users, users_name);
}

Appointment load_ap(char *levels, char *caller_name, long long date_str, long long time_str, double period_length, int number_of_callees, char callee_name[][name_length])
{
    if(DEBUG_ALL) printf("load_ap\n");
    Appointment ret;
    ret.level = 0;
    for(int i = 0; i < level_max; i++){
        bool ifsame = true;
        for(int j = 0; levels[j] != 0 || level_str[i][j] != 0; j++){
            if(levels[j] != level_str[i][j]){
                ifsame = false;
                break;
            }
        }
        if(ifsame){
            ret.level = i;
            break;
        }
    }
    ret.len = ceil(slot_per_hour * period_length);

    ret.p.start_time = str2Time(date_str * 10000 + time_str);
    int _tmp = ceil(period_length * 100);
    int _hour = _tmp / 100;
    int _minute = ceil((_tmp % 100) / 100.0 * 60);
    ret.p.end_time = add_time(ret.p.start_time, _hour, _minute);
    if(DEBUG_ALL) printf("???\n");
    ret.p.st = time2Slot(ret.p.start_time);
    ret.p.ed = ret.p.st + ret.len;
    if(DEBUG_ALL) printf("???\n");
    ret.caller = load_user(caller_name);
    ret.number = number_of_callees;
    for(int i = 0; i < number_of_callees; i++){
        ret.callee[i] = load_user(callee_name[i]);
    }

    ret.ifconfirmed = false;

    return ret;
}

Appointment set_if_confirmed(Appointment a, bool flags){
    a.ifconfirmed = flags;
    return a;
}

bool ap_conflict(Appointment a, Appointment b){
    return period_conflict(a.p, b.p);
}

bool contains(Appointment a, User b){
    if(same(a.caller, b)) return true;
    for(int i = 0; i < a.number; i++){
        if(same(a.callee[i], b)) return true;
    }
    return false;
}

void ap_print(Appointment a){
    printf("< %s || Period: ", level_str[a.level]);
    period_print(a.p);
    printf(" || Caller: ");
    user_print(a.caller);
    printf(" || Number of Callees: %d || Callees: ", a.number);
    for(int i = 0; i < a.number; i++){
        user_print(a.callee[i]);
        printf(" ");
    }
    if(a.ifconfirmed) printf("|| Confirmed >");
    else printf("|| Not confirmed >");
}
