/**
 * @file appointment.c
 * @author Group 08
 * @brief Contains the implementation of all algorithm-related code blocks
 * @version Final 1
 * @date 2023-03-28
 * @attention 
 *  This file is part of API.\n
 *  We call an event that needs to arrange an appointment
 *  
 * 
 */
#include "appointment.h"
#include "ipc_user.h"
#include "ipc_schd.h"

int days_of_holidays = 6;
date_t holidays[] = {{20230305, 2023, 3, 5}, {20230312, 2023, 3, 12}, {20230319, 2023, 3, 19}, {20230326, 2023, 3, 26}, {20230329, 2023, 3, 29}, {20230329, 2023, 3, 30}};/**< List all the dates of the holidays */

extern int schd_cnt = 0;
extern schd_t schd_list[MAX_APPOINTMENT_NUM];
extern bool if_rejected[MAX_APPOINTMENT_NUM];
int day_num;
extern int total_user_num;
date_t st_day, ed_day;

/**
 * @brief init a period that an appointment canbe created.
 * 
 * @param[in] start_day: start date of the period
 * @param[in] end_day: end date of the period
 * @param[in] people_num: number of user
 * @note This function does not create any appointments, but just claims a legel period.
 * 
 * @par Smaple
 * @code 
 *      int start_day = 20130305;
 *      int end_day = 20130320;
 *      int num = 9;
 *      init_appointment(start_day,end_day,num);
 * @endcode
 */
void init_appointment(long long start_day, long long end_day, int people_num){
    st_day = str_to_date(start_day);
    ed_day = str_to_date(end_day);
    total_user_num = people_num;
    day_num = days(ed_day)+1;
}

/**
 * @brief Pass in an arrangement and judge whether it can be arranged
 * 
 * @param[in] a: an appointment 
 * @return true: the time is avaliable, or the privilege is high enough(the original appointment can be replaced)
 * @return false: time is not avaliable, and the privilege is not high enough.
 * @note This function is used to detect whether an appointment can be inserted,and returns a boolean.
 * <p>
 *  The function itself does not do any operation to the current schedule. 
 */
bool user_query_schd(schd_t a){
    for(int i = 0; i < schd_cnt; i++){
        if(if_schd_conflict(&a, &schd_list[i])){
            if(a.priv <= schd_list[i].priv) return false; // Cannot insert
        }
    }
    return true; // Able to insert
}


/**
 * @brief Execute insertion operation
 * 
 * @param[in] a : the appointment to be inserted.
 */
void user_insert_schd(schd_t a){
    schd_list[schd_cnt++] = a;
}
/**
 * @brief When a scheduling conflict occurs and the user insists on scheduling an event with a higher priority.\n
 * Then first query all existing events that conflict with this event.
 * 
 * @param[in] a: the high privilege appointment
 * @param[in] out: an appointment list and will be filled with all the appointments that will be deleted.
 * @return int : the numebr of appointments to be deleted.
 * @note This function is used to query, does not do deletion operation.
 */
int user_delete_query(schd_t a, schd_t *out){
    int p = 0;
    for(int i = 0; i < schd_cnt; i++){
        if(if_schd_conflict(&schd_list[i], &a)){
            out[p++] = schd_list[i];
        }
    }
    return p;
}

/**
 * @brief Execute deletion operation
 * 
 * @param[in] id : the appointment_id of the deleted appointment.
 */
void user_delete_schd(int id){
    for(int i = schd_cnt-1; i >= 0; i--){
        if(schd_list[i].id == id){
            schd_cnt--;
            for(int j = i; j < schd_cnt; j++){
                schd_list[j] = schd_list[j+1];
            }
        }
    }
}

/**
 * @brief Generate all the apporoved appointments
 * 
 * @param[in] out: an appointment array that will be filled with the appointments 
 * @return int : The total number of the appointments
 */
int user_print_schd(schd_t *out){
    for(int i = 0; i < schd_cnt; i++){
        out[i] = schd_list[i];
    }
    return schd_cnt;
}

/**
 * @brief convert a date into a str
 * 
 * @param[in] d : the date to be transfferd.
 * @return long long 
 * @note The "str" here is not a real "string", but a "longlong" integer.
 * 
 * @par Sample
 * @code
 *      date_t d;
 *      long long str;
 *      d.year = 2013;
 *      d.month = 12;
 *      d.day = 24;
 *      str = date_to_str(d);
 * @endcode
 */
static long long date_to_str(date_t d){
    return d.year * 10000ll + d.month * 100 + d.day;
}
/**
 * @brief convert a str to a date
 * 
 * @param s : a integer of type long long
 * @return date_t : a date
 * @note The "str" here is not a real "string", but a "longlong" integer.
 * 
 * @par Sample
 * @code 
 *      date_t a;
 *      long long s = 20131224;
 *      a = str_to_date(s);
 *      // a.year == 2013; a.month == 12; a.day == 24;
 * @endcode
 */
static date_t str_to_date(long long s){
    date_t ret;
    ret.str = s;
    ret.year = s / 10000;// obtain year
    ret.month = (s / 100) % 100;// obtain month
    ret.day = s % 100;// obtain day
    return ret;
}
/**
 * @brief Get the difference in number of days between input date date and the start date
 * 
 * @param[in] a: an input date
 * @return int : the difference in number of days
 * @note The difference in number of days exclude the holidays.
 * 
 * @par Sample
 * @code
 *      // Assume already inited start date as 2013.12.01
 *      date_t a;
 *      a.year = 2013;
 *      a.month = 12;
 *      a.day = 4;
 *      int num = days(a);
 *      // then the num == 3;
 * @endcode 
 *      
 */
static int days(date_t a){
    int p = 0;
    for(date_t i = st_day; !eq(i, a); i = date_add_day(i)){
        p++;
        for(int j = 0; j < days_of_holidays; j++){
            if(eq(i, holidays[j])){
                p--;// exclude holiday
                break;
            }
        }
    }
    return p;
}
/**
 * @brief Add one day to the current date to get a new date
 * 
 * @param[in] a: input date 
 * @return date_t : the new date after adding one day to the original date.
 */
static date_t date_add_day(date_t a){
    if(a.month == 2 && a.day == 28){
        // leap year judgement
        if((a.year % 4 == 0 && a.year % 100 != 0) || a.year % 400 == 0) a.day = 29;
        else{ a.month = 3; a.day = 1; }
        return a;
    }

    a.day++;

    // days out of range,enter into next month/ next year
    int mth[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(a.day > mth[a.month]){
        a.day = 1;
        a.month++;
        if(a.month > 12){
            a.month = 1;
            a.year++;
        }
    }
    a.str = date_to_str(a);
    return a;
}
/**
 * @brief Add a period of time to the current time to get a new time
 * 
 * @param[in] a: input time 
 * @param[in] h: hours to be added 
 * @param[in] m: minutes to br added 
 * @return tm_t : the new time after adding a period to the original time
 */
static tm_t time_add_hm(tm_t a, int h, int m){
    a.minute += m;
    h += a.minute / 60;
    a.minute %= 60;
    a.hour += h;
    while(a.hour >= 24){
        //if hour greater than 24, add a day
        a.hour -= 24;
        a.date = date_add_day(a.date);
    }
    a.str = time_to_str(a);
    return a;
}
/**
 * @brief convert a str to time
 * 
 * @param[in] s: input str 
 * @return tm_t : a time
 * @note The "str" here is not a real "string", but a "longlong" integer.
 * 
 * @par Sample
 * @code
 *      long long s = 201312241930;
 *      tm_t a = str_to_date(s);
 * @endcode
 */
static tm_t str_to_time(long long s){
    tm_t ret;
    ret.str = s;
    ret.date = str_to_date(s/10000);//obtain date
    ret.hour = (s / 100) % 100;//obtain hour
    ret.minute = s % 100;//obtain minute
    return ret;
}
/**
 * @brief convert a time to str
 * 
 * @param[in] a:  input time
 * @return long long : returned "str"
 * @note The "str" here is not a real "string", but a "longlong" integer.
 * 
 */
static long long time_to_str(tm_t a){
    return date_to_str(a.date) * 10000 + a.hour * 100 + a.minute;
}
/**
 * @brief convert a time into its timeslot
 * 
 * @param[in] a: the input time 
 * @return int : the timeslot
 * @note timeslot is hourly based. Each hour is a timeslot. And the timeslot is represented by a number.\n
 *   (Each timeslot has a unique number)
 */
static int time_to_slot(tm_t a){
    for(int i = 0; i < days_of_holidays; i++){
        if(eq(a.date, holidays[i])){
            return -1;
        }
    }
    int base = days(a.date);
    return (base * (END_HOUR - START_HOUR) + (a.hour - START_HOUR+1)) * SLOT_PER_HOUR + a.minute/(60/SLOT_PER_HOUR);
}
/**
 * @brief Determine which time slot a time belongs to
 * 
 * @param[in] a: input time.
 * @return int : the number that represents a timeslot
 * @note It is mainly used to solve the borderline situation when determining the time slot
 */
static int time_minus_minute_to_slot(tm_t a){
    if(a.minute % (60/SLOT_PER_HOUR) == 0)
        return time_to_slot(a)-1;
    else return time_to_slot(a);
}
/**
 * @brief convert a timeslot number into a time
 * 
 * @param[in] s: input timeslot number 
 * @return tm_t : the corresponding time
 */
static tm_t slot_to_time(int s){
    tm_t ret;
    int d = s / (SLOT_PER_HOUR * (END_HOUR - START_HOUR));
    int r = s % (SLOT_PER_HOUR * (END_HOUR - START_HOUR));
    int p = d;

    for(ret.date = st_day; p > 0; ret.date = date_add_day(ret.date)){
        p--;
        for(int j = 0; j < days_of_holidays; j++){
            if(eq(ret.date, holidays[j])){
                p++;// exclude holiday
                break;
            }
        }
    }
    ret.hour = r / SLOT_PER_HOUR - 1 + START_HOUR;
    ret.minute = (r % SLOT_PER_HOUR) * (60 / SLOT_PER_HOUR);
    ret.str = time_to_str(ret);
    return ret;
}

/**
 * @brief To judge whether a conflict occurs
 * 
 * @param[in] a: an appointment 
 * @param[in] b: another appointment 
 * @return true : the 2 input appointments does not conflict
 * @return false : the 2 input appointments conflicts
 */
static bool if_schd_conflict(const schd_t *a, const schd_t *b){
    return !((a->end_slot < b->start_slot) || (a->start_slot > b->end_slot));
}

/**
 * @brief create an appointment
 * 
 * @param id : appointment id, each appointment has a unique id
 * @param caller : the caller of the appointment.
 * @param num_of_callee : number of callee.
 * @param callee : a list that lists all the callees
 * @param type : the appointment type.(Basically, there are 4 types: privateTime,projectMeeting,groupStudy,gathering)
 * @param start_time: stat=rt tiem of the appointment
 * @param len : lasting time of the appointment(timeslot based)
 * @return schd_t : return an appointment
 */
schd_t load_schd(int id, int caller, int num_of_callee, int *callee, int type, long long start_time, double len){
    schd_t ret;
    ret.id = id;
    ret.caller = caller;
    ret.callee_num = num_of_callee;
    for(int i = 0; i < num_of_callee; i++)
        ret.callee[i] = callee[i];
    ret.type = type;
    ret.start_time = str_to_time(start_time);
    ret.len = len;
    int mm = ceil(len * 60);
    int hh = mm / 60;
    mm %= 60;
    ret.end_time = time_add_hm(ret.start_time, hh, mm);
    ret.start_slot = time_to_slot(ret.start_time);
    ret.end_slot = time_minus_minute_to_slot(ret.end_time);
    ret.priv = 0;
    ret.if_user_choose_to_reject = 1;
    return ret;
}
/**
 * @brief 
 * 
 * @param s:current appointment 
 * @return schd_t 
 */
schd_t re_schd(schd_t s){ // For main process to use to suggest a new time
    schd_t t;
    t.type = 0;
    t.priv = 0;
    t.caller = 0;
    int slot_len = ceil(s.len * SLOT_PER_HOUR);
    for(int i = 0; i < day_num; i++){

        int base = i * SLOT_PER_DAY;
        for(int j = 0; base+j+t.end_slot <= SLOT_PER_DAY; j++){

            t.start_slot = base+j+1;
            t.end_slot = base+j+slot_len;
            bool ok = 1;
            ok &= ipc_schd_insert_query(t.caller, &t);
            for(int k = 0; k < s.callee_num && ok; k++){
                ok &= ipc_schd_insert_query(t.callee[k], &t);
            }
            if(ok){
                t.start_time = slot_to_time(t.start_slot);
                t.end_time = slot_to_time(t.end_slot);
                int mm = 60 / SLOT_PER_HOUR;
                t.end_time = time_add_hm(t.end_time, mm / 60, mm % 60);
                return t; // return the time
            }
        }
    }
    t.type = -1; // failed to find a new time
    return t;
}

// For scheduler operating

bool schder_insert_query(schd_t s){
    bool ok = true;
    ok &= ipc_user_insert_query(s.caller, &s);
    for(int i = 0; i < s.callee_num & ok; i++)
        ok &= ipc_user_insert_query(s.callee[i], &s);
    if_rejected[s.id] = !ok;
    return ok;
}



void schder_insert(schd_t s){
    if(if_rejected[s.id] || s.if_user_choose_to_reject){
        ipc_user_insert(0, &s);
        return ;
    }
    schder_delete_query(s);
    for(int i = 0; i < schd_cnt; i++){
        if(if_rejected[schd_list[i].id]) continue;;
        if_rejected[schd_list[i].id] = 1;
        ipc_user_insert(0, &schd_list[i]);
    }
    schder_delete();
    ipc_user_insert(s.caller, &s);
    for(int i = 0; i < s.callee_num; i++)
        ipc_user_insert(s.callee[i], &s);
}

static int schder_delete_query(schd_t s){
    schd_cnt = 0;
    int m = ipc_user_delete_query(&s, schd_buffer);
    for(int i = 0; i < m; i++){
        schd_list[i] = schd_buffer[i];
    }
    schd_cnt = m;
    for(int i = 0; i < s.callee_num; i++){
        m = ipc_user_delete_query(&s, schd_buffer);
        for(int j = 0; j < m; j++){
            schd_list[schd_cnt+j] = schd_buffer[j];
        }
        schd_cnt += m;
    }
}

static void schder_delete() {
    for(int i = 0; i < schd_cnt; i++){
        ipc_user_delete(schd_list[i].caller, schd_list[i].id);
        for(int i = 0; i < schd_list[i].callee_num; i++){
            ipc_user_delete(schd_list[i].callee[i], schd_list[i].id);
        }
    }
}

int schder_print(schd_t *out){
    for(int i = 0; i <= total_user_num; i++){
        ipc_user_print(i);
    }
    for(int i = 0; i < schd_cnt; i++)
        out[i] = schd_list[i];
    return schd_cnt;
}

bool schder_schd(schd_t s){ // not used
    bool ok = schder_insert_query(s);
    if(ok){
        schder_insert(s);
        return true;
    }
    else{
        //--------- re schd Part -----------
        return false;
    }
}

void switch_to_reject_mode(schd_t *a){
    a->if_user_choose_to_reject=1;
}