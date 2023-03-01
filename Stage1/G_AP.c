#include "G_AP.h"

int num_of_user = 0;
char user_name[user_max_num][name_length] = {0};

/**
 * Init all the users
 * 
 */
void init_user(int number, char s[][name_length]){
    num_of_user = number;
    for(int i = 0; i < number; i++){
        for(int j = 0; s[i][j]; j++)
            user_name[i][j] = s[i][j];
        if(user_name[i][0] > 'Z') user_name[i][0] = user_name[i][0] - 'a' + 'A';
        if(DEBUG_ALL) printf("user_name: %s \n", user_name[i]);
    }
}

/**
 * load a user(obtain all the information of a specific user)
 */
User load_user(char *s){
    if(DEBUG_ALL) printf("Load User: %s", s);
    if(s[0] > 'Z') s[0] = s[0] - 'a' + 'A';
    User ret;
    ret.uid = -1;
    for(int i = 0; i < num_of_user; i++){
        bool ifsame = true;
        for(int j = 0; s[j] || user_name[i][j]; j++){
            if(s[j] != user_name[i][j]){
                ifsame = false;
                break;
            }
        }
        if(ifsame){
            ret.uid = i;
            break;
        }
    }
    if(DEBUG_ALL){ printf("User: "); user_print(ret); printf("\n");}
    return ret;
}

/**
 * Judge whether 2 users are the same person by user_id
 * -true: the same person
 * -flase: not the same person
 */
bool same(User a, User b){
    return a.uid == b.uid;
}

/**
 * print user_name and user_id
 */
void user_print(User a){
    printf("%s(%d)", user_name[a.uid], a.uid);
}

/**
 * obtain user_name through user_id
 */
char* get_user_name(User a){
    return user_name[a.uid];
}

/**
 * by increasing priority order
 */
char level_str[level_max][level_name_length] = {"None", "gathering", "groupStudy", "projectMeeting", "privateTime"};

/**
 * Init all
 */
void init_all(long long start_date, long long end_date, int number_of_users, char users_name[][name_length]){
    init_time_slot(start_date, end_date);
    init_user(number_of_users, users_name);
}

/**
 * Set an appointment
 */
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
    ret.p.st = time2Slot(ret.p.start_time);
    ret.p.ed = ret.p.st + ret.len;

    ret.caller = load_user(caller_name);

    ret.number = number_of_callees;
    for(int i = 0; i < number_of_callees; i++){
        ret.callee[i] = load_user(callee_name[i]);
    }

    ret.ifconfirmed = false;

    return ret;
}

/**
 * Set a private_time appointment
 */
Appointment load_private_ap(int user_id, long long date_str, long long time_str, double period_length){
    if(DEBUG_ALL) printf("load_private_ap\n");
    Appointment ret;
    ret.level = private_level;

    ret.len = ceil(slot_per_hour * period_length);

    ret.p.start_time = str2Time(date_str * 10000 + time_str);
    int _tmp = ceil(period_length * 100);
    int _hour = _tmp / 100;
    int _minute = ceil((_tmp % 100) / 100.0 * 60);
    ret.p.end_time = add_time(ret.p.start_time, _hour, _minute);
    ret.p.st = time2Slot(ret.p.start_time);
    ret.p.ed = ret.p.st + ret.len;

    ret.caller.uid = user_id;
    ret.number = 0;

    ret.ifconfirmed = false;

    return ret;
}

/**
 * Set the status of the appointment 
 */
Appointment set_if_confirmed(Appointment a, bool flags){
    a.ifconfirmed = flags;
    return a;
}

/**
 * Judge whether there exists a conflict by calling "period_conflict()"
 */
bool ap_conflict(Appointment a, Appointment b){
    return period_conflict(a.p, b.p);
}

/**
 * Judge whether a user is a participant of an appointment
 */
bool contains(Appointment a, User b){
    if(same(a.caller, b)) return true;
    for(int i = 0; i < a.number; i++){
        if(same(a.callee[i], b)) return true;
    }
    return false;
}

//Get level Name
char* get_level_name(int a){
    return level_str[a];
}

//Print an appointment
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
